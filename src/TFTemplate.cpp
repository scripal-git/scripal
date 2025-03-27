// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "general.hpp"
#include "text.hpp"
#include <iterator>
#include "TFTemplate.hpp"
#include "extern.hpp"

// define lock mechanisms for thread-safe global templates
#ifdef LINUX 
#define LOCK_TEMPS const std::lock_guard<std::recursive_mutex> lock(templateMutex); 
#define UNLOCK_TEMPS  
#endif 

#ifdef WINDOWS 
#define LOCK_TEMPS CRITICAL_SECTION CriticalSection; InitializeCriticalSection(&CriticalSection); 
#define UNLOCK_TEMPS DeleteCriticalSection(&CriticalSection); 
#endif  

namespace Scripal {

// ****************************************************************************
// get a template by name
// aName - template name
// return source
// ****************************************************************************
std::string TFTemplate::get(const std::string &aName) {
  std::string result = "";
  LOCK_TEMPS
  try {
    result = sources.at(trimText(lowerText(aName)));
  } catch (std::exception &aExc) {
    result = "";
  }  
  UNLOCK_TEMPS
  return(result);
}

// ****************************************************************************
// add or reset a template 
// aName   - template name
// aSource - template source
// ****************************************************************************
bool TFTemplate::set(const std::string &aName,const std::string &aSource) {
  TFNum num;
  if(textUIntToNum(aName,num))  {
    setError(ERR_NESTED_TEMPLATE + ", '" + aName + "'"); 
    return(false);
  };  
  LOCK_TEMPS

  if (sources.find(lowerText(aName)) != sources.end()) {
    setError(ERR_DUPLICATE_TEMPLATE + ", '" + aName + "'");
    UNLOCK_TEMPS
    return(false);
  };   
  sources[trimText(lowerText(aName))] = aSource;
  UNLOCK_TEMPS
  return(true);
}

// ****************************************************************************
// delete a template 
// aName - template name
// ****************************************************************************
bool TFTemplate::clear(const std::string &aName) {
  LOCK_TEMPS
  if (sources.erase(trimText(lowerText(aName))) == 0) {
    UNLOCK_TEMPS
    return(false);
  } else {
    UNLOCK_TEMPS
    return(true);
  }
}

// ****************************************************************************
// delete all templates 
// ****************************************************************************
void TFTemplate::reset() {
  LOCK_TEMPS
  sources.clear();
  UNLOCK_TEMPS
}

// ****************************************************************************
// read template definitions from Scripal source, store under name
//   and erase from source
// syntax:  < name = < template >> 
// aSource - Scripal source code
// ****************************************************************************
void TFTemplate::read(std::string &aSource) {
  std::size_t posPtr = 0, endPtr = nPos, defPos;
  std::string newSource;
  bool textOpen = false;
  std::array<std::size_t ,2> block, innerBlock;
  LOCK_TEMPS
  
  try {
    while (posPtr < aSource.size()) {
      block = getBlock(aSource, posPtr, getCP(ctrlTempBegin, 0), getCP(ctrlTempEnd, 0), ctrlEndPoints);
      if (block[0] == nPos)
        break; 

      innerBlock = getBlock(aSource, block[0] + ctrlBlockBegin.size(), getCP(ctrlBlockBegin, 0), getCP(ctrlBlockEnd, 0), ctrlEndPoints);
      if ((innerBlock[0] == nPos) || (innerBlock[1] > block[1])) {
        //  no defintion -> continue 
        posPtr = block[1] + 1;
        continue;
      };

      // '=' follows?
      defPos = aSource.find("=", block[0]);
      if ((defPos > innerBlock[0]) || (defPos == nPos)) {
        // no definiton -> continue
        posPtr = block[1] + 1;
        continue;
      }

      // template found -> get name
      std::string tempName = trimText(aSource.substr(block[0] + ctrlTempBegin.size(), defPos - block[0] - ctrlTempBegin.size()));
    
      std::string tempSource = aSource.substr(innerBlock[0] + ctrlBlockBegin.size(), innerBlock[1] - innerBlock[0] - ctrlBlockEnd.size());
      set(tempName, tempSource);
   
      aSource.erase(block[0], block[1] - block[0] + 1);
      posPtr = block[0];
    } 
  } catch (std::exception &aC) {
    UNLOCK_TEMPS
    return;
  }
  UNLOCK_TEMPS
  return;
}

// ****************************************************************************
// find start and end of block enclosed by aStartCP, aEndCP in sripal code 
// skip text block in scripal code unless escaped
// if final code is reached -> break
// aSource    - Scripal source code
// aIndex     - start index in source
// aStartCP   - start marker
// aEndCP     - end marker (must differ from aStartCP!)
// aFinalCP   - quit block, if one of these codepoinst is found -> no result
// return     - start and end of block including ctrl chars or <nPos,nPos>
// ****************************************************************************
std::array<std::size_t, 2> TFTemplate::getBlock(const std::string &aSource, std::size_t aIndex, char32_t aStartCP, char32_t aEndCP, std::vector<char32_t> aFinalCP) {
  std::size_t posPtr = aIndex;
  char32_t codePoint = 0, lastCodePoint = 0, textPoint = 0;
  int blockLevel = 0;
  bool textOpen = false;
  std::array<std::size_t, 2> result = {nPos, nPos};

  LOCK_TEMPS

  while (posPtr < aSource.size()) {
    int len = getCP(aSource, posPtr, codePoint);

    bool cont = false;

    // text block starts/ends?
    int index = findIdentifier(SP_GRAMMAR_CTRL, toString(codePoint));
    if (index != -1) {
      if (identifiers[index].codeID == SP_CTRL_TEXT) {
        if (lastCodePoint != escChar) {
          if (!textOpen) textOpen = true; else textOpen = false;
          cont = true; 
        }
      };
    };
 
    if (textOpen) {
      // ingnore anything in open text, unless aStartCP/aEndCP is escaped
      if ((lastCodePoint == escChar) && ((codePoint == aStartCP) || (codePoint == aEndCP)))
        cont = false;
      else
        cont = true;
    } else if ((blockLevel == 0) && (codePoint != aStartCP)) {
      // no block start= continue
      cont = true;
    }

    // nothing to process, continue?
    if (cont) {
      lastCodePoint = codePoint;
      posPtr += len;
      continue;
    }    
    
    auto it = std::find(aFinalCP.begin(), aFinalCP.end(), codePoint);

    if (it != aFinalCP.end()) {
      result[0] = result[1] = nPos;
      UNLOCK_TEMPS
      return(result);  
    }
    
    // block start symbol?
    if (codePoint == aStartCP) { 
      if (blockLevel == 0)
        result[0] = posPtr;
      ++blockLevel;  
    }

    // block end symbol?
    if (codePoint == aEndCP) {
      --blockLevel;
      if (blockLevel == 0) {
        result[1] = posPtr + len - 1;
        if ((result[0] == nPos) || (result[0] >= result[1]))
          result[0] = result[1] = nPos;
        UNLOCK_TEMPS
        return(result);
      }
    }  
    
    lastCodePoint = codePoint;
    posPtr += len;
  };
 
  if ((result[1] == nPos) || (result[0] >= result[1]))
    result[0] = result[1] = nPos;
  UNLOCK_TEMPS
  return(result);
}

// ****************************************************************************
// replace templates in source
// aSource   - Scripal source code
// result - true if template found  
// ****************************************************************************
bool TFTemplate::apply(std::string &aSource, std::vector<std::string> &aArgs) {
  std::size_t posPtr = 0, endPtr;
  std::string newSource, content;
  int len, count = 0;
  std::array<std::size_t, 2> block, innerBlock;

  LOCK_TEMPS

  newSource = "";
  ++templateLevel;

  // too many recursive calls?
  if (templateLevel > 1000) {
    setError(ERR_NESTED_TEMPLATE, posPtr, aSource); 
    templateLevel = 0;
    UNLOCK_TEMPS
    return(false);
  }

  while (true) {
    // get template call <name <par1><par2>...>
    block = getBlock(aSource, posPtr, getCP(ctrlTempBegin, 0), getCP(ctrlTempEnd, 0), ctrlEndPoints);
    if (block[0] == nPos) {
      newSource += aSource.substr(posPtr, aSource.size() - posPtr);
      break;
    };
 
    // copy source so far
    newSource += aSource.substr(posPtr, block[0] - posPtr);

    // extract pure template call name <par1><par2>..
    std::string tempCall = aSource.substr(block[0] + ctrlTempBegin.size(), block[1] - block[0] - ctrlTempBegin.size());
    if (tempCall.size() == 0) {
      setError(ERR_NO_TEMPLATE, posPtr, aSource); 
      templateLevel = 0;
      UNLOCK_TEMPS
      return(false);
    }
   
    // get arguments in call
    std::vector<std::string> args;
    std::size_t startName = block[0] + ctrlTempBegin.size(), endName = nPos;
    std::size_t curPosPtr = startName; 
    while(true) {
      innerBlock = getBlock(tempCall, curPosPtr, getCP(ctrlBlockBegin, 0),  getCP(ctrlBlockEnd, 0), ctrlEndPoints);
      if (innerBlock[0] == nPos)
        break;
      std::size_t beginArg = innerBlock[0] + ctrlBlockBegin.size();  
      std::size_t lenArg = innerBlock[1] - beginArg;  
      if (args.size() == 0) 
        endName = startName + innerBlock[0] - 1;

      content =  tempCall.substr(beginArg, lenArg);
      apply(content, aArgs);
      args.push_back(content);
      curPosPtr = innerBlock[1] + 1;
      if (curPosPtr >= tempCall.size())
        break;
    }
    
    if (endName == nPos)
      endName = block[1] - ctrlTempEnd.size();

    // get template name or argument
    content =  aSource.substr(startName, endName - startName + 1);

    int argIndex = -1;
 
    TFNum num;
    if(textUIntToNum(content,num)) 
      argIndex = (int)num;
    
    // find template source or argument
    if (argIndex >= 0) {
      // substitute from argument
      if (argIndex > aArgs.size()) {
        setError(ERR_NO_TEMPLATE, posPtr, aSource); 
        templateLevel = 0;
        UNLOCK_TEMPS
        return(false);          
      }
      newSource += aArgs[argIndex - 1];    
    } else {
      // substitute from template 
      std::string orgTempSource = get(content);

      if (orgTempSource == "") {     
        setError(ERR_NO_TEMPLATE, posPtr, aSource); 
        templateLevel = 0;     
        UNLOCK_TEMPS
        return(false);     
      }
      std::string tempSource = orgTempSource;
      apply(tempSource, args);
      newSource += tempSource;    
    };

    ++count;  
    // find next template after template call
    posPtr = block[1] + 1;
  } 

  --templateLevel;
  UNLOCK_TEMPS
  if (count > 0) {
    aSource = newSource;
    return(true);
  } else {
    return(false);
  }    
}

// ****************************************************************************
// store templates in given file
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFTemplate::store(const std::string &aFile) {
  LOCK_TEMPS
  std::ofstream file(aFile, std::ios_base::binary);
 
  if (file.is_open()) {
    std::string json = toJSON({"all"});
    file.write(json.c_str(), json.size());
    UNLOCK_TEMPS
    return(true);
  } else {
    setError(ERR_FILE +", cannot open file '" + aFile + "'");
  }
  UNLOCK_TEMPS
  return(false);
}  

// ****************************************************************************
// restore templates from given file, add to current ones
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFTemplate::restore(const std::string &aFile) {
  LOCK_TEMPS
  std::ifstream file(aFile, std::ios_base::binary);
  
  if (file.is_open()) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    UNLOCK_TEMPS
    return(fromJSON(json));
  } else {
    setError(ERR_FILE + ", cannot open file '" + aFile + "'");
  };
  UNLOCK_TEMPS
  return(false);
} 

