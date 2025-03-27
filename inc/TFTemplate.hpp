// **********************************************
// main template object
// **********************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFTEMPLATE_INCLUDED
#define TFTEMPLATE_INCLUDED

namespace Scripal {

class TFTemplate {
  public:
  std::recursive_mutex templateMutex;                         // mutex for multi-threaded access
  
  bool clear(const std::string &aName);
  void reset();
  std::string get(const std::string &aName);
  bool set(const std::string &aName, const std::string &aSource);
  bool store(const std::string &aFile); 
  bool restore(const std::string &aFile);
  std::string toJSON(std::vector<std::string> aVec);
  bool fromJSON(const std::string &aJSON);

  void read(std::string &aSource);
  bool apply(std::string &aSource, std::vector<std::string> &aArgs);
  private:
  std::map<std::string, std::string> sources;  
  std::array<std::size_t, 2> getBlock(const std::string &aSource, std::size_t aIndex, char32_t aStartCP, char32_t aEndCP, std::vector<char32_t> aFinalCP);
};

extern TFTemplate templates;

}; // end of namespace

#endif