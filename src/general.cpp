// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "general.hpp"
#include "extern.hpp"
#include "text.hpp"

namespace Scripal {

// is library initialized?
bool initialized = false; 

// **********************************************
// thread specific configs

thread_local TFConfig config;
thread_local std::string configAsJSON;

// current environment's encoding in thread (console or calling language)
thread_local int encoding = config.encoding;

// current environment's encoding in thread
thread_local int logEncoding = config.encoding;

// buffer for logs in string
thread_local std::string logBuffer;

// buffer stripped (too big) ?
thread_local bool logBufferStrip = false;

// last error (UTF-8)
thread_local std::string errorMsg = ERR_NONE;

// last explicit error (UTF-8)
thread_local std::string errorExp = ERR_NONE;

// last error position in source or text
thread_local std::size_t errorPos = nPos;
 
// no. of nested templates
thread_local std::size_t templateLevel = 0;

// relocate interface buffers?
thread_local bool relocateBuffer = false;

// is system Big-Endian
bool isBigEndian = isBigEndianTest();

// **************************************************
// mutex for multi-threaded logging and log channels
#ifdef LINUX 
std::mutex logMutex;
#endif
std::ofstream fileChannel;
int logChannel = LOG_NULL;

// **********************************************
// time measurement
// **********************************************
std::size_t totalTime = 0;
auto sTime =  std::chrono::steady_clock::now();

// **********************************************
// important characters and grammatical elements
// **********************************************

// text/char delimiter
std::string ctrlText;
// infinite sign
std::string ctrlInfinite;
// range delimiter
std::string ctrlRangeBegin;
std::string ctrlRangeEnd;
// block delimiter
std::string ctrlBlockBegin;
std::string ctrlBlockEnd;
// name delimiter
std::string ctrlName;
// semicolon
std::string ctrlSemicolon;
// new line
std::string ctrlNL;
// carriage return
std::string ctrlCR;
// template delimiters
std::string ctrlTempBegin;
std::string ctrlTempEnd;
// template
std::string ctrlConfig;
// operator parameters
std::array<TFOprParms, _SP_OPR_END> oprParms;
// syntax endpoints
std::vector<char32_t> ctrlEndPoints;

// **********************************************
// determine operand type 
// **********************************************
bool logicOpd(std::size_t aCodeID)   {if ((aCodeID >=_SP_LOGIC_BEGIN) && (aCodeID <=_SP_LOGIC_END)) return(true); else return(false);};
bool blockOpd(std::size_t aCodeID)   {if ((aCodeID >=_SP_BLOCK_BEGIN) && (aCodeID <=_SP_BLOCK_END)) return(true); else return(false);};
bool textOpd(std::size_t aCodeID)    {if (aCodeID == SP_OPD_TEXT) return(true); else return(false);};
bool nameOpd(std::size_t aCodeID)    {if (aCodeID == SP_OPD_NAME) return(true); else return(false);};
bool numberOpd(std::size_t aCodeID)  {if (aCodeID == SP_OPD_NUMBER) return(true); else return(false);};
bool rangeOpd(std::size_t aCodeID)   {if (aCodeID == SP_OPD_RANGE) return(true); else return(false);};

// ****************************************************************************
// reset time counter (total)
// ****************************************************************************
void resetTime() {
  totalTime = 0;
};

// ****************************************************************************
// record time
// return - current microsecond time
// ****************************************************************************
std::chrono::time_point<std::chrono::steady_clock> recTime() {
    return(std::chrono::steady_clock::now());
};

// ****************************************************************************
// print time that has passed since aSTime and add to total time
// aSTime - last recorded time 
// return - time difference in microseconds
// ****************************************************************************
std::size_t printTime(std::chrono::time_point<std::chrono::steady_clock> aSTime) {
    auto endTime =  std::chrono::steady_clock::now();
    auto processMicro =  std::chrono::duration_cast<std::chrono::microseconds>(endTime - aSTime).count();
    auto processTime =  processMicro;
    totalTime += (std::size_t)processTime;
    std::cout << "time(us): " << processTime << "\n";
    return((std::size_t)processTime);
};

// ****************************************************************************
// stop time that has passed since aSTime and add to total time
// aSTime - last recorded time 
// return - time difference in microseconds
// ****************************************************************************
std::size_t stopTime(std::chrono::time_point<std::chrono::steady_clock> aSTime) {
    auto endTime =  std::chrono::steady_clock::now();
    auto processMicro =  std::chrono::duration_cast<std::chrono::microseconds>(endTime - aSTime).count();
    auto processTime =  processMicro;
    totalTime += (std::size_t)processTime;
    return((std::size_t)processTime);
};

// ****************************************************************************
// detect endianes of system running
// return - true if big-endian system
// ****************************************************************************
bool isBigEndianTest() {
  int num = 1;
  if(*(char *)&num == 1)
    return(false);
  else  
    return(true);
}

// ****************************************************************************
// swap bytes of value
// aVal - value to convert
// ****************************************************************************
void swapBytes(char32_t &aVal) {
  unsigned char *ptr = (unsigned char *)&aVal;
  aVal = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
};

// ****************************************************************************
// get home directory
// return - directory path 
// ****************************************************************************
std::string homeDir() {
  #ifdef LINUX
   // store config file in ~/.config/scripal/scripal.cnf under Linux
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  return(std::string(homedir));
  #endif 
  #ifdef WINDOWS
  return(".");
  #endif
}


// ****************************************************************************
// set error with no position specification
// aError    - error text
// ****************************************************************************
void setError(const std::string& aError) {
  errorMsg = aError;
  errorExp = aError;
  errorPos = nPos; 

  log("error: " + errorExp);
};

// ****************************************************************************
// set error in meaningful English with source fragments (explicit)
// aError    - error message
// aErrorPos - error position in source 
// aSource   - source
// ****************************************************************************
void setError(const std::string &aError, std::size_t aErrorPos, const std::string &aSource) {
  errorMsg = aError;
  errorPos = aErrorPos; 

  errorExp = aError + " '";
  std::size_t index  = aErrorPos;
 
  if (aErrorPos == nPos) {
    if (aSource.size() > 20)
      index = aSource.size() - 20;
    else   
      index = 0;
    errorExp += aSource.substr(index, 20) + " <<<-\n"; 
  } else {
    if (aErrorPos > 20)
      index = aErrorPos - 20;
    else 
       index = 0;  
    errorExp += aSource.substr(index, aErrorPos - index) + "->>>" + aSource.substr(aErrorPos, 30) + "...'\n"; 
  };

  log("error: " + errorExp);
  return;
}

// ****************************************************************************
// clear last error
// ****************************************************************************
void clearError() {
  errorMsg = ERR_NONE;
  errorExp = ERR_NONE;
  errorPos = nPos;  
}

// ****************************************************************************
// output a UTF-8! message in console (variable encoding determines character encoding)
// aMessage - message to log
// ****************************************************************************
void output(const std::string &aMessage) {
  std::cout << utf8ToEnc(aMessage, encoding);
}

// ****************************************************************************
// output a UTF-8! message in console with given encoding 
// aMessage - message to log
// ****************************************************************************
void output(const std::string &aMessage, int aEncoding) {
  std::cout << utf8ToEnc(aMessage, aEncoding);
}

// ****************************************************************************
// log a UTF-8! message to log device
// aMessage - message to log
// aPos1    - position if involved or nVal
// aPos2    - position if involved or nVal
// ****************************************************************************
void log(const std::string &aMessage, std::size_t aPos1, std::size_t aPos2) {
  if (logChannel == LOG_NULL)
    return;
  
  std::string message;
  
  message = aMessage;
  if ((aPos1 != nVal) && (aPos2 != nVal)) {
    message += " at(" + printPosition(aPos1) + ", " + printPosition(aPos2) + ")";
  } else if ((aPos1 != nVal)) {
    message += " at(" + printPosition(aPos1) + ")";
  } 
 
  if (logEncoding != ENC_UTF8)
    message = utf8ToEnc(message + "\n", logEncoding);
  else 
    message += "\n";
  
  switch (logChannel) {
    case LOG_STDOUT:
      #ifdef LINUX
      logMutex.lock();
      #endif
      #ifdef WINDOWS
      CRITICAL_SECTION CriticalSection;
      InitializeCriticalSection(&CriticalSection);
      #endif

      std::cout << message;
 
      #ifdef LINUX
      logMutex.unlock();
      #endif
      #ifdef WINDOWS
      DeleteCriticalSection(&CriticalSection);
      #endif 
      break;
    case LOG_BUFFER:
      if (logBuffer.size() > 100000000) {
        logBuffer.erase(0, logBuffer.find(utf8ToEnc("\n", logEncoding)));
        if (!logBufferStrip) {
          logBufferStrip = true;
          logBuffer = utf8ToEnc("...\n", logEncoding) + logBuffer;
        }
      }
         
      logBuffer += message;
      break;
    case LOG_FILE:
      fileChannel << message;
      break;
    default:  
      break;
  };
}


// ****************************************************************************
// print out compiled code 
// aCode  - code
// return - code in human readable format
// ****************************************************************************
std::string printCode(std::vector<TFOperator> &aCode) {
  std::string result = "";
  for (size_t i = 0; i < aCode.size(); ++i) {
    result += "[" + std::to_string(i) + "] ";
    result += printOpr(aCode[i]);
    result += separateChar;
    result += " \n";
  };  
  return(result);
}

// ****************************************************************************
// find identifier for given grammar type and code ID
// aType   - atomic type
// aCodeID - code ID
// return  - identifier
// ****************************************************************************
const std::string& findIdentifier(uint16_t aType, uint16_t aCodeID) {
  for (const TFIndentifier & item : identifiers) {
    if ((item.grammarType == aType) && (item.codeID == aCodeID))
      return(item.identifier);
  };   

  return(nullString); 
}

// ****************************************************************************
// find identifier for given grammar type and identifier
// aType - grammar type
// aIdentifier  - string indentifier
// return  - index
// ****************************************************************************
int findIdentifier(uint16_t aType,  const std::string &aIdentifier) {
  for (int index = 0; index < identifiers.size(); ++ index ) {
    if ((identifiers[index].grammarType == aType) && (identifiers[index].identifier == aIdentifier))
      return(index);
  };   

  return(-1); 
}


// ****************************************************************************
// print operator (and it's operands)
// aOpr    - operator
// return  - operator in human readable format
// ****************************************************************************
std::string printOpr(TFOperator& aOpr) {
  std::string result;
  
  // condition 
  if (aOpr.condition != SP_COND_ANYMATCH) {
    result = findIdentifier(SP_GRAMMAR_COND, aOpr.condition);
    result += "[" + std::to_string(aOpr.jumpTo) + "] ";
  };
  
  // operator name 
  result += findIdentifier(SP_GRAMMAR_OPR, aOpr.codeID);
 
  if (aOpr.codeID == SP_OPR_LOOP) {
    result += "[" + std::to_string(aOpr.jumpTo) + "] ";
  }
  
 // operand(s) 
  std::string opd = printOpd(aOpr.operand);
  if (opd != "")
    result += " " + opd;

  return(result);
};

// ****************************************************************************
// print number or special value as string (infinte, NaN)
// aNumber - number to print
// aAttrs  - attributes for number type
// return  - number in human readable format
// ****************************************************************************
std::string printNumber(TFNum aNumber, bool aAttrs[_SP_ATTR_END]) {
  std::string result;
  if (aNumber == INFINTE_VALUE) {
    result = ctrlInfinite;
  } else if (std::isnan(aNumber)) {
    result = "NaN";
  } else {
    numToText(aNumber, result, aAttrs);
  }
  return(result);  
};
   
// ****************************************************************************
// print position in text, either number or special value as string (nPos))
// aPos    - position to print
// return  - position in human readable format
// ****************************************************************************
std::string printPosition(std::size_t aPos) {
  if (aPos == nPos)
    return("nPos");
  else if (aPos == nVal)
    return("NaN");
  else if (aPos == static_cast<std::size_t>(aPos))
    return(std::to_string( static_cast<std::size_t>(aPos)));
  else    
    return(std::to_string(aPos));
};

// ****************************************************************************
// print integer position used in low level interfaces in text
// aPos    - position to print
// return  - position in human readable format
// ****************************************************************************
std::string printPosition(int aPos) {
  if (aPos == noPos)
    return("nPos");
  else if (aPos == noVal)
    return("NaN");
  else    
    return(std::to_string(aPos));
};

// ****************************************************************************
// print operand
// aOpd    - operand to print
// return  - operand in human readable format 
// ****************************************************************************
std::string printOpd(TFOperand& aOpd) {
  std::string result = "";
  
  switch (aOpd.opdtype) {
    case SP_OPD_NULL:
      break;
    case SP_OPD_TEXT:
      result += printAllAttrs(aOpd);
      result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_TEXT) +  aOpd.text + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_TEXT);
      break;
    case SP_OPD_NAME:
      result += printAllAttrs(aOpd);
      result = findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_NAME) +  aOpd.text + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_NAME);
      break;
    case SP_OPD_NUMBER:
      result += printAllAttrs(aOpd);
      result += printNumber(aOpd.values[0], aOpd.attrs);
      break;
    case SP_OPD_RANGE:
      result += printAllAttrs(aOpd);
      if ((!std::isnan(aOpd.values[0])) && (!std::isnan(aOpd.values[1]))) {
        result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_BEGIN) +  printNumber(aOpd.values[0], aOpd.attrs);
        result += " " + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_COMMA) + " " + printNumber(aOpd.values[1], aOpd.attrs) + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_END);
      }; break;
    case SP_OPD_LOGICAL: {
      bool first = true;
      result += printAllAttrs(aOpd);
      result += findIdentifier(SP_GRAMMAR_OPD, aOpd.codeID);

      if (!(std::isnan(aOpd.attrValues[0])) && !(std::isnan(aOpd.attrValues[1]))) {
        result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_BEGIN) +  printNumber(aOpd.attrValues[0], aOpd.attrs);
        result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_COMMA) + " " + printNumber(aOpd.attrValues[1], aOpd.attrs) + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_END);
      };
      result += " " + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_GROUP_BEGIN);
      for (auto curOpd : aOpd.operands) {
        if (!first)
          result += " ";
        result += printOpd(curOpd);
        first = false;
      }
      result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_GROUP_END);
      
      break; };
    case SP_OPD_BLOCK:
      result += printAllAttrs(aOpd);
      result += findIdentifier(SP_GRAMMAR_OPD, aOpd.codeID);
      if (!(std::isnan(aOpd.attrValues[0])) && !(std::isnan(aOpd.attrValues[1]))) {
        result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_BEGIN) +  printNumber(aOpd.attrValues[0], aOpd.attrs);
        result += findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_COMMA) + " " + printNumber(aOpd.attrValues[1], aOpd.attrs) + findIdentifier(SP_GRAMMAR_CTRL, SP_CTRL_RANGE_END);
      };
      break;
 };
    
  return(result);
};

