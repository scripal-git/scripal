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

// null object
TFScripal nullScripal;

TFScripal::TFScripal() {};

// ****************************************************************************
// shallow Copy constructor!
// ****************************************************************************
TFScripal::TFScripal(TFScripal& aObj) {
  if (aObj.matchType == MATCH_SOURCE) {
    *this = TFScripal(aObj.source, ENC_UTF8);
  } else {
    *this = TFScripal(aObj.source, aObj.patternDist, aObj.matchType, ENC_UTF8);
  }
}

// ****************************************************************************
// initialize object for source match
// aSource   - source string
// aEncoding - source encoding 
// ****************************************************************************
void TFScripal::initialize(const std::string &aSource, int aEncoding) {

  if (!initialized)
    initLibrary("");
    
  if (aSource == "") {
    setError(ERR_SOURCE + ", empty source"); 
    return;
  };  
  // source match 
  matchType = MATCH_SOURCE;
  jsonConfig = "";

  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;
  
  if (aEncoding != ENC_UTF8) { 
    source = encToUtf8((std::string_view)aSource, aEncoding);
  } else {
    source = aSource;
  }
  
  TFCompile compileObj(static_cast<std::string>(source), *this);

  compileObj.run();
}

// ****************************************************************************
// create object for pattern matching (nearest or block match)
// aSource    - compare string 
// aDist      - nearest: max. no. of matches or 
//              block: critical distance for result [0..1]: 1 perfect, 0 no similarity
// aMatchType - match algorithm
// aEncoding  - source encoding
// ****************************************************************************
TFScripal::TFScripal(const std::string &aSource, double aDist, int aMatchType, int aEncoding) {
  if (!initialized)
    initLibrary("");

  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

  if (aEncoding != ENC_UTF8) {
    source = encToUtf8((std::string_view)aSource, aEncoding);
  } else {
    source = aSource;
  }

  // nearest match
  matchType = aMatchType;
  patternDist = aDist;
}
  
// ****************************************************************************
// create object and compile source (source match)
// aSource    - source string
// aEncoding  - source encoding
// ****************************************************************************
TFScripal::TFScripal(const std::string &aSource, int aEncoding) {
  initialize(aSource, aEncoding);
  matchType = MATCH_SOURCE;
}
  
// ****************************************************************************
// reset object for new match
// ****************************************************************************
void TFScripal::reset() {
  hasMatched = false; 
  processTime = 0;
  totalTime = 0;
  results.clear();                       
  names.clear();

  hasMatch = false;                                  
  runElse = false;

  nameOn = false;                                   
  nameAdd = false;                                   

  posPtr = 0;                                    
  resultBegin = nPos;                                
  resultEnd = nPos;                                  
  curName = "";
  curTag  = "";
  instPtr = 0;
  lastInstPtr = MAX_SIZE_T;
  notCount = 0;
  lastMatch = nullptr;
 
}
 
// ****************************************************************************
// match object against text
// aText     - string 
// aEncoding - text encoding
// return - true if at least one result occured
// ****************************************************************************
bool TFScripal::match(const std::string &aText, int aEncoding) {
  std::string curText;
  bool result;
  
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

  if (aEncoding != ENC_UTF8) {
    curText = encToUtf8((std::string_view)aText, aEncoding);
    text.assignString(&curText);
  } else {
    text.assignString(&aText);
  };
 
  if (matchType == MATCH_NEAREST)
    // find according to nearest match
    result = findNearest(source, text, results, patternDist);
  else if (matchType == MATCH_BLOCK)
    // compare blocks
    result = compareBlock(source, text, results, patternDist);    
  else   
    // find according to Scripal source
    result = process();

  if (aEncoding != ENC_UTF8)  
    encodeResults(aText, curText, *this, aEncoding);

  return(result);
}
 
// ****************************************************************************
// match object against file
// aFile         - path to file 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
//                 set aFileEncoding to encoding detected
// return - true if at least one result occured
// ****************************************************************************
bool TFScripal::matchFile(const std::string &aFile, int aFileEncoding) {
  bool result;
  int curEncoding;
  std::string fileContent, textSource;

  clearError();
  text.assignFile(aFile);
  
  if (errorMsg != ERR_NONE) 
    return(false);

  if (aFileEncoding == ENC_DEFAULT) { 
    // find out encoding 
    curEncoding = (int)getBOMEncoding(text, 0);
    
    // if no BOM -> use environments default ecoding
    if (curEncoding == ENC_DEFAULT)
      curEncoding = encoding;
    aFileEncoding = curEncoding;
  };

  // file too big?
  if ((int)text.size() > config.maxFileSize * 1000000) {
    setError(ERR_FILE + ", file too big to scan '" + aFile + "'"); 
    return(false);
  };      

  if (aFileEncoding != ENC_UTF8) {
    // read file entirely to convert to UTF-8
    fileContent = text.read();
    textSource = encToUtf8((std::string_view)fileContent, aFileEncoding);
 
    text.assignString(&textSource);
  }

  if (matchType == MATCH_NEAREST)
    // find according to nearest match
    result = findNearest(source, text, results, patternDist);
  else if (matchType == MATCH_BLOCK)
    // compare blocks
    result = compareBlock(source, text, results, patternDist);
  else    
    // find according to Scripal source
    result = process();
  
  if (aFileEncoding != ENC_UTF8)  
    encodeResults(fileContent, source, *this, aFileEncoding);
  
  return(result);
}

