// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "interface.hpp"
#include "general.hpp"
#include "scripalLib.hpp"
#include "text.hpp"
#include "encoding.hpp"
#include "TFAccess.hpp"
#include "TFCompile.hpp"
#include "TFTemplate.hpp"
#include "TFScripal.hpp"

// current new object ID
thread_local int curId = 1;

// pool of Scripal objects
thread_local std::map<int, std::unique_ptr<TFManagedObj>> objects; 

// memory holding internal objects
TFManagedMem iMemory;  

// ****************************************************************************
// // main initialization
// ****************************************************************************
void baseInit() {
  initBase();
}

// ****************************************************************************
// main finalization
// ****************************************************************************
void baseFinalize() {
  finalizeBase();
}

// ****************************************************************************
// intialize library in thread
// aConfigPath, aConfigPathLen - path to config file, if empty use default
// aEncoding                   - encoding used
// ****************************************************************************
void libInit(const char* aConfigPath, int aConfigPathLen, int aEncoding) {
  initLibrary(static_cast<std::string>(std::string_view(aConfigPath,  aConfigPathLen)), aEncoding);
}

// ****************************************************************************
// intialize library in thread
// aConfigPath, aConfigPathLen - path to config file, if empty use default
// aEncoding                   - encoding used
// ****************************************************************************
void libInitC(const char* aConfigPath, int aConfigPathLen, int aEncoding) {
  libInit(aConfigPath, aConfigPathLen, aEncoding);
  relocateBuffer = true;
}

// ****************************************************************************
// finalize library in thread
// ****************************************************************************
void libFinalize() {
  finalizeLibrary();
}

// ****************************************************************************
// intialize log system
// aLogChannel, aLogChannelLen  - log channel ("stdout", "buffer" or path to file)
// ****************************************************************************
void logInit(const char*  aLogChannel, int  aLogChannelLen) {
  initLog(static_cast<std::string>(std::string_view(aLogChannel, aLogChannelLen)));
}

// ****************************************************************************
// get log buffer
// aLogBuf, aLogBufLen  - contents of log buffer
// ****************************************************************************
void getLogBuf(const char** aLogBuf, int* aLogBufLen) {
  iMemory.addBuffer(utf8ToEnc(*getLogBuffer(), encoding), aLogBuf, aLogBufLen);
}

// ****************************************************************************
// clear log buffer
// ****************************************************************************
void clearLogBuf() {
  getLogBuffer()->clear();
}

// ****************************************************************************
// print text on stdout
// aText, aTextLen - text in foreign encoding
// aEncoding - aText's encoding
// ****************************************************************************
void printText(const char* aText, int aTextLen, int aEncoding) {
  std::string encText;

  try {
    encText = encToUtf8(std::string_view(aText, aTextLen), aEncoding);
    output("'" + encText + "': '" + printBytesHex(encText) + "'\n");
  } catch(const std::exception &aExc) {
    output("error\n");
  }
}

// ****************************************************************************
// delete char buffer owned by Scripal
// aPtr - buffer to free
// ****************************************************************************
void deletePtr(const char* aPtr) {
  iMemory.clearBuffer(aPtr);
}

// ****************************************************************************
// get last error message
// aError, aErrorLen - error (empty if none)
// ****************************************************************************
void getErrMsg(const char** aError, int* aErrorLen) {
  iMemory.addBuffer(utf8ToEnc(*getErrorMsg(), encoding), aError, aErrorLen);
}

// ****************************************************************************
// get last explicit error
// aError, aErrorLen - error (empty if none)
// ****************************************************************************
void getErrExp(const char** aError, int* aErrorLen) {
  iMemory.addBuffer(utf8ToEnc(*getErrorExp(), encoding), aError, aErrorLen);
}

// ****************************************************************************
// get last error byte! position in original encoded text
// return - byte position
// ****************************************************************************
int getErrPos() {
  return(posToInt(getErrorPos()));
}

// ****************************************************************************
// clear last error 
// ****************************************************************************
void clearErr() {
  clearError();
}

// ****************************************************************************
// get library version
// aVersion, aVersionLen - version
// ****************************************************************************
void getVersion(const char** aVersion, int* aVersionLen) {
  iMemory.addBuffer(utf8ToEnc(VERSION, encoding), aVersion, aVersionLen);
}

// ****************************************************************************
// set encoding of interface for current thread
// aEncoding - encoding ENC_xxx, set ENC_DEFAULT to use locale's encoding
// ****************************************************************************
void setEncoding(int aEncoding) {
  encoding = getBestEncoding(aEncoding);
}