// ****************************************************************************
// print operand attribute if set
// aOpd    - operand
// aAttr   - attribute to print
// return  - attribute in human readable format 
// ****************************************************************************
std::string printAttr(TFOperand &aOpd, std::size_t aAttr) {
  std::string result = "";
  if (aOpd.attrs[aAttr]) {        
    switch (aAttr) {
      case SP_ATTR_NOCASE:    
      case SP_ATTR_NOT:    
        result += findIdentifier(SP_GRAMMAR_ATTR, aAttr);
        break;
      default:  
        result += findIdentifier(SP_GRAMMAR_ATTR, aAttr) + " ";
        break;
    };
  };  
  return(result);
}

// ****************************************************************************
// print all operand attributes 
// aOpd   -  operand
// return  - attributes set in human readable format 
// ****************************************************************************
std::string printAllAttrs(TFOperand &aOpd) {
  std::string result = "";

  for (std::size_t i = 0; i < _SP_ATTR_END ; ++i) {
    result += printAttr(aOpd, i);
  }

  return(result);
}

// ****************************************************************************
// convert Scripal position to int number interpretable by most languages
// aPos - position, may be nPos or NaN
// ****************************************************************************
int posToInt(std::size_t aPos) {
  if (aPos == nPos)
    // nPos
    return(noPos);
  else if (aPos == nVal)
    // NaN
    return(noVal);
  else if (aPos >= (std::size_t)(0x10000000000000))
    // position too large for int representation
    return(noPos);
  else 
    return((int)aPos);
}

