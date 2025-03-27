// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "general.hpp"
#include "TFAccess.hpp"

namespace Scripal {

// ****************************************************************************
// destruct
// ****************************************************************************
TFAccess::~TFAccess() {
  reset();
} 
  
// ****************************************************************************
// initialize object
// ****************************************************************************
void TFAccess::reset() {
  if (accessType == M_STRING) { 
    if (owner) 
      delete(mString); 
  } else if (accessType == M_FILE) {
    if (mFile != nullptr)
      mFile->close(); 
      delete(mFile); 
  };
  
  accessType = M_STRING; 
  mString = nullptr;
  mFile = nullptr; 
}

// ****************************************************************************
// assign char string to access object
// aText  - text string
// return - true on success
// ****************************************************************************
void TFAccess::assignString(const char* aText) { 
  reset();
  
  if (aText == nullptr) {
    setError(ERR_UNKNOWN_IDENTIFIER + ", cannot assign null pointer"); 
    return; 
  }

  accessType = M_STRING; 
  std::string* buffer = new std::string(aText);
  mString = buffer;
  mSize = mString->size(); 
  owner = true;
  return;
}

// ****************************************************************************
// assign text string to access object
// aText - text string
// return - true on success
// ****************************************************************************
void TFAccess::assignString(const std::string* aText) { 
  reset();

  if (aText == nullptr) {
    setError(ERR_UNKNOWN_IDENTIFIER + ", cannot assign null pointer"); 
    mString = nullptr; 
    return; 
  }
  
  accessType = M_STRING; 
  mString = aText;
  mSize = aText->size(); 
  owner = false;
  return;
}

// ****************************************************************************
// assign file to access object
// aFile - file path
// ****************************************************************************
void TFAccess::assignFile(const std::string &aFile) {
  reset();
  
  mFile = new std::fstream(aFile, std::ios_base::in | std::ios_base::binary);
  if (!mFile->is_open()) {
    setError(ERR_FILE + ", cannot open file '" + aFile + "'"); 
    delete(mFile);
    mFile = nullptr; 
    return; 
  }

  accessType = M_FILE; 

  const auto begin = mFile->tellg();
  mFile->seekg (0, std::ios::end);
  const auto end = mFile->tellg();
  mSize = (end - begin);
  
  // fill cache
  fillCache(0, CACHE_SIZE);
  
  owner = false;
  return;
}    

// ****************************************************************************
// return size of text media 
// return - size in bytes
// ****************************************************************************
std::size_t TFAccess::size() {
  return(mSize);
}  

// ****************************************************************************
// fill cache for given position
// aPos     - position
// aMinSize - minimum cache size
// ****************************************************************************
void TFAccess::fillCache(std::size_t aPos, std::size_t aMinSize) { 
  std::size_t cacheSize;
  #ifdef DEBUG_SCRIPAL
  std::cout << "fill file cache at: " << aPos << " , size: " << aMinSize << " bytes\n";
  #endif

  // set cache confiǵuration
  if (aPos >= CACHE_OFFSET)
    cacheBegin = aPos - CACHE_OFFSET;
  else 
    cacheBegin = 0;

  if (aMinSize > CACHE_SIZE)
    cacheEnd = cacheBegin + aMinSize - 1;
  else
    cacheEnd = cacheBegin + CACHE_SIZE - 1;
  
  if (cacheEnd >= size())
    cacheEnd = size() - 1;

  cacheSize = cacheEnd - cacheBegin + 1;
  
  cache.resize(cacheSize);
  mFile->clear();
  mFile->seekg(cacheBegin);
  mFile->read((char*)cache.c_str(), cacheSize);

  if ((mFile->rdstate() != std::ios_base::goodbit)) {
    // error
    setError(ERR_FILE + ", cannot access position (" + std::to_string(aPos) + ") in file");
    return;
  };  
}
  
// ****************************************************************************
// access single byte in media (unsafe! access not error or exception handling)
// aPos   - position
// return - char read
// ****************************************************************************
char TFAccess::get(std::size_t aPos) { 
  if (accessType == M_STRING) { 
    return((*mString)[aPos]); 
  } else { 
    if ((aPos < cacheBegin) || (aPos > cacheEnd)) {
      fillCache(aPos, CACHE_SIZE);
    }
    return(cache[aPos - cacheBegin]); 
  } 
}

// ****************************************************************************
// read entire file and allocate buffer
// return - string buffer
// ****************************************************************************
std::string TFAccess::read() {
  // read file at once
  std::string result;
  result.resize(size());
  mFile->clear();
  mFile->seekg(0);
  mFile->read((char*)result.c_str(), size()); 
  return(result);   
}

// ****************************************************************************
// return substr in text media 
// aPos   - position in media
// aLen   - no. of bytes to read
// return - string with parts of media content
// ****************************************************************************
std::string TFAccess::substr(size_t aPos, size_t aLen) {
  clearError();
  if (accessType == M_STRING) {
    return(mString->substr(aPos, aLen));
  } else {
    if ((aPos < cacheBegin) || ((aPos + aLen -1) > cacheEnd)) {
      fillCache(aPos, aLen + CACHE_OFFSET);
      if (errorMsg != ERR_NONE) {
        return("");
      }
    };
    
    return(cache.substr(aPos - cacheBegin, aLen));
  }    
}

// ****************************************************************************
// find string in media
// aStr   - string to search for
// aPos   - start position in media, where to begin
// return - position
// ****************************************************************************
std::size_t TFAccess::find(const std::string& aStr, std::size_t aPos) {
  if (accessType == M_STRING) 
    return(mString->find(aStr, aPos));

  if (accessType != M_FILE) 
   return(nPos); 

  std::size_t location = aPos; 
  std::size_t findLoc = nPos; 
  std::size_t fileSize = size(); 

  while (location < fileSize) {
    if ((location >= cacheBegin) && (location <= (cacheEnd - aStr.size() + 1))) {
      findLoc = cache.find(aStr, location - cacheBegin);
      if (findLoc != nPos) {
        #ifdef DEBUG_SCRIPAL
        std::cout << "quick find at position: " << location + findLoc << "\n";
        #endif
        return(cacheBegin + findLoc);
      } else {
        location = cacheEnd  - aStr.size() + 2;  
      }  
    }
    if ((location + aStr.size()) <= fileSize)
      fillCache(location, CACHE_SIZE);
    else 
      return(nPos);  
  }

  return(nPos);
}


}