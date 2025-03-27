// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "general.hpp"
#include "text.hpp"
#include "TFConfig.hpp"
#include "extern.hpp"

namespace Scripal {

// ****************************************************************************
// set string array from Array::Ptr 
// aArray  - Array::Ptr array
// aVector - string vector
// ****************************************************************************
bool setVector(const Poco::JSON::Array::Ptr &aArray ,std::vector<std::string> &aVector) {
  aVector.clear();
  for (Poco::JSON::Array::ConstIterator it= aArray->begin(); it != aArray->end(); ++it) {
     std::string valStr = it->convert<std::string>();
     aVector.push_back(it->convert<std::string>()); 
  }
  return(true);
}

// C++ getters/setters for different values
bool TFConfig::set(const std::string &aKey, bool aValue) {
  if (lowerText(aKey) == "debugcompile") { debugCompile = aValue; return(true); };
  if (lowerText(aKey) == "debugrun") { debugRun = aValue; return(true); };
  if (lowerText(aKey) == "showcode") { showCode = aValue; return(true); };
  if (lowerText(aKey) == "measuretime") { measureTime = aValue; return(true); };
  if (lowerText(aKey) == "useempty") { useEmpty = aValue; return(true); };
  if (lowerText(aKey) == "translatedigits") { translateDigits = aValue; return(true); };
  if (lowerText(aKey) == "verboseresult") { verboseResult = aValue; return(true); };
  return(false);
}
  
bool TFConfig::get(const std::string &aKey, bool &aValue) {
  if (lowerText(aKey) == "debugcompile") { aValue = debugCompile; return(true); };
  if (lowerText(aKey) == "debugrun") { aValue = debugRun; return(true); };
  if (lowerText(aKey) == "showcode") { aValue = showCode; return(true); };
  if (lowerText(aKey) == "measuretime") { aValue = measureTime; return(true); };
  if (lowerText(aKey) == "useempty") { aValue = useEmpty; return(true); };
  if (lowerText(aKey) == "translatedigits") { aValue = translateDigits; return(true); };
  if (lowerText(aKey) == "verboseresult") { aValue = verboseResult; return(true); };
  return(false);
}
 
bool TFConfig::set(const std::string &aKey, char32_t aValue) {
  if (lowerText(aKey) == "decimalpoint") { decimalPoint = aValue; return(true); };
  if (lowerText(aKey) == "thousandssep") { thousandsSep = aValue; return(true); };
  if (lowerText(aKey) == "possign") { posSign = aValue; return(true); };
  if (lowerText(aKey) == "negsign") { negSign = aValue; return(true); };
  return(false);
}

bool TFConfig::get(const std::string &aKey, char32_t &aValue) {
  if (lowerText(aKey) == "decimalpoint") { aValue = decimalPoint; return(true); };
  if (lowerText(aKey) == "thousandssep") { aValue = thousandsSep; return(true); };
  if (lowerText(aKey) == "possign") { aValue = posSign; return(true); };
  if (lowerText(aKey) == "negsign") { aValue = negSign; return(true); };
  return(false);
}

bool TFConfig::set(const std::string &aKey, int aValue) {
  if (lowerText(aKey) == "encoding") { 
    if ((aValue >= ENC_DEFAULT) && (aValue <= ENC_WIN1258)) { encoding = aValue; return(true); }
    else { return(false); }};
  if (lowerText(aKey) == "logencoding") { 
    if ((aValue >= ENC_DEFAULT) && (aValue <= ENC_WIN1258)) { logEncoding = aValue; return(true); }
    else { return(false); }};
  if (lowerText(aKey) == "maxfilesize") { maxFileSize = aValue; return(true); };
  if (lowerText(aKey) == "patternnearest") { 
    if ((aValue >= PATTERN_LEVEN_WORD) && (aValue <= PATTERN_JAROWINKLER_WORD)) { patternNearest = aValue; return(true); }
    else { return(false); }};
  if (lowerText(aKey) == "patternblock") { 
    if ((aValue >= PATTERN_LEVEN_WORD) && (aValue <= PATTERN_JAROWINKLER_WORD)) { patternBlock = aValue; return(true); }
    else { return(false); }};
  if (lowerText(aKey) == "positiontype") {
    if ((aValue >= POS_UTF8) && (aValue <= POS_COUNT)) { positionType = aValue; return(true); }
    else { return(false); }};

  return(false);
}

bool TFConfig::get(const std::string &aKey, int &aValue) {
  if (lowerText(aKey) == "encoding") { aValue = encoding; return(true); };
  if (lowerText(aKey) == "logencoding") { aValue = logEncoding; return(true); };
  if (lowerText(aKey) == "maxfilesize") { aValue = maxFileSize; return(true); };
  if (lowerText(aKey) == "patternnearest") { aValue = patternNearest; return(true); };
  if (lowerText(aKey) == "patternblock") { aValue = patternBlock; return(true); };
  if (lowerText(aKey) == "positiontype") { aValue = positionType; return(true); };
  return(false);
}

bool TFConfig::set(const std::string &aKey, const std::string &aValue) {
  if (lowerText(aKey) == "logchannel") { logChannel = aValue; return(true); };
  if (lowerText(aKey) == "pdfreader")  { pdfReader = aValue; return(true); };
  if (lowerText(aKey) == "frmreader") { frmReader = aValue; return(true); };
  if (lowerText(aKey) == "apppath") { appPath = aValue; return(true); };
  return(false);
}

bool TFConfig::get(const std::string &aKey, std::string &aValue) {
  if (lowerText(aKey) == "logchannel") { aValue = logChannel; return(true); };
  if (lowerText(aKey) == "pdfreader")  { aValue = pdfReader; return(true); };
  if (lowerText(aKey) == "frmreader") { aValue = frmReader; return(true); };
  if (lowerText(aKey) == "apppath") { aValue = appPath; return(true); };
  return(false);
}

bool TFConfig::set(const std::string &aKey, const std::vector<std::string> &aValue) {
  if (lowerText(aKey) == "sentenceend") { sentenceEnd = aValue; return(true); }; 
  if (lowerText(aKey) == "separators") { separators = aValue; return(true); }; 
  if (lowerText(aKey) == "abbreviations") { abbreviations = aValue; return(true); }; 
  return(false);
}

bool TFConfig::get(const std::string &aKey, std::vector<std::string> &aValue) {
  if (lowerText(aKey) == "sentenceend") { aValue = sentenceEnd; return(true); }; 
  if (lowerText(aKey) == "separators") { aValue = separators; return(true); }; 
  if (lowerText(aKey) == "abbreviations") { aValue = abbreviations; return(true); }; 
  return(false);
}

bool TFConfig::get(const std::string &aKey, int aIndex, std::string &aValue) {
  if (lowerText(aKey) == "sentenceend") { aValue = sentenceEnd[aIndex]; return(true); }; 
  if (lowerText(aKey) == "separators") { aValue = separators[aIndex]; return(true); }; 
  if (lowerText(aKey) == "abbreviations") { aValue = abbreviations[aIndex]; return(true); }; 
  return(false);
}

bool TFConfig::clear(const std::string &aKey) {
  if (lowerText(aKey) == "sentenceend") { sentenceEnd.clear(); return(true); }; 
  if (lowerText(aKey) == "separators") { separators.clear(); return(true); }; 
  if (lowerText(aKey) == "abbreviations") { abbreviations.clear(); return(true); }; 
  return(true); 
}

bool TFConfig::add(const std::string &aKey, const std::string &aValue) {
  if (lowerText(aKey) == "sentenceend") { sentenceEnd.push_back(aValue); return(true); }; 
  if (lowerText(aKey) == "separators") { separators.push_back(aValue); return(true); }; 
  if (lowerText(aKey) == "abbreviations") { abbreviations.push_back(aValue); return(true); }; 
  return(false);
}

// ****************************************************************************
// convert config to JSON format
// aVec - specify names to store or {"all"} for entire config
// return - config as JSON
// ****************************************************************************
std::string TFConfig::toJSON(std::vector<std::string> aVec) {
  Poco::JSON::Object::Ptr cJSON = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
  bool all = false;
  
  if (std::find(aVec.begin(), aVec.end(), "all") != aVec.end()) 
    all = true;

  if ((std::find(aVec.begin(), aVec.end(), "debugCompile") != aVec.end()) || all)
    cJSON->set("debugCompile",  debugCompile);

  if ((std::find(aVec.begin(), aVec.end(), "debugRun") != aVec.end()) || all)
    cJSON->set("debugRun",  debugRun);

  if ((std::find(aVec.begin(), aVec.end(), "showCode") != aVec.end()) || all)
    cJSON->set("showCode",  showCode);

  if ((std::find(aVec.begin(), aVec.end(), "measureTime") != aVec.end()) || all)
    cJSON->set("measureTime",  measureTime);

  if ((std::find(aVec.begin(), aVec.end(), "useEmpty") != aVec.end()) || all)
    cJSON->set("useEmpty",  useEmpty);

  if ((std::find(aVec.begin(), aVec.end(), "translateDigits") != aVec.end()) || all)
    cJSON->set("translateDigits",  translateDigits);

  if ((std::find(aVec.begin(), aVec.end(), "verboseResult") != aVec.end()) || all)
    cJSON->set("verboseResult",  verboseResult);

  if ((std::find(aVec.begin(), aVec.end(), "decimalPoint") != aVec.end()) || all)
    cJSON->set("decimalPoint",  toString(decimalPoint));
 
  if ((std::find(aVec.begin(), aVec.end(), "thousandsSep") != aVec.end()) || all)
    cJSON->set("thousandsSep",  toString(thousandsSep));

  if ((std::find(aVec.begin(), aVec.end(), "posSign") != aVec.end()) || all)
    cJSON->set("posSign",  toString(posSign));

  if ((std::find(aVec.begin(), aVec.end(), "negSign") != aVec.end()) || all)
    cJSON->set("negSign",  toString(negSign));

  if ((std::find(aVec.begin(), aVec.end(), "encoding") != aVec.end()) || all)
    cJSON->set("encoding",  encoding);

  if ((std::find(aVec.begin(), aVec.end(), "logEncoding") != aVec.end()) || all)
    cJSON->set("logEncoding",  logEncoding);

  if ((std::find(aVec.begin(), aVec.end(), "maxFileSize") != aVec.end()) || all)
    cJSON->set("maxFileSize",  maxFileSize);

  if ((std::find(aVec.begin(), aVec.end(), "patternNearest") != aVec.end()) || all)
    cJSON->set("patternNearest",  patternNearest);

  if ((std::find(aVec.begin(), aVec.end(), "patternBlock") != aVec.end()) || all)
    cJSON->set("patternBlock",  patternBlock);

  if ((std::find(aVec.begin(), aVec.end(), "positionType") != aVec.end()) || all)
    cJSON->set("positionType",  positionType);

  if ((std::find(aVec.begin(), aVec.end(), "logChannel") != aVec.end()) || all)
    cJSON->set("logChannel",  logChannel);

  if ((std::find(aVec.begin(), aVec.end(), "pdfReader") != aVec.end()) || all)
    cJSON->set("pdfReader",  pdfReader);

  if ((std::find(aVec.begin(), aVec.end(), "frmReader") != aVec.end()) || all)
    cJSON->set("frmReader",  frmReader);

  if ((std::find(aVec.begin(), aVec.end(), "appPath") != aVec.end()) || all)
    cJSON->set("appPath",  appPath);

  if ((std::find(aVec.begin(), aVec.end(), "sentenceEnd") != aVec.end()) || all)
    cJSON->set("sentenceEnd",  sentenceEnd);

  if ((std::find(aVec.begin(), aVec.end(), "separators") != aVec.end()) || all)
    cJSON->set("separators",  separators);

  if ((std::find(aVec.begin(), aVec.end(), "abbreviations") != aVec.end()) || all)
    cJSON->set("abbreviations",  abbreviations);

  std::stringstream ss;
  cJSON->stringify(ss, 2, 5);
  return(ss.str());
}

// ****************************************************************************
// read config from JSON format (also only parts)
// aJSON - config as JSON
// return - true if valid JSON 
// ****************************************************************************
bool TFConfig::fromJSON(const std::string &aJSON) {
  std::string keyName = "";

  try {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(aJSON);
    Poco::JSON::Object::Ptr cJSON = result.extract<Poco::JSON::Object::Ptr>();
    Poco::Dynamic::Var value; 
    Poco::JSON::Array::Ptr arrValue; 

    std::array keysBool{ "debugCompile" , "debugRun", "showCode", "measureTime", "useEmpty", "translateDigits", "verboseResult"};
    for(int i = 0; i < keysBool.size(); ++i) {
      keyName = keysBool[i];
      value = cJSON->get(keyName);
      if (!value.isEmpty())
        if (!set(keyName, value.convert<bool>()))
          throw std::exception(); 
    };

    std::array keysChar32{ "decimalPoint", "thousandsSep", "posSign", "negSign"};
    for(int i = 0; i < keysChar32.size(); ++i) {
      keyName = keysChar32[i];
      value = cJSON->get(keyName);
      if (!value.isEmpty())
        if (!set(keyName,  getCP(value.toString(), 0)))
          throw std::exception(); 
    };

    std::array keysInt{ "encoding", "logEncoding", "maxFileSize", "patternNearest", "patternBlock", "positionType"};
    for(int i = 0; i < keysInt.size(); ++i) {
      keyName = keysInt[i];
      value = cJSON->get(keyName);
      if (!value.isEmpty())
        if (!set(keyName, value.convert<int>()))
          throw std::exception(); 
    };

    std::array keysString{ "logChannel", "pdfReader", "frmReader", "appPath"};
    for(int i = 0; i < keysString.size(); ++i) {
      keyName = keysString[i];
      value = cJSON->get(keyName);
      if (!value.isEmpty())
        if (!set(keyName, value.convert<std::string>()))
          throw std::exception(); 
    };

    keyName = "sentenceEnd";
    arrValue = cJSON->getArray(keyName);
    if (!arrValue.isNull())
      setVector(arrValue, sentenceEnd);

    keyName = "separators";
    arrValue = cJSON->getArray(keyName);
    if (!arrValue.isNull())
      setVector(arrValue, separators);

    keyName = "abbreviations";
    arrValue = cJSON->getArray(keyName);
    if (!arrValue.isNull())
      setVector(arrValue, abbreviations); 
  } catch (const std::exception &aExp) {
    if (std::string(aExp.what()).find("std::") == std::string::npos)
      setError(ERR_INVALID_CONFIG + " at " + keyName + ", check JSON format: " + aExp.what());
    else  
      setError(ERR_INVALID_CONFIG + ", wrong value for " + keyName);
    return(false);
  } 
  return(true);
}

// ****************************************************************************
// store config in given file 
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFConfig::store(const std::string &aFile) {
  std::ofstream file(aFile);
 
  if (file.is_open()) {
    std::string json = toJSON({"all"});
    file.write(json.c_str(), json.size());
    return(true);
  } else {
    setError(ERR_FILE +", cannot open file '" + aFile + "'");
  }
  return(false);
}  

// ****************************************************************************
// restore config from given file
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFConfig::restore(const std::string &aFile) {
  
  std::ifstream file(aFile, std::ios_base::binary);
  
  if (file.is_open()) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    return(fromJSON(json));
  } else {
    setError(ERR_FILE + ", cannot open file '" + aFile + "'");
  };
  return(false);
} 