// ****************************************************************************
// is number in given range (including endpoints)? 
// aValue - value to check
// aMin, aMax - range
// return - true if in range 
// ****************************************************************************
bool inRange(TFNum aValue, TFNum aMin, TFNum aMax) {
  if (aMin == INFINTE_VALUE)
    return false;
  if ((std::fabs(aValue - aMin) >= epsilon) && (aMax == INFINTE_VALUE))
    return true;
  if ((aValue >= aMin - epsilon) && (aValue <= aMax + epsilon))
    return true;
  return false;
}

// ****************************************************************************
// print all bytes of string as hex values for debug purposes 
// aString - string to print
// return - string with hex values "hh hh.." , example "fa 1b ...." 
// ****************************************************************************
std::string printBytesHex(const std::string &aString) {
  std::string result, digits;
  for (std::size_t i = 0; i < aString.size(); ++i) {
    TFNum val = (TFNum)(unsigned char)(aString[i]);
    numHexToText(val, digits);
    if (i > 0)
      result += " ";
    result += digits;
  }
  return(result);
}

// ****************************************************************************
// get internal encoding ID for given characetr encoding "utf-try to detect file encoding 
int getEncID(std::string &aEncoding) {
  std::string encodingStr = lowerText(aEncoding);
  if (encNames.find(encodingStr) != encNames.end()) 
    return(encNames[encodingStr]);
  else   
    return(-1);
}