// ****************************************************************************
// match object against text and replace occurences with given string
// single result gives modified text 
// aText     - string 
// aReplace  - string to replace with
// aEncoding - text encoding
// return    - true if at least one result occured
// ****************************************************************************
bool TFScripal::replace(const std::string &aText, const std::string &aReplace, int aEncoding) {
  std::string curText;
  std::string resText;
  bool result;
 
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;
 
  if (aEncoding != ENC_UTF8) {
    curText = encToUtf8((std::string_view)aText, aEncoding);
    text.assignString(&curText);
  } else {
    text.assignString(&aText);
  };

  if (matchType == MATCH_NEAREST)
    // find according to nearest match
    result = findNearest(source, text, results, patternDist);
  else   
    // find according to Scripal source
    result = process();

  if (!result)
    return(false);
 
  // make last result become text to modify 
  results.push_back("", aText, {nPos, nPos}, 0, "");

  int offset = 0;
  int diff = 0;

  for (std::size_t i = 0; i < results.size() - 1; ++i) {
    diff = (int)(aReplace.size() - results.text[i].size());
    if ((results.text[i].size() != 0) || ((results.text[i].size() == 0) && (config.useEmpty)))
      results.text.back().replace((int)results.positions[i][0] + offset, results.text[i].size(), aReplace);
    offset += diff;
  }
  if (results.size() > 1)
    results.erase(0, results.size() - 1);

  return(true);
}

// ****************************************************************************
// match object against file and replace occurences with given string 
// aFileIn       - input file 
// aFileOut      - output file (must be different from input file)
//                 if empty append "_" to aFileIn  (aFileIn_)
// aReplace      - string to replace with 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
//                 set aFileEncoding to encoding detected
// return        - true on success
// ****************************************************************************
bool TFScripal::replaceFile(const std::string &aFileIn,const std::string &aFileOut, const std::string &aReplace, int aFileEncoding) {
  std::string fileOut = aFileOut;
  
  if (trimText(lowerText(aFileIn)) == trimText(lowerText(aFileOut))) {
    setError(ERR_FILE + ", in- and out file must differ"); 
    return(false);
  };

  if (fileOut == "")
    fileOut = aFileIn + "_";
  
  if (!matchFile(aFileIn, aFileEncoding)) {
    return(false);
  }  
  
  // replacement must be same encoding as file, detected in matchFile()
  std::string replace;
  if (results.encoding == ENC_UTF8)  
    replace = aReplace;
  else 
    replace = utf8ToEnc(aReplace, results.encoding);
  
  std::ifstream inFile(aFileIn, std::ios_base::binary);
  std::ofstream outFile(fileOut, std::ios_base::binary);
  
  std::size_t offsetIn = 0;
  std::size_t index = 0;

  if (!inFile.is_open()) {
    setError(ERR_FILE + ", cannot open '" + aFileIn + "'"); 
    return(false);
  }

  if (!outFile.is_open()) {
    setError(ERR_FILE + ", cannot open '" + aFileOut + "'"); 
    return(false);
  }
  
  char charIn;
  inFile.get(charIn);

  while (!inFile.eof()) {
    // read string sizes
    bool posMatch = false;
    if (index < results.positions.size()) 
      if (offsetIn == results.positions[index][0]) 
        posMatch = true;

    if (posMatch) {
      outFile.write(replace.c_str(), replace.size());
      // set to end of search region
      offsetIn += ( results.positions[index][1] - results.positions[index][0] + 1 );   
      inFile.seekg(offsetIn);
      ++index;
    } else { 
      offsetIn += 1;
      outFile.put(charIn);
    }
    inFile.get(charIn);
  };      

  return(true);
}