// ****************************************************************************
// set new object ID to use (curID is changed)
// ****************************************************************************
void getNewID() {
  while(true) {
    if (objects.find(curId) == objects.end())
      break;
    ++curId;
 
   // since we're using signed integers, spare out negatives
    if (curId < 0)
      curId = 0;   
  }
  return;  
}

// ****************************************************************************
// create new Scripal object from code, if error occurs no object is created
//   and object ID is -1
// aSource, aSourceLen - source
// return object ID or -1, may set error
// ****************************************************************************
int newCodeObj(const char* aSource, int aSourceLen) {
  std::unique_ptr<TFManagedObj> manObj(new TFManagedObj);
  std::string orgError;
  std::string source;

  if (aSource == nullptr) {
    setError(ERR_SOURCE + ", no source given"); 
    return(-1);
  }  

  clearError();
  manObj->obj.reset(new TFScripal(static_cast<std::string>(std::string_view(aSource, aSourceLen)), encoding));
  
  // error occured?
  if (errorMsg != ERR_NONE) 
    return(-1);

  // get new object ID
  getNewID();
 
  objects.insert(std::make_pair(curId, std::move(manObj)));
  return(curId);
}

// ****************************************************************************
// create new Scripal object for nearest string match
// aSource, aSourceLen - source to look for
// aMax    - maximum distance (1.0 for perfect match) for result
// return object ID or -1, may set error
// ****************************************************************************
int newNearestObj(const char* aSource, int aSourceLen, double aMax) {
  std::unique_ptr<TFManagedObj> manObj(new TFManagedObj); 

  if (aSource == nullptr) {
    setError(ERR_SOURCE + ", no source given");
    return(-1);
  }  

  clearError();

  manObj->obj.reset(new TFScripal(encToUtf8(std::string_view(aSource, aSourceLen), encoding), aMax, MATCH_NEAREST));
  
  // error occured?
  if (errorMsg != ERR_NONE) 
    return(-1);
  
  // get new object ID
  getNewID();
 
  objects.insert(std::make_pair(curId, std::move(manObj)));
  return(curId);
}

// ****************************************************************************
// create new Scripal object for block string match
// aSource, aSourceLen - source to look for
// aMax    - maximum distance (1.0 for perfect match) for result
// return object ID or -1, may set error
// ****************************************************************************
int newBlockObj(const char* aSource, int aSourceLen, double aMax) {
  std::unique_ptr<TFManagedObj> manObj(new TFManagedObj); 

  if (aSource == nullptr) {
    setError(ERR_SOURCE + ", no source given");
    return(-1);
  }  

  clearError();

  manObj->obj.reset(new TFScripal(encToUtf8(std::string_view(aSource, aSourceLen), encoding), aMax, MATCH_BLOCK));
  
  // error occured?
  if (errorMsg != ERR_NONE) 
    return(-1);
  
  // get new object ID
  getNewID();
 
  objects.insert(std::make_pair(curId, std::move(manObj)));
  return(curId);
}

// ****************************************************************************
// delete Scripal object
// aObjID - object ID
// ****************************************************************************
void deleteObj(int aObjID) {
  if (objects.find(curId) == objects.end())
    return; 
    
  objects.erase(aObjID); 
}

