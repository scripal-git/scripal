// ***************************************************************************
// general functions and global data used in Scripal 
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef GENERAL_HPP_INCLUDED
#define GENERAL_HPP_INCLUDED

#include "definitions.hpp"
#include "TFScripal.hpp"

using namespace Scripal;

// export under namespace Scripal
namespace Scripal {

// **********************************************
// thread specific globals
// **********************************************
extern thread_local TFConfig config;
extern thread_local std::string configAsJSON;
extern thread_local int encoding;
extern thread_local int logEncoding;
extern thread_local std::string logBuffer;
extern thread_local std::string errorMsg;
extern thread_local std::string errorExp;
extern thread_local std::size_t errorPos;
extern thread_local std::size_t templateLevel;
extern thread_local bool relocateBuffer;

// **********************************************
// globals
// **********************************************
extern bool initialized; 
extern int logChannel;
extern std::ofstream fileChannel;
extern bool isBigEndian;
extern std::size_t totalTime;

extern std::string ctrlInfinite;
extern std::string ctrlRangeBegin;
extern std::string ctrlRangeEnd;
extern std::string ctrlBlockBegin;
extern std::string ctrlBlockEnd;
extern std::string ctrlName;
extern std::string ctrlSemicolon;
extern std::string ctrlNL;
extern std::string ctrlCR;
extern std::string ctrlTempBegin;
extern std::string ctrlTempEnd;
extern std::string ctrlConfig;
extern std::array<TFOprParms, _SP_OPR_END> oprParms;
extern std::vector<char32_t> ctrlEndPoints;

void setError(const std::string& aError);
void setError(const std::string &aError, std::size_t aErrorPos, const std::string &aSource);

// **********************************************
// class for managed memory, non NULL terminated
// **********************************************
class TFManagedMem {
  public:
  std::vector<std::pair<bool, char*>> buffers;  // string buffer to manage
  
  // add a string to the objects buffer
  std::size_t addBuffer(const std::string &aStrElement) {
    std::size_t index;
    bool found = false;
    for (index = 0; index < buffers.size(); ++index) {
      if (buffers[index].first == false) {
        #ifdef DEBUG_SCRIPAL
        std::cout << "addBuffer[" << index << "](" << std::hex << (void*)buffers[index].second << ") " << aStrElement << "\n";
        #endif
        found = true;
        break;
      }  
    }
    if (!found) {
      buffers.push_back({false, NULL});
      index = buffers.size() - 1;
    }  
    
    buffers[index].first = true;
    buffers[index].second = (char*)malloc(aStrElement.size() + 1);
    buffers[index].second[aStrElement.size()] = 0;
    memcpy(buffers[index].second, aStrElement.data(), aStrElement.size());
   
    #ifdef DEBUG_SCRIPAL
    std::cout << "addBuffer[" << buffers.size() - 1 << "](" << std::hex << (void*)buffers.back().second << ") " << aStrElement << "\n";
    #endif
    return(index);
  }

  // add a string to the objects buffer and get reference to element in buffer
  std::size_t addBuffer(const std::string &aStrElement, const char** aStr, int* aStrLen) {
    std::size_t result, index;
    if ((relocateBuffer) && (*aStr != NULL)) {
      bool found = false;
      for (index = 0; index < buffers.size(); ++index) {
        if ((buffers[index].second == *aStr) && (buffers[index].first == true)) {
          buffers[index].second = (char*)realloc((void*)buffers[index].second, aStrElement.size() + 1);
          memcpy(buffers[index].second, aStrElement.data(), aStrElement.size());
          buffers[index].second[aStrElement.size()] = 0;
          result = index;
          found = true;
        };
      }  
      if (!found)
        result = addBuffer(aStrElement);
    } else {
      result = addBuffer(aStrElement);
    }  
    *aStr = buffers[result].second;
    *aStrLen = (int)aStrElement.size(); 
    return(result);
  }

  // delete a string with given memory in objects buffer
  // if managed...else ignore pointer, memory will be deleted in destructors or else   
  void clearBuffer(const char* aStr) {
    for (std::size_t index = 0; index < buffers.size(); ++index) {
      if (buffers[index].second == aStr) {
        #ifdef DEBUG_SCRIPAL
        std::cout << "clearBuffer[" << index << "](" << std::hex << (void*)buffers[index].second << ") " << std::string(buffers[index].second) << "\n";
        #endif   
        buffers[index].first = false;
        free(buffers[index].second);
        buffers[index].second = NULL;
        return;
      };
    };
    // not found in buffer
    std::stringstream ss;
    ss << std::hex << (void*)aStr;
    setError(ERR_SYSTEM + ", address(" +  ss.str() + ") not found in buffer");
  };
};

// class for managed Scripal objects
class TFManagedObj {
  public:
  TFManagedObj() {};
  std::unique_ptr<TFScripal> obj;                     // underlying Scripal object
};

// **********************************************
// class for iterating over files
// **********************************************
class TFFileIterate {
  public:
  std::filesystem::directory_iterator* iterator = nullptr;
  std::filesystem::recursive_directory_iterator* iteratorRec = nullptr;
  const std::filesystem::directory_entry* entry = nullptr;
  bool recursive = false;    
  bool error = true;
  std::string dirExtension = "*";
  