// ****************************************************************************
// match object against text and split text, results will hold parts
// aText  - string 
// aEncoding - text encoding
// return - true if at least one result occured
// ****************************************************************************
bool TFScripal::split(const std::string &aText, int aEncoding) {
  std::string curText;
  std::string curResult;
  std::vector<std::string> newResults;
 
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

  if (aEncoding != ENC_UTF8) {
    curText = encToUtf8((std::string_view)aText, aEncoding);
    text.assignString(&curText);
  } else {
    text.assignString(&aText);
  };

  bool result = process();

  if (!result) {
    results.clear();
    return(result);  
  }
  
  int offset = 0;
  int len = 0;

  for (std::size_t i = 0; i < results.size(); ++i) {
    len = (int)results.positions[i][0] - offset;
    curResult = aText.substr(offset, len);
    offset = (int)(results.positions[i][0] + results.text[i].size());
    if ((curResult.size() > 0) || ((curResult.size() == 0) && (config.useEmpty)))
      newResults.push_back(curResult);
  }
  if ((offset < (int)aText.size()) || ((offset >= (int)aText.size()) && (config.useEmpty)))
    newResults.push_back(aText.substr(offset, nPos) );

  results.clear();
  for (auto value: newResults) {
    results.push_back("", value, {nPos,nPos}, 0, "");
  }  
   
  return(result);
}

// ****************************************************************************
// match object against file and split file, results will hold parts
// aFileIn       - input file 
// aFileOut      - output file (must be different from input file)
//                 if empty append "_" to aFileIn  (aFileIn_)
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return        - true if at least one result occured
// ****************************************************************************
bool TFScripal::splitFile(const std::string &aFileIn, const std::string &aFileOut, int aFileEncoding) {
  if (trimText(lowerText(aFileIn)) == trimText(lowerText(aFileOut))) {
    setError(ERR_FILE + ", in- and out file must differ"); 
  };
  
  if (!matchFile(aFileIn, aFileEncoding))   
   return(false);
 
  std::string fileOut = aFileOut;
  if (fileOut == "")
    fileOut = aFileIn + "_";

  std::ifstream inFile(aFileIn, std::ios_base::binary);
  std::ofstream outFile(fileOut, std::ios_base::binary);
  
  std::size_t offsetIn = 0;
  std::size_t index = 0;

  if (!inFile.is_open()) {
    setError(ERR_FILE + ", cannot open '" + aFileIn + "'"); 
    return(false);
  }

  if (!outFile.is_open()) {
    setError(ERR_FILE + ", cannot open '" + fileOut + "'"); 
    return(false);
  }
  
  bool charWrite = false;
  char charIn;
  inFile.get(charIn);

  while (!inFile.eof()) {
    bool posMatch = false;
    if (index < results.positions.size()) 
      if (offsetIn == results.positions[index][0]) 
        posMatch = true;

    if (posMatch) {
      if (charWrite) {    
        #ifdef LINUX
          outFile.write(ctrlNL.c_str(), ctrlNL.size());
        #endif  
        #ifdef WINDOWS
          outFile.write(ctrlCR.c_str(), ctrlCR.size());
          outFile.write(ctrlNL.c_str(), ctrlNL.size());
        #endif  
      };  
      // set to end of search region
      offsetIn = results.positions[index][0] + results.text[index].size();    
      inFile.seekg(offsetIn);
      ++index;
    } else { 
      offsetIn += 1;
      outFile.put(charIn);
    }
 
    charWrite = true;
    inFile.get(charIn);
  };      

  return(true);
}

// ****************************************************************************
// show object's result in human readable format, encode as desired 
// aEncoding - encoding of result
// return    - result  
// ****************************************************************************
std::string TFScripal::resultsToHRF(int aEncoding) {
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

  return(anyResultsToHRF(results, aEncoding));
};

// ****************************************************************************
// show object's result in CSV format, encode as desired 
// aEncoding - encoding of result
// return    - result  
// ****************************************************************************
std::string TFScripal::resultsToCSV(int aEncoding) {
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

  return(anyResultsToCSV(results, aEncoding));
};

// ****************************************************************************
// show object's result in JSON format, encode as desired 
// aEncoding - encoding of result
// return    - result  
// ****************************************************************************
std::string TFScripal::resultsToJSON(int aEncoding) {
  if (aEncoding == ENC_DEFAULT) 
    aEncoding = ENC_UTF8;

    return(anyResultsToJSON(results, aEncoding));
}; 

