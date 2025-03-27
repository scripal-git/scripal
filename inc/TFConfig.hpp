// ***************************************************************************
// Scripal configuration
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFCONFIG_HPP_INCLUDED
#define TFCONFIG_HPP_INCLUDED

namespace Scripal {
  
class  TFConfig {
  private:

  std::string copyJSON = "";                                  // copy of object as JSON

  public:
  
  bool debugCompile = false;                                  // compile source with debug option
  bool debugRun = false;                                      // run code with debug option
  bool showCode = false;                                      // show compiled code 
  bool measureTime = false;                                   // if true measure time in milliseconds
  bool useEmpty = false;                                      // use empty results in replace/split operation, true/false
  bool translateDigits = false;                               // if true translate digits in foreign languages to '0','1'... 
  bool verboseResult = false;                                 // if true, make result verbose
  
  char32_t decimalPoint = '.';                                // decimal point
  char32_t thousandsSep = ',';                                // thousands separator
  char32_t posSign = '+';                                     // mathematical plus sign
  char32_t negSign = '-';                                     // mathematical minus sign

  int encoding = ENC_DEFAULT;                                 // default encoding used in environment, set to ENC_DEFAULT for auto setting 
  int logEncoding = ENC_DEFAULT;                              // default encoding used in logs, set to ENC_DEFAULT for auto setting 
  int maxFileSize = 1000;                                     // maximum size of file to load in MB if not UTF-8 encoded 
  int patternNearest = PATTERN_LEVENPLUS_WORD;                // default pattern for nearest search
  int patternBlock = PATTERN_JARO;                            // default pattern for block match
  int positionType = POS_UTF8;                                // result position type, UTF-8 or nan for non-UTF8 text

  std::string logChannel = "default";                         // log channel to use, stdout, buffer or a file
  #ifdef LINUX
  std::string pdfReader = "pdftotext";                        // entire path to PDF parser , default: pdftotext
  std::string frmReader = "pandoc";                           // entire path to multi parser , default: pandoc
  std::string appPath = "/usr/lib/scripal";                   // entire path to application data
  #endif
  #ifdef WINDOWS
  std::string pdfReader = "pdftotext";                        // entire path to PDF parser , default: pdftotext
  std::string frmReader = "pandoc";                           // entire path to multi parser , default: pandoc
  std::string appPath = "..\\..\\";                           // entire path to application data
  #endif

  std::vector<std::string> sentenceEnd =  {".", "!", "?"};    // sentence end । in Hindi etc.
  std::vector<std::string> separators  =  {" ", ".", "!", "?", ",", ";", ":", "/", "(", ")", "[", "]", "{", "}"};     // word separators to identify word
  std::vector<std::string> abbreviations;                      // abbreviations to distinguish word and end of sentence
  
  LIB_EXPORT std::string toJSON(std::vector<std::string> aVec);
  LIB_EXPORT bool fromJSON(const std::string &aJSON);
  LIB_EXPORT bool store(const std::string &aFile);
  LIB_EXPORT bool restore(const std::string &aFile);
  LIB_EXPORT bool setTemp(const std::string &aJSON);
  LIB_EXPORT bool resetTemp();
 
  // set language (specifiyng number format)
  LIB_EXPORT bool setLocale(const std::string &aLang) {  std::setlocale(LC_ALL, std::string(aLang + ".UTF-8").c_str());
    decimalPoint = (char32_t)std::use_facet< std::numpunct<char> >(std::cout.getloc()).decimal_point();
    thousandsSep = (char32_t)std::use_facet< std::numpunct<char> >(std::cout.getloc()).thousands_sep();
    return(true);    
  };
  
  // C++ getters/setters for different values
  LIB_EXPORT bool set(const std::string &aKey, bool aValue);
  LIB_EXPORT bool get(const std::string &aKey, bool &aValue);
  LIB_EXPORT bool set(const std::string &aKey, char32_t aValue);
  LIB_EXPORT bool get(const std::string &aKey, char32_t &aValue);
  LIB_EXPORT bool set(const std::string &aKey, int aValue);
  LIB_EXPORT bool get(const std::string &aKey, int &aValue);
  LIB_EXPORT bool set(const std::string &aKey, const std::string &aValue);
  LIB_EXPORT bool get(const std::string &aKey, std::string &aValue);
  LIB_EXPORT bool set(const std::string &aKey, const std::vector<std::string> &aValue);
  LIB_EXPORT bool get(const std::string &aKey, std::vector<std::string> &aValue);
  LIB_EXPORT bool get(const std::string &aKey, int aIndex, std::string &aValue);
  LIB_EXPORT bool clear(const std::string &aKey);
  LIB_EXPORT bool add(const std::string &aKey, const std::string &aValue);
  
  LIB_EXPORT std::string read(std::string &aSource);
};

}; // end of namespace
#endif