  ~TFFileIterate() { delete iterator; delete iteratorRec;};
  
  // set path , specify file extensions to scan or '*' for all files and true for recursive dir scans  
  TFFileIterate(const std::string &aPath, const std::string &aDirExtension, bool aRecursive) {
    std::error_code ec;
    if (aDirExtension == "")
      dirExtension = "*";
    else  
      dirExtension = aDirExtension;

    recursive = aRecursive;

    if (recursive)
      iteratorRec = new std::filesystem::recursive_directory_iterator(aPath, ec);
    else 
      iterator = new std::filesystem::directory_iterator(aPath, ec);

    error = (ec.value() != 0);
  };
  
  void iterate() {
    std::error_code ec;
    ec.clear();
    entry = nullptr;
    bool found = false;
 
    if (error) 
      return;
      
    while (!found) {
      if (recursive) {
        iteratorRec->increment(ec);
        if (end(*iteratorRec) == *iteratorRec) {
          error = true; 
        }
      } else { 
        iterator->increment(ec); 
        if (end(*iterator) == *iterator) {
          error = true; 
        }
      };
      
      if (ec.value() != 0) 
        error = true;
      
      if (error) 
        return;
    
      if (recursive) 
        entry = &**iteratorRec;
      else   
        entry = &**iterator;
        
      if(entry->is_directory())
        continue;
        
      std::string fileName = entry->path().string();
      if (( fileName.find("." + dirExtension) !=  nPos) || (dirExtension == "*")) 
        found = true;        
    };    
  };
};

bool isBigEndianTest();
void swapBytes(char32_t &aVal);
std::string homeDir();
void clearError();
void log(const std::string &aMessage, std::size_t aPos1 = nVal, std::size_t aPos2 = nVal);
void output(const std::string &aMessage);
void output(const std::string &aMessage, int aEncoding);

std::string printOpr(TFOperator& aOpr);
std::string printOpd(TFOperand& aOpd);
std::string printNumber(TFNum aNumber, bool aAttrs[_SP_ATTR_END]);
std::string printPosition(std::size_t aPos);
std::string printPosition(int aPos);
std::string printAttr(TFOperand &aOpd, std::size_t aAttr);
std::string printAllAttrs(TFOperand &aOpd);
std::string printCode(std::vector<TFOperator> &code);
std::string printBytesHex(const std::string &aString);

int getEncID(const std::string &aEncoding);
int getFileEncoding(std::string &aPath);

std::vector<std::string> getFiles(const std::string &aPath, const std::string &aExtension, bool aRecursive);
void printFiles(const std::string &aPath, const std::string &aDirExtension, bool aRecursive);

int posToInt(std::size_t aPos);
const std::string& findIdentifier(uint16_t aType, uint16_t aCodeID);
int findIdentifier(uint16_t aType, const std::string &aIdentifier);
bool inRange(TFNum aValue, TFNum aMin, TFNum aMax);
bool logicOpd(std::size_t aCodeID);  
bool blockOpd(std::size_t aCodeID);   
bool textOpd(std::size_t aCodeID);   
bool nameOpd(std::size_t aCodeID);    
bool numberOpd(std::size_t aCodeID);  
bool rangeOpd(std::size_t aCodeID);  

std::string anyResultsToHRF(TFResults &aResults, int aEncoding); 
std::string anyResultsToCSV(TFResults &aResults, int aEncoding);
std::string anyResultsToJSON(TFResults &aResults, int aEncoding); 

std::string examplesToJSON(const std::vector<TFExmpl> aExamples, int aEncoding);

void encodeResults(const std::string &aOrgText, const std::string &aUTF8Text, TFScripal& aObj, int aEncoding);

std::string decodeFile(const std::string &aFile);
void initConfig(const std::string aPath, int aEncoding = ENC_DEFAULT);

bool callSystem(const std::string &aCommand, const std::string &aParameters);
void resetTime();
std::chrono::time_point<std::chrono::steady_clock> recTime() ;
std::size_t printTime( std::chrono::time_point<std::chrono::steady_clock> aSTime);
std::size_t stopTime( std::chrono::time_point<std::chrono::steady_clock> aSTime);
}; // end of namespace

#endif