// ****************************************************************************
// convert templates to JSON format
// aVec - specify names to store or {"all"} for entire config
// return - config as JSON
// ****************************************************************************
std::string TFTemplate::toJSON(std::vector<std::string> aVec) {
  Poco::JSON::Object::Ptr cJSON = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
  LOCK_TEMPS

  if (std::find(aVec.begin(), aVec.end(), "all") != aVec.end()) {
    for (auto it = sources.begin(); it != sources.end(); it++) {
      cJSON->set(it->first,  it->second);
    };
  } else {
    for (auto &it : aVec) {
      if (sources.find(it) != sources.end())
        cJSON->set(it, get(it));
    }
  }

  std::stringstream ss;
  cJSON->stringify(ss, 2, 5);
  UNLOCK_TEMPS
  return(ss.str());
}

// ****************************************************************************
// read templates from JSON format (also only parts)
// aJSON - templates as JSON
// ****************************************************************************
bool TFTemplate::fromJSON(const std::string &aJSON) {
  LOCK_TEMPS
  try {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(aJSON);
    Poco::JSON::Object::Ptr cJSON = result.extract<Poco::JSON::Object::Ptr>();
    Poco::Dynamic::Var value; 
    Poco::JSON::Object::NameList names;

    cJSON->getNames(names);
    for (std::size_t i = 0; i < names.size(); ++i) {
      value = cJSON->get(names[i]);
      set(names[i], value);
    };
 
  } catch (const std::exception &aExp) {
      UNLOCK_TEMPS
      return(false);
  } 
  UNLOCK_TEMPS
  return(true);
}



TFTemplate templates;

}