// ****************************************************************************
// call code processor
// return - true if at least one result occured 
// ****************************************************************************
bool TFScripal::process() {
  std::size_t lastPosPtr = 0;
  std::chrono::steady_clock::time_point startTime;
  resetTime();
  if (code.size() == 0) {
    setError(ERR_INVALID_CODE); 
    return(false);
  };  

  if (jsonConfig != "") {
    if (!config.setTemp(jsonConfig)) {
      return(false);
    }
  };
  
  reset();
  clearError();
  
  results.init(MATCH_SOURCE, config.positionType);

  if (config.debugRun)
    debug = true;  

  if (debug) log("----- start run -----");
  
  if (config.measureTime) 
    startTime =  std::chrono::steady_clock::now();

  // loop over operators 
  try { 
  
    while (instPtr < code.size()) {
      #ifdef DEBUG_SCRIPAL
      std::cout << "position pointer: " << posPtr << "\n";
      #endif
 
      if (debug) log("\n-> operator[" + std::to_string(instPtr) + "]: " + printOpr(code[instPtr]) + " , match(" + (hasMatch ? "true" : "false") +")", posPtr);

      // check for operator condition
      if (!checkCondition(code[instPtr].condition)) 
        continue;
      
      // state to remember?
      if ((lastInstPtr < instPtr) || (lastInstPtr == MAX_SIZE_T)) {
        for (auto item = code[instPtr].states.begin(); item != code[instPtr].states.end(); item++) {
          code[item->first].count = 0;
          saveState(item->second, dummyOperand);   
        }
      };
      
      switch (code[instPtr].codeID) {
        case SP_OPR_MATCH:                // match 
          parse();
          incInstr();
          break;

        case SP_OPR_MATCHEND:             // match and end
          parse();
          if (hasMatch) 
            endResult(resultEnd);
          incInstr();
          break;

        case SP_OPR_RESET:                // reset result
          resetResult();
          incInstr();
          break;

        case SP_OPR_END:                  // end result, store
          endResult(resultEnd);
          incInstr();
          break;

        case SP_OPR_ENDLAST:              // end result from last match or at eot
          if (posPtr <= text.size() - 1) 
            endResult(lastMatch->matchBegin - 1);
          else  
            endResult(text.size() - 1);
          incInstr();
          break;

        case SP_OPR_REPLACE:             // end result, replace last result with new one
          if ((resultBegin != nPos) || (resultEnd != nPos)) 
            clearResult();
          
          endResult(resultEnd);
          incInstr();
          break;

        case SP_OPR_TAG:                  // tag
          setTag();
          incInstr();
          break;

        case SP_OPR_NAME:                 // make result to name
          setName();
          incInstr();
          break;

        case SP_OPR_NAMEADD:              // also name result 
          setNameAdd();
          incInstr();
          break;

        case SP_OPR_SETMATCH:             // set conditon match
          setMatch(SP_MATCH);
          incInstr();
          break;

        case SP_OPR_SETNOMATCH :          // set conditon nomatch
          setMatch(SP_NOMATCH);
          incInstr();
          break;

        case SP_OPR_LOOP:                 // loop until no changes occur
          if ((posPtr != lastPosPtr) && (posPtr != nPos)){
            jump(code[instPtr].jumpTo, false);
            lastPosPtr = posPtr;
          } else {  
            // no changes in loop body -> break  
            invalidInstr(); 
            break;
          }
          break;

        case SP_OPR_TOTAL:             // repeated matches
          multiParse(lastPosPtr);
          break;

        case SP_OPR_MOVEON:              // continue matching at next character
          if (posPtr != nPos) {
            if (numberOpd(code[instPtr].operand.codeID))
              movePos(code[instPtr].operand.values[0], posPtr);
            else  
              movePos(1, posPtr);
          }   
          incInstr();
          break;

        case SP_OPR_EXIT:                // exit
          invalidInstr();
          break;

        case SP_OPR_NULL:                // null operator
          incInstr();
          break;
      }
    }

    // if open match region or position -> store
    endResult(resultEnd);

  } catch (std::exception const& aExc) {
    if (std::strcmp(aExc.what(), ERR_FATAL.c_str()) != 0)   
      setError(aExc.what());
  }
  
  
  if (config.measureTime) {
    auto endTime =  std::chrono::steady_clock::now();
    auto processMilli =  std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    auto processMicro =  std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    processTime =  processMilli + (((long double)processMicro / 1000) - processMilli);
  }

  //std::cout <<  "totalTime: " << totalTime << "\n";

  // temporary config?
  if (jsonConfig != "") {
    // temporary config used -> restore original config 
    config.resetTemp();
  }

  if (errorMsg != ERR_NONE) {
    return(false);
  } else {  
    if (debug) log("\nend run: " + std::to_string(results.size()) + " results\n");
    return (hasMatched);
  }
}

// ****************************************************************************
// jump to given instruction
// aInstr   - index of new instruction
// aRunElse - if true, run else case if process continues with else... 
// ****************************************************************************
isInline void TFScripal::jump(std::size_t aInstr, bool aRunElse) {
  if (debug) log("jump to [" + std::to_string(aInstr) + "] " + findIdentifier(SP_GRAMMAR_OPR, code[code[aInstr].jumpTo].codeID) + "...");
  lastInstPtr = instPtr;
  instPtr = aInstr;
  runElse = aRunElse;
}