// ****************************************************************************
// try to detect file encoding 
// if encoding is at start of path enc//, return given encoding
// aPath - path to file, take out encoding if given
// return - encoding given or found, else default encoding
// ****************************************************************************
int getFileEncoding(std::string &aPath) {
  std::string curEncoding;
  int result = ENC_DEFAULT;
  trimText(aPath);
  
  // get encoding from path
  auto pos = aPath.find("//");
  if ((pos != nPos) && (pos > 0) && (pos < 10)) {
    // path given
    curEncoding = lowerText(aPath.substr(0, pos));
    if (encNames.find(curEncoding) != encNames.end()) 
      result = encNames[curEncoding];
    aPath.erase(0, pos + 2);  
    return(result); 
  }

  // try to read BOM
  TFAccess fileAccess;
  fileAccess.assignFile(aPath);
  if (errorMsg != ERR_NONE) 
    return(ENC_DEFAULT);
  result = getBOMEncoding(fileAccess, 0);

  // no clue? use environment's encoding  
  if (result == ENC_DEFAULT) 
    result = encoding;
    
  return(result);
}

// ****************************************************************************
// show Scripal result in human readable format, encode as desired 
// aResults  - result object
// aEncoding - encoding of HRF format
// return    - result  
// ****************************************************************************
std::string anyResultsToHRF(TFResults &aResults, int aEncoding) {
  std::string result;
  for (std::size_t index = 0; index < aResults.size(); ++ index) {
     if (aResults.fileNames[index] != "")
        result += "File: " + aResults.fileNames[index] + ",";

    result += "[" + printPosition(aResults.positions[index][0]) + ", " + printPosition(aResults.positions[index][1]) + "]" ;
    result += " \"" + encToUtf8(aResults.text[index], aResults.encoding) + "\"";
    if (aResults.matchType == MATCH_SOURCE) {
      if (aResults.tags[index] != "")
        result += " tag:\"" +  encToUtf8(aResults.tags[index], aResults.encoding) + "\"";
    } else {
      std::string floatNum;
      numFloatToText(aResults.ratings[index], floatNum, floatPrecision);
      result += ", \"" + floatNum + "\"";
    }    
    result += "\n";
  }
  return(utf8ToEnc(result, aEncoding));
}  