// ****************************************************************************
// set temporary config   
// aTemp - temporary JSON values
// return - true if successful
// ****************************************************************************
bool TFConfig::setTemp(const std::string &aJSON) {
  copyJSON = toJSON({"all"});

  return(fromJSON(aJSON));
}

// ****************************************************************************
// set config to original  
// ****************************************************************************
bool TFConfig::resetTemp() {
  if (copyJSON != "")
    return(fromJSON(copyJSON));
  else 
    return(false);  
}
  
// ****************************************************************************
// read config values from Scripal source
// syntax:    % { "showCode" : true, "posSign" : "+" } %
// aSource   - Scripal source code, will be shortened by config section 
// return    - temporary config as JSON 
// ****************************************************************************
std::string TFConfig::read(std::string &aSource) {
  std::size_t startPtr = nPos, posPtr = 0, endPtr = nPos;
  std::string newSource;
  char32_t codePoint;
  std::string result = ""; 
                               
  TFAccess media;
  media.assignString(&aSource);  

  try {
    while (posPtr < media.size()) {
      if (media.substr(posPtr, ctrlConfig.size()) == ctrlConfig) {
        startPtr = posPtr;

        posPtr = findStr(media, ctrlBlockBegin, posPtr);
        if (posPtr == nPos)
          break;
          
        endPtr = findMarkedBlock(media, posPtr, ctrlBlockBegin, ctrlBlockEnd, {}, {"\""});
        if (endPtr == nPos) {
          posPtr += ctrlBlockBegin.size();
          continue;
        }
        break;
      } else {
        posPtr += getCP(aSource, posPtr, codePoint);
      }
    } 
  } catch (std::exception &aExc) {
    errorMsg = ERR_NO_CONFIG_END;
    errorPos = startPtr;
    return("");
  }
  
  if (startPtr != nPos) {
    // take out config from source
    result = media.substr(posPtr, endPtr - posPtr + 1); 
    aSource.erase(startPtr, endPtr - startPtr + 1);
    return(result);
  }
  return(result);

}



}