// ****************************************************************************
// increment instruction pointer
// ****************************************************************************
isInline void TFScripal::incInstr() {
  lastInstPtr = instPtr;
  ++instPtr;
}

// ****************************************************************************
// set invalid instruction pointer
// ****************************************************************************
isInline void TFScripal::invalidInstr() {
  lastInstPtr = instPtr;
  instPtr = MAX_SIZE_T;
}

// ****************************************************************************
// set position pointer, set nPos if invalid and end of Input
// aPos - new position in text
// ****************************************************************************
isInline void TFScripal::setPos(std::size_t aPos) {
  if (aPos >= text.size()) {
    posPtr = nPos;
  } else {
    posPtr = aPos;
  }
}

// ****************************************************************************
// set position pointer on positive match, set nPos if invalid and end of Input
// aPos - new position in text
// aOpd - matching operand
// ****************************************************************************
isInline void TFScripal::setLogicPos(std::size_t aPos, TFOperand &aOpd) {
  if (aOpd.attrs[SP_ATTR_NOT]) 
    return;
    
  setPos(aPos);  
}

// ****************************************************************************
// move position pointer ahead, might end with nPos at start/end of text
// aCount - pos/neg number of code points to move in text
// aPos   - position pointer to be moved
// ****************************************************************************
isInline void TFScripal::movePos(TFNum aCount, std::size_t &aPos) {
  if (aPos == nPos)
    return;

  for (TFNum pos = 0; pos < aCount; ++pos) {
    aPos = getNextCP(text, aPos);
    if (aPos == nPos) {
      break;
    } else {
    }
  };
}

// ****************************************************************************
// check operator condition, true if fullfilled
// set instruction pointer according to position
// aCondition - condition to check
// return - true if fullfilled
// ****************************************************************************
isInline bool TFScripal::checkCondition(uint16_t aCondition) {
  switch (aCondition) {
    case SP_COND_IFMATCH:
      if (!hasMatch) { 
        jump(code[instPtr].jumpTo,true);
        return(false);  
      } else {
        return(true);
      } 

    case SP_COND_IFNOMATCH:
      if (hasMatch) {
        jump(code[instPtr].jumpTo, true);
        return(false);
      } else {
        return(true);
      } 
      break;

    case SP_COND_IFMATCHED:
      if (!hasMatched) { 
        jump(code[instPtr].jumpTo,true);
        return(false);  
      } else {
        return(true);
      } 

    case SP_COND_IFNOMATCHED:
      if (hasMatched) { 
        jump(code[instPtr].jumpTo,true);
        return(false);  
      } else {
        return(true);
      } 

    case SP_COND_ELSE:
      if (!runElse) {
        jump(code[instPtr].jumpTo, false);
        return(false);
      } else {
        runElse = false;
        return(true);
      }
      break;

    case SP_COND_ANYMATCH:
      return(true);  

    }
  
  return(false);
}

// ****************************************************************************
// set debug message for a match of given operand
// aOpd - operand that matched 
// aType - positive (true) or negative match
// ****************************************************************************
isInline void TFScripal::debugMatch(TFOperand &aOpd, bool aType) {
  std::string msg = "";
 
  if (aType)
    msg += "pos. match at " + printOpd(aOpd);
  else
    msg += "neg. match at " + printOpd(aOpd);
  
  msg += ", pos:(" + printPosition(posPtr) + "), range:(" + printPosition(aOpd.matchBegin)  + "," + printPosition(aOpd.matchEnd) + "), result: (" + printPosition(resultBegin) + "," + printPosition(resultEnd) + ")";;

  if ((aOpd.matchBegin != nPos) && (aOpd.matchEnd != nPos)) {
    std::size_t len = aOpd.matchEnd - aOpd.matchBegin + 1;
    if (len > 50)
      msg += ", '" + text.substr(aOpd.matchBegin, 50) + "....'";
    else 
      msg += ", '" + text.substr(aOpd.matchBegin, len) + "'";
  } else if (aOpd.matchBegin != nPos) {
    msg += ", text: '>>> " + text.substr(aOpd.matchBegin, 20) + "'";
  } else if (aOpd.matchEnd != nPos) {
    std::size_t offset = aOpd.matchEnd;
    if (offset >= 20)
      offset -= 20;
    else 
      offset = 0;
    msg += ", text: '<<< " + text.substr(offset, 20) + "'";
  }  

  if (debug) log(msg);
}

