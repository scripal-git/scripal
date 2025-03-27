// **********************************************
// read only access to static media 
// **********************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFACCESS_INCLUDED
#define TFACCESS_INCLUDED

#include "definitions.hpp"

namespace Scripal {

// string used as text
const char M_STRING = 0;  
// file used as text 
const char M_FILE = 1;    

// size of file cache, should be at least 1 KB
const std::size_t CACHE_SIZE = 100000000;
// size of left cache offset 
const std::size_t CACHE_OFFSET = 100;

class  TFAccess {
  public:

  bool owner = false;                       // if true -> object is owner of string 

  char accessType = M_STRING;               // access type: M_STRING or M_FILE
  const std::string* mString = nullptr;     // string used (either remote or self owned)
  std::fstream* mFile = nullptr;            // file used  
  std::size_t mSize = 0;                    // size of media 

  std::size_t cacheBegin = 0;               // position in file where cache starts  
  std::size_t cacheEnd = 0;                 // position in file where cache starts
  std::string cache = "";                   // file cache

  LIB_EXPORT ~TFAccess();
  void fillCache(std::size_t aPos, std::size_t aMinSize);
  char get(std::size_t aPos);
  std::string read();
  std::string substr(std::size_t aPos = 0, std::size_t aLen = nPos); 
  std::size_t find(const std::string& aStr, std::size_t aPos = 0);
  void assignString(const char* aText);
  void assignString(const std::string* aText);
  void assignFile(const std::string &aFile);
  void reset();
  std::size_t size();

}; 

};// end of namespace

#endif