// ****************************************************************************
// show Scripal result in CSV style, encode as desired 
// aResults  - result object
// aEncoding - encoding of CSV text
// return    - result  
// ****************************************************************************
std::string anyResultsToCSV(TFResults &aResults, int aEncoding) {
  std::string result;

  for (std::size_t index = 0; index < aResults.size(); ++ index) {
     if (aResults.fileNames[index] != "")
        result += "File: " + aResults.fileNames[index] + ",";
    result += printPosition(aResults.positions[index][0]) ;
    result += "," + printPosition(aResults.positions[index][1]) ;
    result += "," + encToUtf8(aResults.text[index], aResults.encoding);
    if (aResults.matchType == MATCH_SOURCE) {
      result += ",\"" +  encToUtf8(aResults.tags[index], aResults.encoding) + "\"";
    } else { 
      std::string floatNum;
      numFloatToText(aResults.ratings[index], floatNum, floatPrecision);
      result += "," +  floatNum;
    }
    result += "\n";
  }  
  return(utf8ToEnc(result, aEncoding));
}  

// ****************************************************************************
// show Scripal result in JSON style, encode as desired 
// aResults  - result object
// aEncoding - encoding of JSON
// return    - JSON result  
// ****************************************************************************
std::string anyResultsToJSON(TFResults &aResults, int aEncoding) {
  Poco::JSON::Object::Ptr cJSON = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
  Poco::JSON::Array::Ptr  array = new Poco::JSON::Array();
  
  for (std::size_t index = 0; index < aResults.size(); ++ index) {
    Poco::JSON::Object::Ptr inner = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
    if (aResults.fileNames[index] != "")
      inner->set("file",  aResults.fileNames[index]);
    inner->set("posBegin",  posToInt(aResults.positions[index][0]));
    inner->set("posEnd",  posToInt(aResults.positions[index][1]));
    inner->set("result",encToUtf8(aResults.text[index], aResults.encoding));
    if (aResults.matchType == MATCH_SOURCE) {
      inner->set("tag", encToUtf8(aResults.tags[index], aResults.encoding));
    } else {   
      std::string floatNum;
      numFloatToText(aResults.ratings[index], floatNum, floatPrecision);
      inner->set("distance", floatNum);
    }
    array->add(inner);
  }
  cJSON->set("results", array);
 
  std::stringstream ss;
  cJSON->stringify(ss, 2, 5);
  return(utf8ToEnc(ss.str(), aEncoding));

}