// ****************************************************************************
// match operand against text
// ****************************************************************************
isInline void TFScripal::parse() {
  TFOperand& curOpd = code[instPtr].operand;  

  // reset not level counter
  notCount = 0;
 
  // clear unless positions
  unlessPos.clear();

  // evaluate operand
  matchOpd(curOpd, nullOperand);

  // is operand match type?
  if (curOpd.hasMatch) {
    // operand matched
    hasMatch   = true;    
    hasMatched = true;
  } else {
    // operand did not match
    hasMatch = false;
  };  

}

// ****************************************************************************
// repeated matches of operators/operands against text
// aLastPosPtr - last position pointer 
// ****************************************************************************
isInline void TFScripal::multiParse(std::size_t &aLastPosPtr) {
  if (hasMatch) {
    ++code[instPtr].count;

    if (code[instPtr].count >= code[instPtr].operand.values[0]) {
      // loop done! reset counter
      code[instPtr].count = 0;
      incInstr();
    } else {
      if (posPtr != aLastPosPtr) {
        // next loop
        jump(code[instPtr].jumpTo, false);
      } else {
        // no changes in loop body -> break  
        invalidInstr(); 
        hasMatch = false;
      }  
    }
  };

  if (!hasMatch) {
    // no match -> failed try, restore state
    restoreState(code[code[instPtr].jumpTo].states[instPtr], dummyOperand);   
    code[instPtr].count = 0;
    incInstr();
  }
 
  aLastPosPtr = posPtr;
}

// ****************************************************************************
// find operand string quickly in text and set position pointer to start
// may throw exception
// aOpd -operand to match
// aStartPtr, aEndPtr - region in text where to look, 
// if aEndPtr == nPos -> search to end
// return - true if positioning worked
// ****************************************************************************
isInline bool TFScripal::posToText(TFOperand& aOpd, std::size_t aStartPtr, std::size_t aEndPtr) {
  std::string* textPart= &aOpd.text;

  if (aOpd.attrs[SP_ATTR_NOT]) 
    return(false);

  if (aEndPtr == nPos)
    aEndPtr = text.size() - 1;

  if (aOpd.opdtype == SP_OPD_NAME) {
    textPart = &findName(*textPart);
    if (*textPart == "") {
      // name not found 
      setError(ERR_NO_NAME + ", '" +  aOpd.text + "'"); 
      throw(std::runtime_error(ERR_FATAL));
    }
  }
    
  if (!aOpd.attrs[SP_ATTR_NOCASE]) {
    // case match
    std::size_t foundPtr = findStr(text, *textPart, aStartPtr);
    if ((foundPtr != nPos) && (foundPtr <= aEndPtr)) {
      setPos(foundPtr);
      return(true);
    }
  } else {
    // ignore case
    std::size_t foundPtr = findStrCase(text, *textPart, aStartPtr);
    if ((foundPtr != nPos) && (foundPtr <= aEndPtr)) {
      setPos(foundPtr);
      return(true);
    }
  }
  return(false);
}


// ****************************************************************************
// set match/nomatch result
// aResult : SP_MATCH / SP_NOMATCH
// ****************************************************************************
isInline void TFScripal::setMatch(uint16_t aResult) {
  if (aResult == SP_MATCH) {
    if (debug) log("set match true");
    hasMatch = true;
  } else { 
    if (debug) log("set match false");
    hasMatch = false;
  }
}

// ****************************************************************************
// store result, string and positions
// aBegin, aEnd  - result region
// aResult       - result text
// aTag          - result tag
// ****************************************************************************
void TFScripal::storeResult(std::size_t aBegin, std::size_t aEnd, const std::string& aResult, const std::string& aTag) {
  // prevent storing same result again
  if (results.size() > 0) 
    if (((results.positions.back()[0] == aBegin) && (results.positions.back()[1] == aEnd)))   
      return;

  results.push_back("", aResult, {aBegin, aEnd}, 0, aTag);
}  

// ****************************************************************************
// clear last result
// ****************************************************************************
void TFScripal::clearResult() {
  if (results.positions.size() > 0) 
    results.pop_back();
}  

// ****************************************************************************
// add a new negative (no) match (if SP_ATTR_NOT is set!)
// NOT match only stores positions
// aPos   - position where match occured
// aOpd   - operand involved
// ****************************************************************************
isInline void TFScripal::addNegMatch(std::size_t aPos, TFOperand &aOpd) {
  // don't set match if NOT is not set
  if (!aOpd.attrs[SP_ATTR_NOT])
    return;
  
  // set operand to match
  aOpd.hasMatch  = true;
  ++aOpd.matchCount;

  // a negative match produces only a virtual result [begin, nPos] or [nPos, end]
  if (aPos != nPos) {
    if ((resultBegin == nPos) && (aPos < resultEnd)) {
      // start match
      resultBegin = aPos;
      aOpd.matchBegin = resultBegin;
      if (debug) debugMatch(aOpd, true); 
    } else if (resultBegin != nPos) {  
      // expand match up to but not including position
      if ((aPos > resultEnd) || (resultEnd == nPos)) {
        if (aPos > 0)
          resultEnd = aPos - 1;
        else   
          resultEnd = 0;
        aOpd.matchEnd = resultEnd;
        if (debug) debugMatch(aOpd, true); 
      }
    }
  } 
  
  lastMatch = &aOpd;
 
}