// ****************************************************************************
// match object against text
// aObjID          - Scripal object ID
// aText,aTextLen  - text 
// return - true if at least one result occured
// ****************************************************************************
bool match(int aObjID, const char* aText, int aTextLen) {
  std::string utf8Text;
  bool result;

  try {
    result = objects.at(aObjID)->obj->match(static_cast<std::string>(std::string_view(aText, aTextLen)), encoding);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
}

// ****************************************************************************
// match object against file
// aObjID          - Scripal object ID
// aFile,aFileLen  - name of file
// aFileEncoding   - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool matchFile(int aObjID, const char* aFile, int aFileLen, int aFileEncoding) {
  std::string utf8File;
  bool result;

  try {
    utf8File = encToUtf8(std::string_view(aFile, aFileLen), encoding);
    result = objects.at(aObjID)->obj->matchFile(utf8File, aFileEncoding);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
}

// ****************************************************************************
// match object against text and replace occurences
// aObjID                   - Scripal object ID
// aText,aTextLen           - text 
// aReplace,aReplaceLen     - replacement 
// return - true if at least one result occured, result[0] is replaced text
// ****************************************************************************
bool replace(int aObjID, const char* aText, int aTextLen, const char* aReplace, int aReplaceLen) {
  std::string utf8Text, utf8Replace;
  bool result;

  try {
    result = objects.at(aObjID)->obj->replace(static_cast<std::string>(std::string_view(aText, aTextLen)), static_cast<std::string>(std::string_view(aReplace, aReplaceLen)), encoding);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
}  

// ****************************************************************************
// match object against file and replace occurences
// aObjID                    - Scripal object ID
// aFileIn, aFileInLen       - source file 
// aFileOut,aFileOutLen      - output file 
// aReplace, aReplaceLen     - replacement 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool replaceFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, const char* aReplace, int aReplaceLen, int aFileEncoding) {
  std::string utf8FileIn, utf8FileOut, utf8Replace;
  bool result;

  try {
    utf8FileIn  = encToUtf8(std::string_view(aFileIn, aFileInLen), encoding);
    utf8FileOut = encToUtf8(std::string_view(aFileOut, aFileOutLen), encoding);
    utf8Replace = encToUtf8(std::string_view(aReplace, aReplaceLen), encoding);
    result = objects.at(aObjID)->obj->replaceFile(utf8FileIn, utf8FileOut, utf8Replace, aFileEncoding);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
} 

// ****************************************************************************
// split object 
// aObjID                 - Scripal object ID
// aText,aTextLen         - text 
// return - true if at least one result, result[x] is splitted text
// ***************************************************************************
bool split(int aObjID, const char* aText, int aTextLen) {
  std::string utf8Text;
  bool result;
  
  try {
    if (encoding != ENC_UTF8) {
      utf8Text = encToUtf8(std::string_view(aText, aTextLen), encoding);
      result = objects.at(aObjID)->obj->split(utf8Text);
      encodeResults(static_cast<std::string>(std::string_view(aText, aTextLen)), utf8Text, *objects.at(aObjID)->obj, encoding);
    } else {
      result = objects.at(aObjID)->obj->split(static_cast<std::string>(std::string_view(aText, aTextLen)));
    }
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
} 

// ****************************************************************************
// split file
// aObjID                - Scripal object ID
// aFileIn,aFileInLen    - source file 
// aFileOut,aFileOutLen  - output file 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool splitFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, int aFileEncoding) {
  std::string utf8FileIn, utf8FileOut;
  bool result;
  
  try {
    utf8FileIn  = encToUtf8(std::string_view(aFileIn, aFileInLen), encoding);
    utf8FileOut = encToUtf8(std::string_view(aFileOut, aFileOutLen), encoding);
    result = objects.at(aObjID)->obj->splitFile(utf8FileIn, utf8FileOut, aFileEncoding);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
} 

// ****************************************************************************
// search in files, results will be global (object ID = 0)
// aObjID                          - Scripal object ID
// aPath, aPathLen                 - path to search    
// aDirExtension,aDirExtensionLen  - file types to search for in directories (default: "*")
// aRecursive                      - if true, recurse through directories
// return - true if at least one result occured
// ****************************************************************************
bool searchFiles(int aObjID, const char* aPath, int aPathLen, const char* aDirExtension, int aDirExtensionLen , bool aRecursive) {
  std::string utf8Path, utf8Extension;
  bool result;
  
  try {
    utf8Path  = encToUtf8(std::string_view(aPath, aPathLen), encoding);
    utf8Extension = encToUtf8(std::string_view(aDirExtension, aDirExtensionLen), encoding);
    result = objects.at(aObjID)->obj->fileSearch(utf8Path, utf8Extension, aRecursive);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(result);
}

// ****************************************************************************
// get object result size
// aObjID     - Scripal object ID 
// return     - no of results
// ****************************************************************************
int getResultSize(int aObjID) {
  try {
    return(objects.at(aObjID)->obj->results.size());
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(0);
  }
}

// ****************************************************************************
// get object result specs
// aObjID        - Scripal object ID 
// aEncoding     - encoding
// aMatchType    - match type
// aPositionType - position type
// return        - no of results
// ****************************************************************************
LIB_CTYPE bool getResultSpecs(int aObjID, int aIndex, int* aEncoding, int* aMatchType, int* aPositionType) {
  try {
    if (aIndex >= (int)objects.at(aObjID)->obj->results.positions.size())
      return(false);
    *aEncoding  = objects.at(aObjID)->obj->results.encoding; 
    *aMatchType = objects.at(aObjID)->obj->results.matchType; 
    *aPositionType = objects.at(aObjID)->obj->results.positionType; 
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }
  return(true); 
}

// ****************************************************************************
// get object result string
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultStr(int aObjID, int aIndex, const char** aResult, int* aResultLen) {
  try {
    if (aIndex >= (int)objects.at(aObjID)->obj->results.size())
      return(false);
    iMemory.addBuffer(objects.at(aObjID)->obj->results.text[aIndex], aResult, aResultLen);
    #ifdef DEBUG_SCRIPAL
    std::cout << "getResultStr[" << std::to_string(aIndex) << "] ,  " << std::string(*aResult) << "\n";
    #endif
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(true); 
}

// ****************************************************************************
// get object result filename
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultFileName(int aObjID, int aIndex, const char** aResult, int* aResultLen) {
  try {
    if (aIndex >= (int)objects.at(aObjID)->obj->results.size())
      return(false);
    iMemory.addBuffer(objects.at(aObjID)->obj->results.fileNames[aIndex], aResult, aResultLen);
    #ifdef DEBUG_SCRIPAL
    std::cout << "getResultFileName[" << std::to_string(aIndex) << "] ,  " << std::string(*aResult) << "\n";
    #endif
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(true); 
}

// ****************************************************************************
// get object result tag
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultTag(int aObjID, int aIndex, const char** aResult, int* aResultLen) {
  try {
    if (aIndex >= (int)objects.at(aObjID)->obj->results.size())
      return(false);
    iMemory.addBuffer(objects.at(aObjID)->obj->results.tags[aIndex], aResult, aResultLen);
    #ifdef DEBUG_SCRIPAL
    std::cout << "getResultTag[" << std::to_string(aIndex) << "] ,  " << std::string(*aResult) << "\n";
    #endif
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(true); 
}

// ****************************************************************************
// get object result byte position
// aObjID     - Scripal object ID or 0 for global results
// aIndex     - result index  
// aPosStart  - position start
// aPosEnd    - position end
// return     - true if result exists 
// ****************************************************************************
bool getResultPos(int aObjID, int aIndex, int* aPosStart, int* aPosEnd) {
  *aPosStart = 0;*aPosEnd = 0; 
  try {
    if (aIndex >= (int)objects.at(aObjID)->obj->results.positions.size())
      return(false);
    *aPosStart = posToInt(objects.at(aObjID)->obj->results.positions[aIndex][0]); 
    *aPosEnd   = posToInt(objects.at(aObjID)->obj->results.positions[aIndex][1]);
    #ifdef DEBUG_SCRIPAL
    std::cout << "getResultPos[" << std::to_string(aIndex) << "] ,  " << std::to_string(*aPosStart) << " , " << std::to_string(*aPosEnd) << "\n";
    #endif
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(true); 
}

// ****************************************************************************
// get object result rating (block and nearest))
// aObjID     - Scripal object ID or 0 for global results
// aIndex     - result index  
// aRating    - rating 0..1 
// return     - true if result exists 
// ****************************************************************************
bool getResultRating(int aObjID, int aIndex, double* aRating) {
  *aRating = 0; 
  try {
    if (aIndex >= (int)(objects.at(aObjID)->obj->results.ratings.size()))
      return(false);
    *aRating = objects.at(aObjID)->obj->results.ratings[aIndex]; 
    #ifdef DEBUG_SCRIPAL
    std::cout << "getResultRating[" << std::to_string(aIndex) << "] ,  " << std::to_string(*aRating) << "\n";
    #endif
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }

  return(true); 
}

// ****************************************************************************
// get result as JSON string
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result  
// return                 - true if result exists
// ***************************************************************************
bool getResultJSON(int aObjID, const char** aResult, int* aResultLen) {
  try { 
    if (!objects.at(aObjID)->obj->isResult()) 
      return(false);
    iMemory.addBuffer(anyResultsToJSON(objects.at(aObjID)->obj->results, encoding), aResult, aResultLen);
    return(true);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }
}

// ****************************************************************************
// get result as CSV separated string
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result 
// return                 - true if result exists
// ***************************************************************************
bool getResultCSV(int aObjID, const char** aResult, int* aResultLen) {
  try { 
    if (!objects.at(aObjID)->obj->isResult()) 
      return(false);
    iMemory.addBuffer(anyResultsToCSV(objects.at(aObjID)->obj->results, encoding), aResult, aResultLen);
    return(true);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }
}

// ****************************************************************************
// get result as string in human readable format
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result 
// return                 - true if result exists
// ***************************************************************************
bool getResultHRF(int aObjID, const char** aResult, int* aResultLen) {
  try { 
    if (!objects.at(aObjID)->obj->isResult()) 
      return(false);
    iMemory.addBuffer(anyResultsToHRF(objects.at(aObjID)->obj->results, encoding), aResult, aResultLen);
    return(true);
  } catch(const std::exception &aExc) {
    setError(ERR_SYSTEM + ", object ID " + std::to_string(aObjID) + ", error: " + aExc.what());
    return(false);
  }
}

// ****************************************************************************  
// set config according to given locale 
// aLoc, aLocLen - locale
// ****************************************************************************
bool configSetLocale(const char* aLoc, int aLocLen) {
  return(config.setLocale(encToUtf8(std::string_view(aLoc, aLocLen), encoding)));
}

// ****************************************************************************  
// get/set boolean key in config, aKey, aKeyLen: key aValue: value to set
// ****************************************************************************  
bool configGetBool(const char* aKey, int aKeyLen, bool* aValue) {
  return(config.get(encToUtf8(std::string_view(aKey, aKeyLen), encoding), *aValue));
} 
bool configSetBool(const char* aKey, int aKeyLen, bool aValue){
  return(config.set(encToUtf8(std::string_view(aKey, aKeyLen), encoding), aValue));
}

// ****************************************************************************  
// get/set character key in config, aKey, aKeyLen: key aValue: value to set
// ****************************************************************************  
bool configGetChar(const char* aKey, int aKeyLen, char32_t* aValue) {
  return(config.get(encToUtf8(std::string_view(aKey, aKeyLen), encoding), *aValue));
} 
bool configSetChar(const char* aKey, int aKeyLen, char32_t aValue) {
  return(config.set(encToUtf8(std::string_view(aKey, aKeyLen), encoding), aValue));
}

// ****************************************************************************  
// get/set integer key in config, aKey, aKeyLen: key,  aValue: value to set
// ****************************************************************************  
bool configGetInt(const char* aKey, int aKeyLen, int* aValue) {
  return(config.get(encToUtf8(std::string_view(aKey, aKeyLen), encoding), *aValue));
} 
bool configSetInt(const char* aKey, int aKeyLen, int aValue){
  return(config.set(encToUtf8(std::string_view(aKey, aKeyLen), encoding), aValue));
}

// ****************************************************************************  
// get/set string key in config, aKey, aKeyLen: key aValue, aValueLen: value to set
// ****************************************************************************  
bool configGetStr(const char* aKey, int aKeyLen, const char** aValue, int* aValueLen) {
  std::string resStr;
  if (!config.get(encToUtf8(std::string_view(aKey, aKeyLen), encoding), resStr)) return(false);
  iMemory.addBuffer(utf8ToEnc(resStr, encoding),aValue, aValueLen);
  return(true);
}
bool configSetStr(const char* aKey, int aKeyLen, const char* aValue, int aValueLen) {
  return(config.set(encToUtf8(std::string_view(aKey, aKeyLen), encoding), encToUtf8(std::string_view(aValue, aValueLen), encoding)));
}

// ****************************************************************************
// get string from config vector, aKey,aKeyLen: key name, aIndex: index
// aValue,aValueLen: value to get
// ****************************************************************************
bool configGetPart(const char* aKey, int aKeyLen, int aIndex, const char** aValue, int* aValueLen) {
  std::string resStr;
  if (!config.get(encToUtf8(std::string_view(aKey, aKeyLen), encoding), aIndex, resStr)) return(false);
  iMemory.addBuffer(utf8ToEnc(resStr, encoding),aValue, aValueLen);
  return(true);
}

// ****************************************************************************
// add string to config vector, aKey,aKeyLen: key name, aValue,aValueLen: value to add
// ****************************************************************************
bool configAddPart(const char* aKey, int aKeyLen, const char* aValue, int aValueLen) {
  return(config.add(encToUtf8(std::string_view(aKey, aKeyLen), encoding), utf8ToEnc(std::string_view(aValue, aValueLen), encoding)));
} 

// ****************************************************************************
// clear vector in config, aKey, aKeyLen: key 
// ****************************************************************************
bool configClear(const char* aKey, int aKeyLen) {
 return (config.clear(encToUtf8(std::string_view(aKey, aKeyLen), encoding)));
} 

// ****************************************************************************
// config as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool configToJSON( const char** aValue, int* aValueLen) {
  iMemory.addBuffer(utf8ToEnc(config.toJSON({"all"}), encoding),aValue, aValueLen);
  return(true);
}

// ****************************************************************************
// parts of config as JSON string
// aValue,aValueLen: JSON string
// aNames, aNameLen : list of config values, separated by ','
// ****************************************************************************
bool configToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen) {
  std::string nameList = encToUtf8(std::string_view(aNames, aNamesLen), encoding);
  std::vector<std::string> names;
  tokenize(nameList, ",", names);
  iMemory.addBuffer(utf8ToEnc( config.toJSON(names), encoding),aValue, aValueLen);
  return(true);
}

// ****************************************************************************
// config as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool configFromJSON(const char* aValue, int aValueLen) {
  return(config.fromJSON(encToUtf8(std::string_view(aValue, aValueLen), encoding)));
}

// ****************************************************************************
// store config in given file, aFile, aFileLen - path to file
// ****************************************************************************
bool configStore(const char* aFile, int aFileLen) {
  return(config.store(encToUtf8(std::string_view(aFile, aFileLen), encoding)));
}

// ****************************************************************************
// restore config from given file, aFile, aFileLen - path to file
// ****************************************************************************
bool configRestore(const char* aFile, int aFileLen) {
  return(config.restore(encToUtf8(std::string_view(aFile, aFileLen), encoding)));
}

// ****************************************************************************
// store templates in given file, aFile, aFileLen - path to file
// ****************************************************************************
bool templateStore(const char* aFile, int aFileLen) {
  return(templates.store(encToUtf8(std::string_view(aFile, aFileLen), encoding)));
}

// ****************************************************************************
// restore templates from given file, aFile, aFileLen - path to file
// ****************************************************************************
bool templateRestore(const char* aFile, int aFileLen) {
  return(templates.restore(encToUtf8(std::string_view(aFile, aFileLen), encoding)));
}

// ****************************************************************************
// set template source with given name, aName, aNamelen - name of template
// aSource, aSourceLen - template source
// ****************************************************************************
bool templateSet(const char* aName, int aNameLen, const char* aSource, int aSourceLen) {
  return(templates.set(encToUtf8(std::string_view(aName, aNameLen), encoding), encToUtf8(std::string_view(aSource, aSourceLen), encoding)));
}

// ****************************************************************************
// clear template with given name, aName, aNamelen - name of template
// ****************************************************************************
bool templateClear(const char* aName, int aNameLen) {
  return(templates.clear(encToUtf8(std::string_view(aName, aNameLen), encoding)));
}

// ****************************************************************************
// clear all templates
// ****************************************************************************
void templateReset() {
  templates.reset();
}

// ****************************************************************************
// find template with given name, aName, aNamelen - name of template
// aSource, aSourceLen - template source
// ****************************************************************************
bool templateGet(const char* aName, int aNameLen, const char** aSource, int* aSourceLen) {
  std::string resStr = templates.get(encToUtf8(std::string_view(aName, aNameLen), encoding));
  if (resStr == "") {
    *aSource = nullptr;
    aSourceLen = 0;
    return(false);
  } else {
    resStr = utf8ToEnc(resStr, encoding);
    iMemory.addBuffer(resStr,aSource, aSourceLen);
    return(true);
  }
}

// ****************************************************************************
// all templates as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool templatesToJSON( const char** aValue, int* aValueLen) {
  iMemory.addBuffer(utf8ToEnc( templates.toJSON({"all"}), encoding),aValue, aValueLen);
  return(true);
}

// ****************************************************************************
// some parts of templates as JSON string
// aValue,aValueLen: JSON string
// aNames, aNameLen : list of template names, separated by ','  
// ****************************************************************************
bool templatesToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen) {
  std::string nameList = encToUtf8(std::string_view(aNames, aNamesLen), encoding);
  std::vector<std::string> names;
  tokenize(nameList, ",", names);
  iMemory.addBuffer(utf8ToEnc( templates.toJSON(names), encoding),aValue, aValueLen);
  return(true);
}

// ****************************************************************************
// templates from JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool templatesFromJSON(const char* aValue, int aValueLen) {
  return(templates.fromJSON(encToUtf8(std::string_view(aValue, aValueLen), encoding)));
}