// ****************************************************************************
// show Scripal examples in JSON style, encode as desired 
// aExamples - Scripal examples, only convert short text examples < 10 kB
// aEncoding - encoding of JSON
// return    - result  
// ****************************************************************************
std::string examplesToJSON(const std::vector<TFExmpl> aExamples, int aEncoding) {
  Poco::JSON::Object::Ptr cJSON = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
  Poco::JSON::Array::Ptr  array = new Poco::JSON::Array();
  const std::string nameId = "task:";
  const std::string descriptionId = "how:";

  for (std::size_t index = 0; index < aExamples.size(); ++ index) {
    if (aExamples[index].text.size() > 10000)
      continue;

    Poco::JSON::Object::Ptr inner = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
    std::string explain = aExamples[index].explain;

    // set counter
    inner->set("counter",  index);

    // set name
    std::string part = aExamples[index].explain.substr(0, aExamples[index].explain.find(descriptionId));
    part = part.substr(part.find(nameId) + nameId.size(), nPos);
    inner->set("name",  trimText(part));

    // set description
    part = aExamples[index].explain.substr(aExamples[index].explain.find(descriptionId), nPos);
    part = part.substr(part.find(descriptionId) + descriptionId.size(), nPos);
    inner->set("description",  trimText(part));

    // set source
    inner->set("source", aExamples[index].source);

    // set text
    inner->set("text", aExamples[index].text);
    array->add(inner);
  }
  cJSON->set("examples", array);
 
  std::stringstream ss;
  cJSON->stringify(ss, 2, 5);
 
  return(utf8ToEnc(ss.str(), aEncoding));
}

