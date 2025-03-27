// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "general.hpp"
#include "text.hpp"
#include "TFAccess.hpp"
#include "TFTemplate.hpp"
#include "TFCompile.hpp"
#include "extern.hpp"

namespace Scripal {

// ****************************************************************************
// prepare object
// aSource - Scripal source code
// aOutput - associated Scripal object
// ****************************************************************************
TFCompile::TFCompile(const std::string &aSource, TFScripal &aOutput) {
  output = &aOutput;
  source = aSource;
  std::vector<std::string> args {};
 
  clearError();

  // fill in template source
  templates.read(source); 
  templates.apply(source, args); 

  if (errorMsg != ERR_NONE) {
    // set source in TFScripal object to modified source
    output->source = source;
    return;
  }

  // read config settings
  aOutput.jsonConfig = config.read(source);

  // set source in TFScripal object to modified source
  output->source = source;
}

// ****************************************************************************
// compile source
// return - false if error occured or ran more than once
// ****************************************************************************
bool TFCompile::run() {
  const TFIndentifier* curAtom = nullptr;
  std::size_t curPos;
  auto exitFcn = [&](bool aSuccess) { if (!aSuccess) output->code.clear(); if (output->jsonConfig != "") config.resetTemp();};

  clearError();
  output->code.clear();
 
  try {
    // clear output object
    output->reset();

    // set parameters from temporary config if it exists
    if (output->jsonConfig != "") {
      if (!config.setTemp(output->jsonConfig)) 
        return(false);
    };
    
    if (config.debugCompile) {
      log("\n----- compilation -----\n");        
      debug = true;
    }

    while (posStart < source.size()) {
      // check for atomic identifier
      curAtom = nullptr;

      for (const TFIndentifier& item : identifiers) {
        if (compareTextICase(source.substr(posStart, item.identifier.size()), item.identifier) == CMP_EQU) {
          if ((posStart + item.identifier.size()) < source.size()) {
            char curChar = source[posStart + item.identifier.size() - 1];
            char nextChar = source[posStart + item.identifier.size()];
            if (((curChar >= 'A') && (curChar <= 'Z')) || ((curChar >= 'a') && (curChar <= 'z')))
              if (((nextChar >= 'A') && (nextChar <= 'Z')) || ((nextChar >= 'a') && (nextChar <= 'z')))
                // English character follows character identifier -> continue
                continue; 
          }        
          // atom found
          curAtom = &item;
          break;
        };
      };

      if (curAtom != nullptr) {
        // atom found -> set atom
        setAtom(*curAtom);
        posStart = posEnd + 1; posEnd = nPos;
      } else if ((curPos = findAnyNumber(posStart)) != nPos) {
        // number found
        posEnd = curPos;
        setNumber(source.substr(posStart, posEnd -posStart + 1)); 
        posStart = posEnd + 1; posEnd = nPos;       
      } else if (source[posStart] == ' ') {
        // white space -> continue
        posStart += sizeof(' '); posEnd = nPos;
      } else {
        // non white space but no atom -> error
        setError(ERR_UNKNOWN_IDENTIFIER, posStart,source);
        exitFcn(false); return(false);
      };
    };  

    // any operator still open?
    if (operatorOpen)
      closeOperator();
  
  } catch (const std::exception& aExc) {
    // exception but no fatal message? set general syntax error
    if (std::strcmp(aExc.what(), ERR_FATAL.c_str()) != 0) {
      setError(ERR_SYNTAX, posStart,source);
    }

    exitFcn(false); return(false);
  };

  // anything not completed?
  if (blocks.size() > 0) {
    setError(ERR_NO_BLOCK_END, nPos, source);
  }
  if (newGroup) {
    setError(ERR_NO_GROUP_END, nPos,source);
  }

  if (errorMsg != ERR_NONE) {
    exitFcn(false); return(false);
  }

  // set final null operator in case of jump past end
  setOperator(nullOprIdentifier);
  closeOperator();
  
  if (config.showCode) {
    std::string code = printCode(output->code);
    log("\n----- compiled code -----\n\n" + code);
  }
  exitFcn(true);
  return(true);
}

// ****************************************************************************
// set atom type
// aAtom - atom type 
// ****************************************************************************
void TFCompile::setAtom(const TFIndentifier& aAtom) {
  switch (aAtom.grammarType) {
     case SP_GRAMMAR_OPR:
       // new operator found
       if (operatorOpen) {
         assignOperator(aAtom);
       } else {
         setOperator(aAtom);
       };  
       break;
     case  SP_GRAMMAR_OPD:
       // new operand found 
       assignOperand(aAtom);
       break;
     case SP_GRAMMAR_COND:
       // new operator condition found 
       setCondition(aAtom);
       break;
     case SP_GRAMMAR_CTRL:
       // new control type found
       setControl(aAtom);
       break;
     case SP_GRAMMAR_ATTR:
       // new operand attribute found  
       setAttribute(aAtom);
       break;
  }
}

// ****************************************************************************
// set control type
// aControl - control type
// ****************************************************************************
void TFCompile::setControl(const TFIndentifier& aControl) {
  switch(aControl.codeID) {
    case SP_CTRL_GROUP_BEGIN:
      // group starts
      startGroup();
      posEnd = posStart + aControl.identifier.size() - 1; 
      break;

    case SP_CTRL_GROUP_END:
      // group ends
      endGroup();
      posEnd = posStart + aControl.identifier.size() - 1; 
      break;

    case SP_CTRL_TEXT: {
      // text start
      posEnd = findText(posStart, aControl.identifier);
      if (posEnd == nPos) {
        setError(ERR_NO_TEXT_END, posStart, source);
        throw(std::runtime_error(ERR_FATAL));
      };
      setText(extractContent(posStart, posEnd, aControl.identifier));
      break; };

    case SP_CTRL_NAME: {
      // name starts
      posEnd = findName(posStart);
      if (posEnd == nPos) {
        setError(ERR_NO_NAME_END, posStart ,source);
        throw(std::runtime_error(ERR_FATAL));
      };
      setName(extractContent(posStart, posEnd, ctrlName));
      break; };

    case SP_CTRL_RANGE_BEGIN: {
      // range starts
      posEnd = findRange(posStart);
      if (posEnd == nPos) {
        setError(ERR_NO_RANGE_END, posStart, source);
        throw(std::runtime_error(ERR_FATAL));
      };
      setRange(source.substr(posStart + ctrlRangeBegin.size(), posEnd - posStart + 1 - ctrlRangeBegin.size()- ctrlRangeEnd.size()));
      break; };

    case SP_CTRL_RANGE_END:
      setError(ERR_NO_RANGE_BEGIN, posStart, source);
      throw(std::runtime_error(ERR_FATAL));
      break; 

    case SP_CTRL_BLOCK_BEGIN:
      // block starts
      startBlock(); 
      posEnd = posStart + aControl.identifier.size() - 1; 
      break; 

    case SP_CTRL_BLOCK_END:
      // block ends
      endBlock();
      posEnd = posStart + aControl.identifier.size() - 1; 
      break; 
 
    case SP_CTRL_SEMICOLON:
    case SP_CTRL_NL:
    case SP_CTRL_CR:
      // end of operator
      if (operatorOpen) {
        if (accessOperator().codeID != SP_OPR_NULL)       
          closeOperator(); 
      }
      lastBlock = nPos;
      posEnd = posStart + aControl.identifier.size() - 1; 
      // special case /cr/nl
      if (source.substr(posEnd + 1, ctrlNL.size()) == ctrlNL) {
        posEnd += ctrlNL.size();
      };  
      break;

    case SP_CTRL_COMMENT:
      // comment
      posEnd = posStart;
      while ((posEnd < source.size()) && (posEnd != nPos)) {
        if (source.substr(posEnd, ctrlCR.size()) == ctrlCR) {
          posEnd += ctrlCR.size() - 1;
          // special case /cr/nl ?
          if (posEnd + 1 <= source.size())
            if (source.substr(posEnd + 1, ctrlNL.size()) == ctrlNL) 
              posEnd += ctrlNL.size();
          break;    
        };

        if (source.substr(posEnd, ctrlNL.size()) == ctrlNL) {
          posEnd += ctrlNL.size() - 1;
          break;
        };  

        posEnd = getNextCP(source, posEnd);
      }

      if (posEnd == nPos)
        posEnd = source.size() -1 ;
      break;
      
    default:
      posEnd = posStart + aControl.identifier.size() - 1; 
  };  
}

// ****************************************************************************
// set empty operator and assign condition
// aCondition - condition
// ****************************************************************************
void TFCompile::setCondition(const TFIndentifier& aCondition) {

  TFOperator* newOpr = nullptr; 
  if (operatorOpen) {
    setError(ERR_NO_OPR_END, posStart, source);
    throw(std::runtime_error(ERR_FATAL));
  } else {
    // open new dummy Operator
    newOpr = &setOperator(nullOprIdentifier);
  }
  newOpr->condition = aCondition.codeID;

  // jump to next operator, if block starts, block wil set long jump 
  newOpr->jumpTo = output->code.size();
 
  posEnd = posStart + aCondition.identifier.size() - 1;
 
  if (debug) log("set condition '" + aCondition.identifier + "' jump to: (" + std::to_string(newOpr->jumpTo)+ ")");

  return;
}

// ****************************************************************************
// is a special handling for last operator necessary?
// aOperator - operator
// return - new operator
// ****************************************************************************
void TFCompile::specialOperator() {
  TFOperator& opr = output->code.back();
  
  // special case matches, add state status to begin of block
  if (opr.codeID == SP_OPR_TOTAL) {
    // default value for matches is 1
    opr.operand.values = {1,1};
    TFState state;
    output->code[opr.jumpTo].states.insert({output->code.size() -1, state});  
  } 
}

// ****************************************************************************
// set new operator
// aOperator - operator
// return - new operator
// ****************************************************************************
TFOperator& TFCompile::setOperator(const TFIndentifier&  aOperator) {

  if (operatorOpen) {
    setError(ERR_NO_OPR_END, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  } else {
    operatorOpen = true;
    posOpr = posStart;
 
    // create new operator
    TFOperator newOpr = TFOperator(SP_COND_ANYMATCH, aOperator.codeID, nullOperand);
 
    if (lastBlock != nPos) {
      // jump to begin of last block if exists (example loop, matches)
      newOpr.jumpTo = lastBlock;
      lastBlock = nPos;
    } else {
      // jump to begin of code if no block just ended
      newOpr.jumpTo = 0;
    }
    output->code.push_back(newOpr);
    
    // special operator?
    specialOperator();
  };
  
  posEnd = posStart + aOperator.identifier.size() - 1;

  if (debug) log("set operator '" + aOperator.identifier + "'", output->code.size() - 1);

  return(output->code.back());
}  

// ****************************************************************************
// assign operator type to existing dummy
// aOperator - operator
// ****************************************************************************
void TFCompile::assignOperator(const TFIndentifier&  aOperator) {

  checkOprOpen();

  if (accessOperator().codeID != SP_OPR_NULL) {
    setError(ERR_NO_OPR_END, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  };
  
  // set expected operand type for operator
  accessOperator().codeID = aOperator.codeID;
  expectedOpd = oprParms[aOperator.codeID].opdtype;
  
  // special operator?
  specialOperator();

  posEnd = posStart + aOperator.identifier.size() - 1;

  if (debug) log("assign operator '" + aOperator.identifier + "'", output->code.size() -1);
}  

// ****************************************************************************
// start block
// ****************************************************************************
void TFCompile::startBlock() {
  // remember operator position 
  std::size_t pos;
  if (operatorOpen) {
    pos = output->code.size() - 1;
  } else {
    pos = output->code.size();
  }

  blocks.push_back(pos);
  if (debug) log("start block" ,pos);
}

// ****************************************************************************
// end block, may throw exception
// ****************************************************************************
void TFCompile::endBlock() {

  // set jump position
  if (blocks.size() == 0) {
    setError(ERR_NO_BLOCK_BEGIN, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  }
  if (blocks.back() >= output->code.size()) { 
    setError(ERR_NO_BLOCK_BEGIN, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  }
 
  // remember begin of last block 
  lastBlock = blocks.back();

  // set label and erase last block entry, block is done
  if (!operatorOpen) {
    // no operator open yet, new ouptut element will follow
    output->code[blocks.back()].jumpTo = output->code.size();
  } else {
    // operator open , jump to following operator, if not same as last block, set to new operator
    output->code[blocks.back()].jumpTo = output->code.size();
    if ((output->code.size() - 1) != blocks.back())
      output->code[output->code.size() - 1].jumpTo = blocks.back();
    closeOperator();
  }

  if (debug) log("end block starting", blocks.back());

  blocks.pop_back();
}

// ****************************************************************************
// close operand
// ****************************************************************************
void TFCompile::closeOperand() {
  if (!operandOpen) 
    return;
  
  if (debug) log("close operand");
  
  TFOperand* curOpd = nullptr;
  curOpd = &accessOperand();

  // ******************** 
  // integrity check  
  if (curOpd->opdtype == SP_OPD_LOGICAL) {
  }

  operandOpen = false;
}

// ****************************************************************************
// close operator
// ****************************************************************************
void TFCompile::closeOperator() {
  
  if (!operatorOpen) 
    return;
 
  if (debug) log("operator done: '" + printOpr(accessOperator()) + "'", output->code.size() - 1);
 
  firstOpd = false;
  lastBlock = nPos;

  // ***************************
  // integrity check

  // group left open?
  if (opdPath.size() > 0) {
    setError(ERR_NO_GROUP_END, posStart, source); 
    throw(std::runtime_error(ERR_FATAL)); 
  }
  
  // does operand match expected type?
  if (!oprParms[accessOperator().codeID].opdtype.test(accessOperator().operand.opdtype)) {
    setError(ERR_WRONG_OPERAND, posOpr, source); 
    throw(std::runtime_error(ERR_FATAL)); 
  };
  
  // end of operator, also close operand
  if (operandOpen) 
    closeOperand(); 
 
  // end of group
  if (newGroup) 
    endGroup();

  expectedOpd.reset();
  opdPath.clear();   
  operatorOpen = false;
  if (debug) log("close operator");
}

// ****************************************************************************
// start new group
// return - operand group
// ****************************************************************************
std::vector<TFOperand>& TFCompile::startGroup() {
  TFOperand* curOpd = nullptr; 

  if (debug) log("start group");

  checkOprOpen();
  
  if (!operandOpen) {
    curOpd = &setOperand();
  } else {
    curOpd = &accessOperand();
  }

  if (curOpd->opdtype == SP_OPD_NULL) {
    // use all as default operand if not specified
    curOpd->opdtype = SP_OPD_LOGICAL;
    curOpd->codeID = SP_LOGIC_ALL;
  } 

  if (curOpd->opdtype != SP_OPD_LOGICAL) {
    // only logical operand can start group  
    setError(ERR_WRONG_OPERAND , posStart, source);
    throw(std::runtime_error(ERR_FATAL));
  }

  // clear group for new operands
  curOpd->operands.clear();

  newGroup = true;
  closeOperand();

  return(curOpd->operands);
}

// ****************************************************************************
// end group
// ****************************************************************************
void TFCompile::endGroup() {
  if (debug) log("end group");

  if (operandOpen) 
    closeOperand();
 
  newGroup = false;

  // decrease path level
  if (opdPath.size() == 0) {
    setError(ERR_NO_GROUP_BEGIN , posStart, source);
    throw(std::runtime_error(ERR_FATAL));
  }
    
  opdPath.pop_back();  

  return;
}

// ****************************************************************************
// set new null operand
// return - new operand
// ****************************************************************************
TFOperand& TFCompile::setOperand() {
  checkOprOpen();

  if (operandOpen) {
    // operand open!
    setError(ERR_NO_OPD_END, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  }  
  
  // create new operand and add to group
  TFOperand newOpd;
   
  // integrity check, prevent second main operand
  if (opdPath.size() == 0) {
    if (accessOperator().operand.opdtype == SP_OPD_NULL) {
      firstOpd = true;
    } else {
      if (firstOpd) {
        firstOpd = false;
      } else { 
        setError(ERR_WRONG_OPERAND, posOpr, source); 
        throw(std::runtime_error(ERR_FATAL));
      }
    }  
  }
  
  if (newGroup) {
    TFOperand& curOpd = accessOperand();
    std::vector<TFOperand>& opdGroup = curOpd.operands;
    opdPath.push_back(0);  
    opdGroup.push_back(newOpd);
    newGroup = false;
  } else { 
    if (opdPath.size() > 0) {
      std::vector<TFOperand>& opdGroup = accessParent().operands;
      opdGroup.push_back(newOpd);
      ++opdPath.back(); 
    }  
  }

  operandOpen = true;
  return(accessOperand());
}

// ****************************************************************************
// set operand attribute 
// aAttribute -attribute
// ****************************************************************************
void TFCompile::setAttribute(const TFIndentifier&  aAttribute) {
  TFOperand* curOpd = nullptr;

  checkOprOpen();
  
  if (operandOpen) {
    curOpd = &accessOperand();
    if (curOpd->codeID != SP_OPD_NULL) { 
      // block operand may still be open 
      closeOperand();
      curOpd = &setOperand();
    }
  } else {
    // open new Operator
    curOpd = &setOperand();
  }

  // set attribute and saveState flag 
  curOpd->attrs[aAttribute.codeID] = true;
  if ((curOpd->attrs[SP_ATTR_AT]) || (curOpd->attrs[SP_ATTR_SKIP]) || (curOpd->attrs[SP_ATTR_TEST]) || \
      (curOpd->attrs[SP_ATTR_TRY]) || (curOpd->attrs[SP_ATTR_LAST]) || (curOpd->attrs[SP_ATTR_NOTLAST]) || (curOpd->attrs[SP_ATTR_UNLESS]) )
    curOpd->saveState = true;  

  posEnd = posStart + aAttribute.identifier.size() - 1;
  if (debug) log(std::string("set attribute '" + aAttribute.identifier + "'"));

  return;
}

// ****************************************************************************
// specify operand, set existing one if null or new one 
// aCodeID - code ID
// return - operand
// ****************************************************************************
TFOperand* TFCompile::specifyOperand(std::size_t aCodeID) {
  TFOperand* curOpd = nullptr;

  checkOprOpen();

  if (operandOpen) {
    // former operand not closed 
    curOpd = &accessOperand();
    if (curOpd->codeID != SP_OPD_NULL) {
      closeOperand(); 
      curOpd = &setOperand();
    }
  } else {
    curOpd = &setOperand();
  }
  
  curOpd->codeID = aCodeID;
  if (logicOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_LOGICAL;
  } else if (blockOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_BLOCK;
  } else if (textOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_TEXT;
  } else if (nameOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_NAME;
  } else if (numberOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_NUMBER;
  } else if (rangeOpd(aCodeID)) {
    curOpd->opdtype = SP_OPD_RANGE;
  } 
  
  return(curOpd);
}
  
// ****************************************************************************
// assign operand
// aOperand - operand
// ****************************************************************************
void TFCompile::assignOperand(const TFIndentifier&  aOperand) {
  specifyOperand(aOperand.codeID);

  if (debug) log(std::string("set operand '" + aOperand.identifier + "'"));

  posEnd = posStart + aOperand.identifier.size() - 1; 

  // ******************** 
  // integrity check  

  // look for group begin or range begin as next character, else logical operand is corrupted
  if (logicOpd(aOperand.codeID)) {
    std::string groupBegin = findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_GROUP_BEGIN);
    std::string rangeBegin = findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_BEGIN);
    std::size_t pos = posEnd + 1;
    while (pos < source.size()) {
      if ((source[pos] != ' ') && (source.substr(pos, groupBegin.size()) != groupBegin) && (source.substr(pos, rangeBegin.size()) != rangeBegin) ) {
        // no range
        setError(ERR_NO_GROUP_BEGIN, posStart, source);
        throw(std::runtime_error(ERR_FATAL));
      };
      if ((source.substr(pos, groupBegin.size()) == groupBegin) || (source.substr(pos, rangeBegin.size()) == rangeBegin))
        break;
        
      ++pos;    
    } 
  }

}


// ****************************************************************************
// set text 
// aText - text string
// ****************************************************************************
void TFCompile::setText(const std::string &aText) {
  TFOperand* curOpd = specifyOperand(SP_OPD_TEXT);

  std::string newText = aText;
  unescape(newText);

  if (!curOpd->attrs[SP_ATTR_NOCASE]) {
    curOpd->text = newText;
  } else {
    // set to upper case 
    curOpd->text = upperText(newText);
  }  
  if (debug) log(std::string("set text operand \"" + newText + "\""));

  // close text operand immediately
  closeOperand();
}

// ****************************************************************************
// set name
// aName - name string
// ****************************************************************************
void TFCompile::setName(const std::string &aName) {
  TFOperand* curOpd = specifyOperand(SP_OPD_NAME);

  curOpd->text = aName;

  if (debug) log(std::string("set name '" + aName + "'"));

  // close name operand immediately
  closeOperand();
}

// ****************************************************************************
// extract text from controls
// aPosStart, aPosEnd : start and end position of content
// aDelimiter : delimiter type
// return - text part
// ****************************************************************************
std::string TFCompile::extractContent(std::size_t aPosStart, std::size_t aPosEnd, const std::string &aDelimiter) {
  std::string result = source.substr(aPosStart + aDelimiter.size(), aPosEnd - (aPosStart  + aDelimiter.size()) + 1 - aDelimiter.size());
  return(result);
}
  
// ****************************************************************************
// set number 
// aNumber - number as string
// ****************************************************************************
void TFCompile::setNumber(const std::string &aNumber) {
  TFOperand* curOpd = nullptr;
  bool openNum = false;

  if (operandOpen) {
    curOpd = &accessOperand();
    if (curOpd->codeID != SP_OPD_NULL) {
      openNum =true;
    } else {
      curOpd = specifyOperand(SP_OPD_NUMBER);
    }
  } else {
    curOpd = specifyOperand(SP_OPD_NUMBER);
  }
 
  if (openNum) {
    textToNum(aNumber, curOpd->attrValues[0], curOpd->attrs, '.', ' ');
    curOpd->attrValues[1] = curOpd->attrValues[0]; 
  } else {
    textToNum(aNumber, curOpd->values[0], curOpd->attrs, '.', ' ');
    curOpd->values[1] = curOpd->values[0]; 
    closeOperand();
  } 
}

// ****************************************************************************
// set number range
// aRange - range string
// ****************************************************************************
void TFCompile::setRange(const std::string &aRange) {
  TFOperand* curOpd = nullptr;
  std::array<TFNum,2> values;
  bool openNum = false;
  std::string range = aRange;
  std::string ctrlText = "'";

  if (operandOpen) {
    curOpd = &accessOperand();
    if (curOpd->codeID != SP_OPD_NULL) {
      openNum =true;
    } else {
      curOpd = specifyOperand(SP_OPD_RANGE);
    }
  } else {
    curOpd = specifyOperand(SP_OPD_RANGE);
  }
 
  range.erase(std::remove(range.begin(), range.end(), ' '), range.end());
  std::size_t div = range.find(","); 
  
  bool textDelim = false;
  char32_t codePoint = getCP(range, 0);
  int idIndex = findIdentifier(SP_GRAMMAR_CTRL, toString(codePoint)); 
  if ((idIndex) != -1) {
    if (identifiers[idIndex].codeID == SP_CTRL_TEXT) {
      textDelim = true;
      ctrlText = identifiers[idIndex].identifier;
      codePoint = getCP(range, ctrlText.size());
    };  
  };

  try {
    if (range.substr(0, ctrlInfinite.size()) == ctrlInfinite) {
      values[0] = INFINTE_VALUE;
    } else if (range.substr(0, 1) == "0") {
      values[0] = 0;
    } else if (textDelim) {
      values[0] = codePoint;
    } else {
      if (!textToNum(range.substr(0, div), values[0], curOpd->attrs, '.', ' '))
        throw(std::runtime_error(ERR_FATAL));    
    }
    if (div != nPos) {
      // true range
      if (range.substr(div + 1, ctrlInfinite.size()) == ctrlInfinite) {
        values[1] = INFINTE_VALUE;
      } else if (range.substr(div + 1, ctrlText.size()) == ctrlText) {
        codePoint = getCP(range, div + 1 + ctrlText.size());
        values[1] = codePoint;
      } else {  
        if (!textToNum(range.substr(div + 1), values[1], curOpd->attrs, '.', ' ')) {
          setError(ERR_NUMBER_RANGE, posStart, source);
          throw(std::runtime_error(ERR_FATAL)); 
        }  
      }
    } else {
      // set second value like first if no true range 
      values[1] = values[0]; 
    }

   // first value shouldn't be infinite
   if (values[0] == INFINTE_VALUE) 
     values[0] = 0;
     
    if ((openNum) && (curOpd->codeID != SP_OPD_NULL)) {
      curOpd->attrValues = values;
      if (debug) log("set range attribute [" + printNumber(values[0], curOpd->attrs) + "," + printNumber(values[1], curOpd->attrs) + "]");
    } else {
      curOpd->values = values;
      if (debug) log("set range [" + printNumber(values[0], curOpd->attrs) + "," + printNumber(values[1], curOpd->attrs) + "]");
      closeOperand();
    }
  } catch(std::exception const& except) {
    // no range
    if (errorMsg == ERR_NONE)
      setError(ERR_BROKEN_RANGE, posStart, source);
    throw(std::runtime_error(ERR_FATAL));    
  }
}

// ****************************************************************************
// find end of number
// aPosStart - staring position of number  
// return - end position of number or nPos 
// ****************************************************************************
std::size_t TFCompile::findAnyNumber(std::size_t aPosStart) {
  std::size_t pos = aPosStart;
  TFOperand* curOpd = nullptr;
  // default for attributes is all zeros
  bool attrs[_SP_ATTR_END] = {0};

  if (operandOpen) {
    // use number format in attribute
    curOpd = &accessOperand();
    std::copy(std::begin(curOpd->attrs), std::end(curOpd->attrs), std::begin(attrs));
  } else {
    // use attrs default
  }

  // use TFAccess for number analysis  
  TFAccess sourceM;
  sourceM.assignString(&source); 
  pos = findNumber(sourceM, aPosStart, true, attrs);

  return(pos);
}

// ****************************************************************************
// find end of text, watch out for control characters, ctrlText is delimiter
// aPosStart - staring position of text , including delimiter 
// aCtrlText - delimiter
// return - end position of text or nPos 
// ****************************************************************************
std::size_t TFCompile::findText(std::size_t aPosStart, const std::string &aCtrlText) {
  std::size_t pos = aPosStart;

  pos += aCtrlText.size();
  while (!(pos >= source.size())) {
    if (source[pos] == escChar) {
      // move ahead two characters
      pos += sizeof(escChar);
      pos += getCPSize(source, pos);
    }

    if (source.substr(pos, aCtrlText.size()) == aCtrlText) {
      return(pos); 
    }
    pos += getCPSize(source, pos);
  }

  return(nPos);
}

// ****************************************************************************
// find end of name
// aPosStart - staring position of name, including delimiter
// return - end position of name or nPos  
// ****************************************************************************
std::size_t TFCompile::findName(std::size_t aPosStart) {
  std::size_t pos = aPosStart;
  
  pos += ctrlName.size();
  while (!(pos >= source.size())) {
    if (source.substr(pos, ctrlName.size()) == ctrlName) {
      return(pos); 
    }
    pos += getCPSize(source, pos);
  }

  return(nPos);
}

// ****************************************************************************
// find end of number range
// aPosStart - staring position of range, including delimiter 
// return - end position of range or nPos  
// ****************************************************************************
std::size_t TFCompile::findRange(std::size_t aPosStart) {
  std::size_t pos = aPosStart;

  while (!(pos == source.size())) {
    if (source.substr(pos, ctrlRangeEnd.size()) == ctrlRangeEnd) {
      return(pos); 
    }
    pos += getCPSize(source, pos);
  }

  return(nPos);
}

// ****************************************************************************
// access current operand group
// return - operand group
// ****************************************************************************
std::vector<TFOperand> &TFCompile::accessGroup() {
  std::vector<TFOperand>* result = new std::vector<TFOperand>; return(*result);
}

// ****************************************************************************
// access current operator
// return - operator
// ****************************************************************************
TFOperator& TFCompile::accessOperator() {
  checkOprOpen();

  return(output->code.back());
}

// ****************************************************************************
// access current operand 
// return - operand
// ****************************************************************************
TFOperand& TFCompile::accessOperand() {
  TFOperand* result = &output->code.back().operand;

  std::size_t index = 0;
  while (index < opdPath.size()) {
    result = &result->operands[opdPath[index]];
    ++index;
  }
  return(*result);
}

// ****************************************************************************
// access parent operand
// return - parent operand
// ****************************************************************************
TFOperand& TFCompile::accessParent() {
  TFOperand* result = &output->code.back().operand;

  std::size_t index = 0;
  while (index < opdPath.size() - 1) {
    result = &result->operands[opdPath[index]];
    ++index;
  }
  return(*result);
}

// ****************************************************************************
// test if operator is open, else throw fatal exception
// ****************************************************************************
void TFCompile::checkOprOpen()  {
  if (!operatorOpen) {
    setError(ERR_NO_OPR_BEGIN, posStart, source); 
    throw(std::runtime_error(ERR_FATAL));
  };
}

}