// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "general.hpp"
#include "algos.hpp"
#include "text.hpp"
#include "TFAccess.hpp"
#include "TFCompile.hpp"
#include "TFScripal.hpp"
#include "scripalLib.hpp"


namespace Scripal {

// ****************************************************************************
// operand match logic
// aOpd        - operand to match
// aParentOpd  - parent operand
// ****************************************************************************
void TFScripal::matchOpd(TFOperand& aOpd, TFOperand& aParentOpd) {
  std::size_t curPosPtr = nPos;
  TFState curState;

  // not counter
  if (aOpd.attrs[SP_ATTR_NOT])
    ++notCount;
  aOpd.reset();
 
  // save states for certain attributes to restore conditions
  if (aOpd.saveState) {
    saveState(curState, aOpd);
    curPosPtr = posPtr;
  }
 
  switch (aOpd.opdtype) {
    case SP_OPD_TEXT:
    case SP_OPD_NAME:
      // match text or name
      matchText(aOpd);
      break;
      
    case SP_OPD_NUMBER:
    case SP_OPD_RANGE:
      // match number or number range
      matchNumbers(aOpd);
      break;

    case SP_OPD_BLOCK:
      // match block
      matchBlock(aOpd);
      break;
 
    case SP_OPD_LOGICAL:
      // match logical
      matchLogical(aOpd);
      break;   
  };  
    
  if (aOpd.saveState) {
    if (aOpd.attrs[SP_ATTR_UNLESS]) {
      // set unless position
      if (aOpd.hasMatch) 
        unlessPos[posPtr] = &aOpd;
      else  
        setPos(curPosPtr);
        
      addPosMatch(aOpd);  
      restoreState(curState, aOpd);
    }  else if (aOpd.attrs[SP_ATTR_AT]) {
      // set position pointer to begin of match or restore, restore states
      if (aOpd.hasMatch) {
        setPos(aOpd.matchBegin);
      } else {
        setPos(curPosPtr);
      }
      restoreState(curState, aOpd);
    } else if (aOpd.attrs[SP_ATTR_SKIP]) {
      // restore states
      restoreState(curState, aOpd);
    } else if (aOpd.attrs[SP_ATTR_TEST]) {
      // restore position pointer and states
      setPos(curPosPtr);    
      restoreState(curState, aOpd);
    } else if (aOpd.attrs[SP_ATTR_TRY]) {
      if (!aOpd.hasMatch) {
        addPosMatch(curPosPtr, nPos, aOpd);
      }
    } else if ((aOpd.attrs[SP_ATTR_LAST]) || (aOpd.attrs[SP_ATTR_NOTLAST])) {
      // restore position pointer and states, remember finish status
      if (&aParentOpd != &nullOperand) { 
        aParentOpd.lastTry = true;
        aParentOpd.lastLogic = aOpd.attrs[SP_ATTR_LAST]; 
        if (aOpd.hasMatch) {
          aParentOpd.lastPos = aOpd.matchEnd; 
        };

      
        setPos(curPosPtr);
        restoreState(curState, aOpd);

        addPosMatch(aOpd);  
      }  
    }  
  };  

  // not counter
  if (aOpd.attrs[SP_ATTR_NOT])
    --notCount;

  return;
}

// ****************************************************************************
// block operand match logic
// aOpd - operand to match
// ****************************************************************************
isInline void TFScripal::matchBlock(TFOperand& aOpd) {
  std::size_t curBegin = posPtr, curEnd = nPos; 
  bool curMatch = false;
    
  if (posPtr == nPos) {
    // special case nPos, only EOT, EOW positive can match here
    // match to end of text, EOT is no valid position
    switch (aOpd.codeID) {
      case SP_BLOCK_EOT:
        addPosMatch(nPos, text.size() -1 , aOpd);
        break;
      case SP_BLOCK_EOW:
        char32_t codePoint = getCP(text, getPrevCP(text, nPos));
        if (isLetter(codePoint))
          addPosMatch(nPos, text.size() -1 , aOpd);
        break;
    };         
  
    return;
  }

  switch (aOpd.codeID) { 
    case SP_BLOCK_BOS:
      // set result to (curEnd, nPos), no result range
      curEnd = testBOS(text, posPtr); 
      if (curEnd != nPos) {
        addPosMatch(curEnd, nPos, aOpd);
        curMatch = true;
      }  
      break;

    case SP_BLOCK_EOS:
      // set result to (curBegin, curEnd), result range
      curEnd = findEOS(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      }  
      break;;

    case SP_BLOCK_BOL:
      // set result to (curEnd, nPos), no result range
      curEnd = testBOL(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(curEnd, nPos, aOpd);
        curMatch = true;
      }  
      break;;

    case SP_BLOCK_EOL:
      // set result to (curBegin, curEnd), result range
      curEnd = findEOL(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      }  
      break;;

    case SP_BLOCK_BOT:
      // set result to (0, nPos),  no result range
      if (posPtr == 0) { 
        addPosMatch(0, nPos, aOpd);
        curMatch = true;
      }
      break;

    case SP_BLOCK_EOT:
      // only valid on nPos which is handled earlier
      break;

    case SP_BLOCK_BOW:
      // set result to (curEnd, nPos),  no result range
      curEnd = testBOW(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(curEnd, nPos, aOpd);
        curMatch = true;
      }  
      break;

    case SP_BLOCK_EOW:
      // set result to (nPos, curEnd),  no result range
      curEnd = findEOW(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(nPos, curEnd, aOpd);
        curMatch = true;
      }  
      break;

    case SP_BLOCK_BOMARK:
      // set result to (curBegin, curEnd), result range
      curEnd = findBOMark(text, posPtr); 
      if (curEnd != nPos) { 
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      } 
      break;

    case SP_BLOCK_CHAR: {
      // result range over length of character
      char32_t codePoint = 0;
      int len = getCP(text, posPtr, codePoint); 
      if (len == 0) 
        break;

      if (std::isnan(aOpd.attrValues[0])) {
        // any character
        curMatch = true;
      } else if (std::isnan(aOpd.attrValues[1])) {
        // given character
        if (codePoint == aOpd.attrValues[0])
          curMatch = true;
      } else {       
        // character in range
        if ((codePoint >= aOpd.attrValues[0]) && (codePoint <= aOpd.attrValues[1])) 
          curMatch = true;
      }
      if (curMatch) {
        curEnd = curBegin + len - 1;
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
      };   
      break; }

    case SP_BLOCK_SPACE:
      // result range over space
      if (text.get(posPtr) == ' ') {
        curEnd = curBegin;
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      };
      break;

    case SP_BLOCK_BLANK:
      // result range over all spaces
      curEnd = findBlank(text, posPtr); 
      if (curEnd != nPos) {
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      }  
      break;

    case SP_BLOCK_LETTER: {
      // result range over letter
      char32_t codePoint = 0;
      int len = getCP(text, posPtr, codePoint); 
      if (isLetter(codePoint)) {
        curEnd = curBegin + len - 1;
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      };
      } break;

    case SP_BLOCK_DIGIT: {
      // result range over digit
      char32_t codePoint = 0;
      int len = getCP(text, posPtr, codePoint); 
      if (isDigit(codePoint, aOpd.attrs)) {
        curEnd = curBegin + len - 1;
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      };
      } break;

    case SP_BLOCK_WORD:
      // result range over word
      curEnd = findWord(text, posPtr);
      if (curEnd != nPos) {
        addPosMatch(curBegin, curEnd, aOpd);
        setLogicPos(curEnd + 1, aOpd);
        curMatch = true;
      }
      break;

    case SP_BLOCK_MOVE:
      // result range over skipped region
      curEnd = posPtr;
      if (!std::isnan(aOpd.attrValues[0])) {
        movePos((std::size_t)(double)(aOpd.attrValues[0]), curEnd);
      } else {  
        movePos(1, curEnd);
      }
      // move has special range of function logic
      if (curEnd != nPos) {
        setLogicPos(curEnd, aOpd);
        addPosMatch(aOpd);
        curMatch = true;
      }
      break;  
  };    

  // NOT attribute set and no match? 
  if (!curMatch)
    addNegMatch(posPtr, aOpd);

}

  
// ****************************************************************************
// logical operand match logic
// aOpd -operand to match
// ****************************************************************************
isInline  void TFScripal::matchLogical(TFOperand& aOpd) {
  uint16_t state = 0;
  std::size_t loopPosPtr, opdPosPtr;  // remember position at start of operand and loop
  TFOperand* lastOpd;
  TFState opdState, loopState;
  bool loop = true, first = true;
  static int count = 0;
  aOpd.matchCount = 0;
  
  // remember match/result before starting operand 
  saveState(opdState, aOpd);
  opdPosPtr = posPtr;
   
  // match group
  while (loop) {

    // initialize sub match counter
    aOpd.subCount = 0;
 
    if (first) {
      // start condition 
      switch (aOpd.codeID) { 
        case SP_LOGIC_FIND:
          // if first operand is text type -> quickly advance to position 
          if (textOpd(aOpd.operands[0].codeID)) 
            posToText(aOpd.operands[0], posPtr, nPos);
          break;
        case SP_LOGIC_FINDAT:
          // remember and move position pointer before matching
          aOpd.startPosPtr = posPtr;
        
          if (!std::isinf(aOpd.attrValues[0])) 
            movePos(static_cast<std::size_t>(aOpd.attrValues[0]), posPtr);
          else 
            setPos(nPos);  
          break;  
      };
    }
    
    // remember first loop postion and state to break operands
    saveState(loopState, aOpd);
    loopPosPtr = posPtr;
    lastOpd = nullptr;
    
    // loop over sub operands
    bool inSubOpd = false; 
    for (auto &curOpd : aOpd.operands) { 
      inSubOpd = true; 
    
      bool unlessCase = false; 
      std::map<std::size_t, TFOperand*>::iterator search;
      // is posPtr position set by previous operand?
      if ((search = unlessPos.find(posPtr)) != unlessPos.end()) {
        if (search->second == lastOpd) {
          addNegMatch(posPtr, curOpd);
          unlessCase = true;
        }  
      }  

      if (!unlessCase)  

       // scan all operands
        matchOpd(curOpd, aOpd);
      
      
      // remember start of match
      if ((aOpd.matchBegin == nPos) && (curOpd.matchBegin != nPos)) 
        aOpd.matchBegin = curOpd.matchBegin;
        
      aOpd.matchEnd = resultEnd;
      
      // child breaks parent?
      if (curOpd.breakOpd) {
        curOpd.breakOpd = false;
        state = SP_BREAKOPD | SP_NOMATCH;
        break;
      }
        
      switch (aOpd.codeID) { 

        case SP_LOGIC_ALL:
          state = logicalAll(curOpd, aOpd);
          break;

        case SP_LOGIC_ANY:
          state = logicalAny(curOpd, aOpd);
          break;

        case SP_LOGIC_EACH:
          state = logicalEach(curOpd, aOpd);
          break;

        case SP_LOGIC_EVERY: 
          state = logicalEvery(curOpd, aOpd);
          break;

        case SP_LOGIC_FIND:
          state = logicalFind(curOpd, aOpd);
          break;

        case SP_LOGIC_FINDAT:
          state = logicalFindAt(curOpd, aOpd);
          break;

        case SP_LOGIC_BREAK:
          state = logicalBreak(curOpd, aOpd);
          break;

        case SP_LOGIC_REPEAT:
          state = logicalRepeat(curOpd, aOpd);
          break;

        case SP_LOGIC_ISNUMBER:
          state = logicalIsNumber(curOpd, aOpd);
          break;

        case SP_LOGIC_ISWORD:
          state = logicalIsWord(curOpd, aOpd);
          break;

        case SP_LOGIC_ISUPPER:
          state = logicalIsCase(curOpd, aOpd, CL_UPPER);
          break;

        case SP_LOGIC_ISLOWER:
          state = logicalIsCase(curOpd, aOpd, CL_LOWER);
          break;
      };
      
      lastOpd = &curOpd;
      
      // *********************
      // break operand loop ()
      if ((state & SP_BREAKLOOP) || (state & SP_BREAKOPD))
        break;   

    }; 
    
    // no sub operands?, should'nt happen, compiler catches emp 
    if (!inSubOpd) {
      state = SP_NOMATCH | SP_BREAKOPD;
    }
  
    // empty loop?
    if (posPtr == loopPosPtr) {
      aOpd.emptyRun = true;
    };

    if (state & SP_RESTORELOOP) {
      restoreState(loopState, aOpd);
      setPos(loopPosPtr);
      if (debug) log("restore last loop");
    };

    if (state & SP_BREAKOPD) {
      // break operand, move position?
      if (state & SP_MOVEON)
        movePos(1, posPtr);
      break;
    } else if (state & SP_BREAKLOOP) {
      // break loop
      if (!(state & SP_MATCH)) {
        restoreState(loopState, aOpd);
        setPos(loopPosPtr);
      }
    };
    
    // move position?
    if (state & SP_MOVEON)
      movePos(1, posPtr);

    first = false;
  };

  // sub operand with last attribute?
  if (aOpd.lastTry) {
    if ((aOpd.lastPos == aOpd.matchEnd) && (!aOpd.lastLogic))
        state = SP_NOMATCH;
    if ((aOpd.lastPos != aOpd.matchEnd) && (aOpd.lastLogic))
        state = SP_NOMATCH;
  }
 
  if (state & SP_MATCH) {
    // positive match
    addPosMatch(aOpd.matchBegin, aOpd.matchEnd, aOpd);
  } else if (aOpd.attrs[SP_ATTR_NOT]) {
    // negative match
    addNegMatch(opdPosPtr, aOpd);
    ++aOpd.noMatchCount;
  } else {
    // no match
    restoreState(opdState, aOpd);
    setPos(opdPosPtr);
  }
}

// ****************************************************************************
// text operand match logic
// aOpd -operand to match
// ****************************************************************************
isInline void TFScripal::matchText(TFOperand& aOpd) {

  // interpret text or get text from name
  std::string* textPart= &aOpd.text;

  if (aOpd.opdtype == SP_OPD_NAME) {
    textPart = &findName(*textPart);
    if (*textPart == "") {
      // name not found 
      setError(ERR_NO_NAME + ", '" +  aOpd.text + "'"); 
      throw(std::runtime_error(ERR_FATAL));
    }
  }

  if (posPtr == nPos) {
    // if position is nPos no sense to match
    return;
  }

  if (!aOpd.attrs[SP_ATTR_NOCASE]) {
    // case match
    if (text.substr(posPtr, textPart->size()) == *textPart) {
      // positive match
      std::size_t curEnd = posPtr + textPart->size() - 1;
      addPosMatch(posPtr, curEnd, aOpd);
      setLogicPos(curEnd + 1, aOpd);
    } else {
      // negative match
      addNegMatch(posPtr, aOpd);
    }
  } else {
    // ignore case
    if (upperText(text.substr(posPtr, textPart->size())) == *textPart) {
      // positive match
      std::size_t curEnd = posPtr + textPart->size() - 1;
      addPosMatch(posPtr, curEnd, aOpd);
      setLogicPos(curEnd + 1, aOpd);
    } else {
      // negative match
      addNegMatch(posPtr, aOpd);
    } 
  }
 
  return;
}

// ****************************************************************************
// number operand match logic
// aOpd -operand to match
// ****************************************************************************
isInline void TFScripal::matchNumbers(TFOperand& aOpd)  {
  std::size_t curBegin, curEnd;
  
  if (posPtr == nPos) {
    // if position is nPos no sense to match
    return;
  }

  // find start of number
  curBegin = posPtr; 
  curEnd = findNumber(text, posPtr, true, aOpd.attrs);
  
  if (curEnd != nPos) {
    TFNum value = 0;
    textToNum(text.substr(curBegin, curEnd - curBegin + 1), value, aOpd.attrs, config.decimalPoint, config.thousandsSep);
    
    if (inRange(value, aOpd.values[0], aOpd.values[1])) {
      // positive match
      addPosMatch(curBegin, curEnd, aOpd);
      setLogicPos(curEnd + 1, aOpd);
    } else if (aOpd.attrs[SP_ATTR_NOT]) {
      // negative match
      addNegMatch(posPtr, aOpd);
    } else {
      // no match
    }
  }
  return;
}

// ****************************************************************************
// logical "ALL" 
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalAll(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 
    
  // is sequence held?
  if (!aOpd.hasMatch) {
    // order broken, quit operand
    return(SP_NOMATCH | SP_BREAKOPD);
  } else {
    // sequence correct, continue with next operand 
    if (lastOpd) 
      // all conditions fullfilled -> end operand
      return(SP_MATCH | SP_BREAKOPD);
    if (aParentOpd.emptyRun) 
      // empty run -> end operand
      return(SP_NOMATCH | SP_BREAKOPD);
  } 
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "ANY" 
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalAny(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 

  // is sequence held?
  if (!aOpd.hasMatch) { 
    if ((lastOpd) || (aParentOpd.emptyRun)) 
      // no match at all, quit parent operand
      return(SP_NOMATCH | SP_BREAKOPD);
  } else {
    // match, quit parent operand
    return(SP_MATCH | SP_BREAKOPD);
  };
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "EACH"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalEach(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 

  if (!aOpd.hasMatch) { 
    if (aOpd.matchCount > 0) {
      // at least one match -> quit parent operand
      return(SP_MATCH | SP_BREAKOPD);
    } else {
      // no match at all -> quit parent operand
      return(SP_NOMATCH | SP_BREAKOPD);
    } 
  } else {
    ++aOpd.matchCount;
    if ((lastOpd) || (aParentOpd.emptyRun))
      // at least one match -> quit parent operand
      return(SP_MATCH | SP_BREAKOPD);
  }
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "EVERY"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalEvery(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 

  if (!aOpd.hasMatch) { 
    // no match 
    if (lastOpd) {
      // no match in this loop over operands?
      if (aParentOpd.subCount == 0) {
        if (aParentOpd.matchCount > 0) 
          return(SP_MATCH | SP_BREAKOPD);
        else  
          return(SP_NOMATCH | SP_BREAKOPD);
      }
    } 

    if (aParentOpd.emptyRun) {
      if (aParentOpd.matchCount > 0) 
        return(SP_MATCH | SP_BREAKOPD);
      else  
        return(SP_NOMATCH | SP_BREAKOPD);
    }
  } else {
    // single match 
    ++aParentOpd.matchCount;
    ++aParentOpd.subCount;

    if (aParentOpd.emptyRun) 
      return(SP_MATCH | SP_BREAKOPD);
  }

  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "FIND"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalFind(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 

  // is sequence held?
  if (!aOpd.hasMatch) { 
    if ((posPtr == nPos) && (aParentOpd.emptyRun))
      return(SP_NOMATCH | SP_BREAKOPD);
    // retry at next position
    return(SP_NOMATCH | SP_BREAKLOOP | SP_MOVEON);
  } else {
    // match
    if (lastOpd) {  
      // last operand: match succesfull
      return(SP_MATCH | SP_BREAKOPD);
    }
  };

  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "FINDAT"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalFindAt(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 

  // is sequence held?
  if (!aOpd.hasMatch) { 
    if ((posPtr == nPos) && (aParentOpd.emptyRun))
      return(SP_NOMATCH | SP_BREAKOPD);
 
    // no match 
    if (((posPtr - aParentOpd.startPosPtr) > static_cast<std::size_t>(aParentOpd.attrValues[1])) && (!std::isinf(aParentOpd.attrValues[1]))) 
      // end of source, quit
      return(SP_NOMATCH | SP_BREAKOPD);

    // last operand
    movePos(1, posPtr);

    // try next position
    return(SP_NOMATCH | SP_BREAKLOOP | SP_MOVEON);
  } else {
    // match 
    if (((aParentOpd.matchBegin - aParentOpd.startPosPtr) > static_cast<std::size_t>(aParentOpd.attrValues[1])) && (!std::isinf(aParentOpd.attrValues[1]))) 
      // end of source, quit
      return(SP_NOMATCH | SP_BREAKOPD);

    if (lastOpd) 
      // last operand: match succesfull
      return(SP_MATCH | SP_BREAKOPD);
  };
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "BREAK"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalBreak(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 
    
  // is sequence held?
  if (aOpd.hasMatch) {
    if (lastOpd) {
      // all conditions fullfilled -> break
      aParentOpd.breakOpd = true;
      return(SP_NOMATCH | SP_BREAKOPD);
    }  
    if (aParentOpd.emptyRun)
      return(SP_MATCH | SP_BREAKOPD);
  } else {
    // single mismatch cancels the operand
    return(SP_MATCH | SP_BREAKOPD);
  } 
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "REPEAT"
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalRepeat(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back());  // is sequence held?

  if ((!aOpd.hasMatch) || (aParentOpd.emptyRun)) {
    if ( (aParentOpd.matchCount >= static_cast<std::size_t>(aParentOpd.attrValues[0]) && ((aParentOpd.matchCount <=  static_cast<std::size_t>(aParentOpd.attrValues[1]) || std::isinf(aParentOpd.attrValues[1])))) ) {
      // number of matches sufficient or no number range given? -> break scan
      if (!aOpd.hasMatch)
        // restore last successful scan result
        return(SP_MATCH | SP_BREAKOPD | SP_RESTORELOOP);
      else        
        return(SP_MATCH | SP_BREAKOPD);
    } else { 
      // number of matches not sufficient
      if (!aOpd.hasMatch)
        return(SP_NOMATCH | SP_BREAKOPD | SP_RESTORELOOP);
      else        
        return(SP_NOMATCH | SP_BREAKOPD);
    }
  } else {
    if (lastOpd) {   
      ++aParentOpd.matchCount;

      // number of matches is maximum? then break operand
      if (aParentOpd.matchCount == static_cast<std::size_t>(aParentOpd.attrValues[1])) 
        return(SP_MATCH | SP_BREAKOPD);
      else   
        return(SP_MATCH | SP_BREAKLOOP);
    };
  }
  return(SP_CONTINUE);
}

// ****************************************************************************
// logical "number" 
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalIsNumber(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 
  
  // is sequence held?
  if (!aOpd.hasMatch) {
    // order broken, reset position from parent -> quit parent operand
    return(SP_NOMATCH | SP_BREAKOPD);
  } else {
    // order correct, set new parse region to next character 
    if (lastOpd) {
      // all conditions fullfilled, is match a number?
      std::string num = text.substr(aParentOpd.matchBegin, aParentOpd.matchEnd - aParentOpd.matchBegin + 1);
      TFNum value = 0;
      if (textToNum(num, value, aParentOpd.attrs, config.decimalPoint, config.thousandsSep)) {     
        if (inRange(value, aParentOpd.attrValues[0], aParentOpd.attrValues[1]))
          return(SP_MATCH | SP_BREAKOPD);
        else if (std::isnan(aParentOpd.attrValues[0]))  
          return(SP_MATCH | SP_BREAKOPD);
        else 
          return(SP_NOMATCH | SP_BREAKOPD);
      } else {   
        return(SP_NOMATCH | SP_BREAKOPD);
      }
    } else {
      // part condition fullfilled 
      if (aParentOpd.emptyRun) 
        // empty run -> end operand
        return(SP_NOMATCH | SP_BREAKOPD);
    }   
  } 
  return(SP_CONTINUE); 
}
  
// ****************************************************************************
// logical "word" 
// aOpd       - sub operand 
// aParentOpd - parent operand 
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalIsWord(TFOperand& aOpd, TFOperand& aParentOpd) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 
    
  // is sequence held?
  if (!aOpd.hasMatch) {
    // order broken, reset position from parent -> quit parent operand
    return(SP_NOMATCH | SP_BREAKOPD);
  } else {
    // order correct, set new parse region to next character 
    if (lastOpd) {
      // all conditions fullfilled, is match a word?
      std::size_t curEnd = findWord(text, aParentOpd.matchBegin); 
      if ((curEnd) == (posPtr - 1))
        return(SP_MATCH | SP_BREAKOPD);
      else if ((curEnd == text.size() -1) && (posPtr == nPos)) 
        return(SP_MATCH | SP_BREAKOPD);
      else   
        return(SP_NOMATCH | SP_BREAKOPD);
    } else {
      // part condition fullfilled       
      if (aParentOpd.emptyRun) 
        // empty run -> end operand
        return(SP_NOMATCH | SP_BREAKOPD);
    }   
  } 
  return(SP_CONTINUE); 
}
  
// ****************************************************************************
// logical "isUpper" and "isLower" 
// aOpd       - sub operand 
// aParentOpd - parent operand 
// aCase      - case CL_xxx
// return - match condition SP_X
// ****************************************************************************
isInline uint16_t TFScripal::logicalIsCase(TFOperand& aOpd, TFOperand& aParentOpd, uint8_t aCase) {
  // last operand in group?
  bool lastOpd =(&aOpd == &aParentOpd.operands.back()); 
    
  // is sequence held?
  if (!aOpd.hasMatch) {
    // order broken, reset position from parent -> quit parent operand
    return(SP_NOMATCH | SP_BREAKOPD);
  } else {
    // order correct, set new parse region to next character 
    if (lastOpd) {
      // all conditions fullfilled, is match a word?
      bool match = false;
      if (aCase == CL_UPPER)
        match = isUpperText(text.substr(aParentOpd.matchBegin, aParentOpd.matchEnd - aParentOpd.matchBegin + 1));
      else if (aCase == CL_LOWER)  
        match = isLowerText(text.substr(aParentOpd.matchBegin, aParentOpd.matchEnd - aParentOpd.matchBegin + 1));
      if (match)
        return(SP_MATCH | SP_BREAKOPD);
      else   
        return(SP_NOMATCH | SP_BREAKOPD);
    } else {
      // part condition fullfilled -> continue
      if (aParentOpd.emptyRun) 
        // empty run -> end operand
        return(SP_NOMATCH | SP_BREAKOPD);
    }   
  } 
  return(SP_CONTINUE); 
}

}