// ****************************************************************************
// encode results of UTF8 object
// aOrgText   - original text in foreign encoding
// aUTF8Text  - text in UTF8
// aObj       - Scripal object 
// aEncoding  - text encoding  
// ****************************************************************************
void encodeResults(const std::string &aOrgText, const std::string &aUTF8Text, TFScripal& aObj, int aEncoding) {
  std::string convert;
  char32_t codePoint = 0;
  std::size_t posUtf8 = 0, lenUtf8, lenEnc = 0, posEnc = 0, arrIndex = 0, charCount = 0;
  bool start = true;
  
  if (aEncoding != ENC_UTF8) {
    for (std::size_t index = 0; index < aObj.results.size(); ++index) {
      aObj.results.text[index] = utf8ToEnc(aObj.results.text[index], aEncoding);
      if (aObj.results.positionType == POS_UTF8) {
        aObj.results.positions[index][0] = aObj.results.positions[index][1] = nVal;
      }
    };
  } 

  aObj.results.encoding = aEncoding;

  if (aObj.results.positionType == POS_UTF8) 
    return;

  while (posUtf8 < aUTF8Text.size()) {
    // convert
    lenUtf8 = getCP(aUTF8Text, posUtf8, codePoint);
    lenEnc = getCP(aOrgText, posEnc, codePoint, aEncoding);
    if (lenEnc == 0)
      convert = "";

    if (arrIndex < aObj.results.size()) {
      // check array bounds
      if ((posUtf8 == aObj.results.positions[arrIndex][0]) && start) {
        if  (aObj.results.positionType == POS_OFFSET)
          aObj.results.positions[arrIndex][0] = posEnc;
        else  
          aObj.results.positions[arrIndex][0] = charCount;
        start = false;
      }
      if (((posUtf8 +  lenUtf8 - 1) == aObj.results.positions[arrIndex][1]) && (!start)) {
        if  (aObj.results.positionType == POS_OFFSET)
          aObj.results.positions[arrIndex][1] = posEnc + lenEnc -1 ;
        else  
          aObj.results.positions[arrIndex][1] = charCount;
        ++arrIndex;
        start = true;
      }
    };

    posUtf8 += lenUtf8;
    posEnc += lenEnc;
    ++charCount;
  }
/* 
  // ouput result
  for (arrIndex = 0; arrIndex < aObj.results.size(); ++arrIndex) {
    std::size_t start = aObj.results.positions[arrIndex][0];
    std::size_t end = aObj.results.positions[arrIndex][1];
    std::cout << std::to_string(start) << " , " << std::to_string(end) << "\n";
    std::cout << encToUtf8(aOrgText.substr(start, end - start + 1), aEncoding) << "\n"; 
  }
*/ 
}