// ****************************************************************************
// add a new conditionless positive match without region 
// aOpd   - operand involved
// ****************************************************************************
isInline void TFScripal::addPosMatch(TFOperand &aOpd) {
  // set operand to match
  aOpd.hasMatch  = true;
  ++aOpd.matchCount;

  aOpd.matchBegin = nPos;
  aOpd.matchEnd = nPos;

  lastMatch = &aOpd;
  
  if (debug) debugMatch(aOpd,true);  
}

// ****************************************************************************
// add a new positive match (if SP_ATTR_NOT is not set!)
// if begin is nPos and end ist specified, no result yet, the end marks the new begining
// a (nPos,nPos) is only a logical match without region for logic control
// aBegin - new start of region,  if nPos, only end of match region is specified 
// aEnd   - new end of region, if nPos, only begin of match region is specified 
// aOpd   - operand involved
// ****************************************************************************
isInline void TFScripal::addPosMatch(std::size_t aBegin, std::size_t aEnd, TFOperand &aOpd) {
  bool change = false;

  // don't set match on NOT
  if (aOpd.attrs[SP_ATTR_NOT]) 
    return;
 
  // set operand to match
  aOpd.hasMatch  = true;
  ++aOpd.matchCount;
 
  // if embedded in NOT attributes, only count match, not region or position
  if (notCount > 0) 
    return;

  if (aBegin != nPos) {
    // aBegin != nPos
    if (resultBegin != nPos) {
      // aBegin & resultBegin != nPos -> earlier result begin from after attribute
      if (aBegin < resultBegin) {
        resultBegin = aBegin; 
        change = true;
      }
    } else {
      // aBegin != nPos and resultBegin == nPos -> set new result begin
      resultBegin = aBegin; 
      change = true; 
    }
  };
  
  if (aEnd != nPos) {
    // aEnd != nPos
    if ((aEnd > resultEnd) || (resultEnd == nPos)) {
      resultEnd = aEnd;  
      change = true;
    }  
  };

  aOpd.matchBegin = aBegin;
  aOpd.matchEnd = aEnd;
  if (change && debug) debugMatch(aOpd, true); 

  lastMatch = &aOpd;

}

// ****************************************************************************
// prettify result (verbose mode)
// aPos - start of result
// aLen - result length
// return - modified result 
// ****************************************************************************
std::string TFScripal::prettyResult(std::size_t aPos, std::size_t aLen) {
  std::string result = "";
  const std::size_t offset = 20;
  std::size_t curBegin, curLen = 0;
  if (aPos > offset) {
    curBegin = aPos - offset;
    curLen = offset;
  } else {
    curBegin = 0;
    curLen = aPos;
  }  
  if (curLen > 0)
    result += text.substr(curBegin, curLen);

  result += "-->" + text.substr(aPos, aLen) + "<--";

  if ((aPos + aLen - 1) < text.size())
    result += text.substr(aPos + aLen, offset);
  printCtrl(result);
  return(result);
}
  
// ****************************************************************************
// start new result region
// aPos - start of result
// ****************************************************************************
isInline void TFScripal::beginResult(std::size_t aPos) {
  if (debug) log("begin result ", aPos);
  resultBegin = aPos; 
  resultEnd = nPos;
}

// ****************************************************************************
// end result region
// aPos - end of result
// ****************************************************************************
isInline void TFScripal::endResult(std::size_t aPos) {
  if (aPos < text.size()) 
    resultEnd = aPos;

  // set result if any match found
  if ((resultBegin != nPos) && (resultEnd != nPos)) {
    // result region
    if (nameOn) {
      std::pair<std::string, std::string> curElm = {curName, text.substr(resultBegin, resultEnd - resultBegin + 1)};
      // name only    
      names.push_back(curElm);
      nameOn = nameAdd = false;
    } 
    if (!nameOn || nameAdd) {
      // result
      if (debug) log("store result", resultBegin, resultEnd);
      if (config.verboseResult)
        storeResult(resultBegin, resultEnd, prettyResult(resultBegin, resultEnd - resultBegin + 1), curTag);
      else  
        storeResult(resultBegin, resultEnd, text.substr(resultBegin, resultEnd - resultBegin + 1), curTag);
    }
  } else {
    // virtual result positions
    if (!nameOn || nameAdd) 
      if ((resultBegin != nPos) || (resultEnd != nPos))
        storeResult(resultBegin, resultEnd, "", curTag);
  }
  resetResult();
}