// ****************************************************************************
// if file is not definitely raw text, try to decode to UTF-8 content 
// aFile  - path to file 
// return - new file name of temporay file if decoded, else original
//          if not interpretable return "NULL", remove file after usage!
// ****************************************************************************
std::string decodeFile(const std::string &aFile) {
  // file extensions seen as raw text
  std::vector<std::string> textFiles = {\
    "txt", "rtf", "ini", "cfg", \
    "c", "h", "cpp", "hpp", "cs", "java", "js", "class", "php", "sh", "cgi", "pl", "py", "vb", "swift", \
    "rs", "go", "kt", "r", "rb", "ts", "matlab", \
    "json", "css", \
     "xml" \
  };
  std::map<std::string, std::string> pandocFiles = { \
    {"bib", "bibtex"}, {"bst", "biblatex"}, {"jats", "bits"}, \
    {"csv", "csv"}, { "dbk", "docbook"}, { "docx", "docx"} , { "md", "markdown"} , { "odt", "odt"} \
  };


  std::string content;
 
  if (!(std::filesystem::is_regular_file( std::filesystem::status(aFile)) ))
    return("NULL");
 
  std::string extension = lowerText(std::filesystem::path(aFile).extension().string());
  if (extension[0] == '.')
    extension.erase(0,1);
    
  if (std::find(textFiles.begin(), textFiles.end(), extension) != textFiles.end())  {  
    // file considered raw text
    return(aFile);
  };

  if (extension == "pdf") {
    if (callSystem(config.pdfReader, aFile + " " + tmpFile))
      return(tmpFile);
  };
  
  if (pandocFiles.find(extension) != pandocFiles.end()) {
    if (callSystem(config.frmReader, " " + aFile + " -f " + pandocFiles[extension] + " -t plain -s -o " + tmpFile))
      return(tmpFile);
  };      
  return("NULL");
}
   
// ****************************************************************************
// call command shell with given command
// aCommand    - command
// aParameters - parameters 
// return - true if successfull, may set error
// ****************************************************************************
bool callSystem(const std::string &aCommand, const std::string &aParameters) {
  std::string cmd = aCommand +  " " + aParameters;  
  int cmdResult =std::system(cmd.c_str());
  
  switch (cmdResult) {
    case 0:
     return(true);  
    case ENOENT:
      setError(ERR_SYSTEM + ", command " + aCommand + " is missing, please install");
      return(false);  
      break;
    default:
      setError(ERR_SYSTEM + ", shell not found");
      return(false);  
      break;
  }
  return(false);  
}  

// ****************************************************************************
// print all files in path(s) specified
// aPath         - path to search in
// aDirExtension - file types to search for in directories (default: "*")
// aRecursive    - if true, search all subdirectories
// ****************************************************************************
void printFiles(const std::string &aPath, const std::string &aDirExtension, bool aRecursive) {
  TFFileIterate files(aPath, aDirExtension, aRecursive);
  
  while (files.error == false) {
    files.iterate();
    if (!files.error)
      output(files.entry->path().string() + "\n");
  }
}

// ****************************************************************************
// read main config from file given
// aPath - path to file, if empty use default
// aEncoding  - file encoding 
// ****************************************************************************
void initConfig(const std::string aPath, int aEncoding) {
  std::string json, path;
  bool resetConf = false;

  aEncoding = getBestEncoding(aEncoding);
 
  path = aPath;
  if (lowerText(aPath) == "reset")
    resetConf = true;

  if ((aPath == "") || (resetConf)){
    // use default config
    #ifdef LINUX
    // test for /usr/lib or /usr/lib64
    if (!std::filesystem::exists("/usr/lib/scripal")) {
      if (std::filesystem::exists("/usr/lib64/scripal"))
        config.appPath = "/usr/lib64/scripal";
    }

    path = homeDir() + "/.config/scripal";
    if ((!std::filesystem::exists(path))) {
      try {
        std::string callExt = "mkdir " + path;
        bool res = std::system(callExt.c_str());
      } catch (const std::exception &aExc) {
        setError(ERR_FILE +", file " + path + " create directory or config file");
        return;
      }  
    }
    path += "/scripal.cnf";
    #endif

    #ifdef WINDOWS
    // store config file in ./scripal.cnf
    path = ".\\scripal.cnf";
    #endif

  }
 
  // create config if it does not exit and parameter is reset 
  if (!std::filesystem::exists(path) || resetConf) {
    if (!config.store(path)) {
      setError(ERR_FILE +", config file " + path + " not created, no permission?");
      return;
    } 
  }
  
  if (std::filesystem::exists(path)) {
    // read config from file only if it exists
    if (!readFromFile(path, json)) {
      setError(ERR_FILE +", file " + path + " not found or no permission");
      return;  
    }

    json = encToUtf8(json, aEncoding);   
    config.fromJSON(json);
  }

}

}  