// ****************************************************************************
// reset result 
// ****************************************************************************
isInline void TFScripal::resetResult() {
  if (debug) log("reset result");
  resultBegin = resultEnd = nPos;
  nameOn = false;
  nameAdd = false;
  curName = curTag = "";
}

// ****************************************************************************
// does object have a result? 
// return - true in case of result
// ****************************************************************************
bool TFScripal::isResult() {
  if (results.size() != 0)
    return(true);
  else  
    return(false);
}

// ****************************************************************************
// is object source match?
// ****************************************************************************
bool TFScripal::isSource() {
  return(matchType == MATCH_SOURCE);
}

// ****************************************************************************
// is object nearest match?
// ****************************************************************************
bool TFScripal::isNearest() {
  return(matchType == MATCH_NEAREST);
}

// ****************************************************************************
// is object block match?
// ****************************************************************************
bool TFScripal::isBlock() {
  return(matchType == MATCH_BLOCK);
}

// ****************************************************************************
// save current match/result state of operand
// aState - stored state
// aOpd   - operand 
// ****************************************************************************
void TFScripal::saveState(TFState &aState, TFOperand &aOpd) {
  aState.matchBegin = aOpd.matchBegin;
  aState.matchEnd = aOpd.matchEnd;
  aState.resultBegin = resultBegin;
  aState.resultEnd = resultEnd;
  aState.resultSize = results.size();
}

// ****************************************************************************
// restore current match/result state of operand
// aState - stored state
// aOpd   - operand 
// ****************************************************************************
void TFScripal::restoreState(TFState &aState, TFOperand &aOpd) {
  aOpd.matchBegin = aState.matchBegin;
  aOpd.matchEnd = aState.matchEnd;
  resultBegin = aState.resultBegin;
  resultEnd = aState.resultEnd;
  if (aState.resultSize < results.size()) 
    results.resize(aState.resultSize);
}

// ****************************************************************************
//  set name
// ****************************************************************************
void TFScripal::setName() {
  nameOn = true;
  nameAdd = false;
  curName = code[instPtr].operand.text;
  if (debug) log("set name " + curName);
}

// ****************************************************************************
// set name additionally
// ****************************************************************************
void TFScripal::setNameAdd() {
  setName();
  nameAdd = true;
}

// ****************************************************************************
// set tag
// ****************************************************************************
isInline void TFScripal::setTag() {
  curTag = code[instPtr].operand.text;
  if (debug) log("set tag " + curTag);
}

// ****************************************************************************
// find result for given name
// aName - name to find
// return - name or nullString if not found
// ****************************************************************************
std::string& TFScripal::findName(const std::string& aName) {
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (names[i].first == aName)
      return(names[i].second);
  };
  return(nullString);
}

// ****************************************************************************
// search for files where object matches, set global results!
// aPath         - path to search in
// aDirExtension - file types to search for in directories (default: "*")
// aRecursive    - if true, search all subdirectories
// return        - success (at least one result)
// ****************************************************************************
bool TFScripal::fileSearch(const std::string &aPath, const std::string &aDirExtension, bool aRecursive) {
  TFFileIterate files(aPath, aDirExtension, aRecursive);
  int curEncoding = ENC_DEFAULT;

  if (files.error) {
    setError(ERR_FILE +", path " + aPath + " not found or no permission");
    return(false);
  };  

  TFScripal obj = *this;
 
  results.init(matchType, config.positionType);
  obj.results.init(matchType, config.positionType);

  while (files.error == 0) {
    files.iterate();
    if (files.error) { 
      if (results.size() > 0) {
        return(true);
      } else {
        return(false);
      }  
    };
    std::string fileName = files.entry->path().string();
     // if not text based file -> try to decode

    std::string tmpFileName = decodeFile(fileName);
    if (tmpFileName == "NULL") {
      continue; 
    }

    curEncoding = ENC_DEFAULT;
    if (obj.matchFile(tmpFileName, curEncoding)) {
      for (std::size_t index = 0; index < obj.results.size(); ++index) 
        obj.results.fileNames[index]  = fileName;
      
      results.matchType = obj.results.matchType;

      // insert results sorted
      std::size_t index = 0;
      while (index < results.size()) {
        auto cmp = compareText(fileName, results.fileNames[index]);
        if ((cmp == CMP_GTR) || (cmp == CMP_EQU)) {
          ++index;  
        } else {
          break;
        }
      }
      results.insert(index, obj.results);
    }
    
  };

  if (results.size() > 0) {
    return(true);
  } else {
    return(false);
  }  
}


}
