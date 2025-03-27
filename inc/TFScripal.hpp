// **********************************************
// main Scripal object
// **********************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFSCRIPAL_INCLUDED
#define TFSCRIPAL_INCLUDED

#include "TFAccess.hpp"
#include "TFConfig.hpp"

namespace Scripal {

class  TFScripal {
  public: 
  int matchType = MATCH_SOURCE;                           // match type (source is default)
  bool hasMatched = false;                                // any match at all so far or in last run?
  long double processTime = 0;                            // process duration in ms  
  
  TFResults results;                                      // results

  bool isResult();
  bool isSource();
  bool isNearest();
  bool isBlock();

  TFScripal(TFScripal& aObj);

  void initialize(const std::string &aSource, int aEncoding = ENC_DEFAULT);
  LIB_EXPORT TFScripal();
  LIB_EXPORT TFScripal(const std::string &aSource, double aDist, int aMatchType, int aEncoding = ENC_UTF8);
  LIB_EXPORT TFScripal(const std::string &aSource, int aEncoding = ENC_UTF8);

  LIB_EXPORT bool match(const std::string &aText, int aEncoding = ENC_UTF8);
  LIB_EXPORT bool matchFile(const std::string &aText, int aFileEncoding = ENC_DEFAULT);

  LIB_EXPORT bool replace(const std::string &aText, const std::string &aReplace, int aEncoding = ENC_UTF8);
  LIB_EXPORT bool replaceFile(const std::string &aFileIn, const std::string &aFileOut, const std::string &aReplace, int aFileEncoding = ENC_DEFAULT);

  LIB_EXPORT bool split(const std::string &aText, int aEncoding = ENC_UTF8);
  LIB_EXPORT bool splitFile(const std::string &aFileIn, const std::string &aFileOut, int aFileEncoding = ENC_UTF8);
  LIB_EXPORT bool fileSearch(const std::string &aPath, const std::string &aDirExtension, bool aRecursive);
 
  std::string resultsToHRF(int aEncoding = ENC_UTF8); 
  std::string resultsToCSV(int aEncoding = ENC_UTF8);
  std::string resultsToJSON(int aEncoding = ENC_UTF8); 

  private:  

  std::vector<TFOperator> code;                                  // compiled code
 
  std::vector<std::pair<std::string, std::string>> names;        // names used in object and result text

  std::map<std::size_t, TFOperand*> unlessPos;                   // position for unless attribute

  std::string jsonConfig;                                        // if special config is used -> temporary config as JSON 

  friend class TFTemplate;
  friend class TFCompile;

  TFNum patternDist = 1;                                         // critical distance for nearest or block match to give result
  std::string source;                                            // source for pattern matching or preprocessed source
 
  bool debug = false;                                            // output debug information?  (default is off)

  bool hasMatch = false;                                          // last operator matched? 
  bool runElse = false;                                           // if true, run else branch if it exists

  bool nameOn = false;                                            // named ouput 
  bool nameAdd = false;                                           // named and result ouput 

  TFAccess text;                                              // text media

  std::size_t posPtr = 0;                                     // current scan pointer, always at start of codepoint
  std::size_t resultBegin = 0;                                // pointer to start of result region, always at start of codepoint
  std::size_t resultEnd = 0;                                  // pointer to end of result region, always at end of codepoint
  std::string curName = "";                                   // current name
  std::string curTag = "";                                     // current tag
  std::size_t instPtr = 0;                                    // instruction pointer
  std::size_t lastInstPtr = 0;                                // previous instruction pointer
  std::size_t notCount = 0;                                   // count up for every nested 'not' attribute
  TFOperand* lastMatch = nullptr;                             // last operand that matched
  
  void unitTestPos();

  bool process(); 

  void debugOn() { debug = true;};
  void debugOff() {debug = false;};

  void reset();

  void matchBlock(TFOperand& aOpd);
  void matchOpd(TFOperand& aOpd, TFOperand& aParentOpd);
  void matchLogical(TFOperand& aOpd);
  bool posToText(TFOperand& aOpd, std::size_t aStart, std::size_t aEnd);
  void matchText(TFOperand& aOpd);
  void matchNumbers(TFOperand& aOpd);
  
  bool checkCondition(uint16_t aCondition);

  uint16_t logicalAll(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalAny(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalNot(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalEach(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalEvery(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalFind(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalFindAt(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalBreak(TFOperand& aOpd, TFOperand& aParentOpd);  
  uint16_t logicalRepeat(TFOperand& aOpd, TFOperand& aParentOpd);  
  uint16_t logicalIsNumber(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalIsWord(TFOperand& aOpd, TFOperand& aParentOpd);
  uint16_t logicalIsCase(TFOperand& aOpd, TFOperand& aParentOpd, uint8_t aCase);
  
  void parse();
  void multiParse(std::size_t &aLastPosPtr);
  void jump(std::size_t aInstr, bool aRunElse);
  void incInstr();
  void invalidInstr();
  void beginResult(std::size_t aPos);
  std::string prettyResult(std::size_t aPos, std::size_t aLen); 
  void endResult(std::size_t aPos);
  void resetResult();
  void setName();
  void setNameAdd();
  void setTag();

  void saveState(TFState &aState, TFOperand &aOpd);
  void restoreState(TFState &aState, TFOperand &aOpd);
  
  void setMatch(uint16_t aResult);
  void storeResult(std::size_t aBegin, std::size_t aEnd, const std::string& aResult, const std::string& aTag);
  void clearResult();
  void addNegMatch(std::size_t aPos, TFOperand &aOpd);
  void addPosMatch(TFOperand &aOpd);
  void addPosMatch(std::size_t aBegin, std::size_t aEnd, TFOperand &aOpd);
  void setPos(std::size_t aPos); 
  void setLogicPos(std::size_t aPos, TFOperand &aOpd);
  void movePos(TFNum aCount, std::size_t &aPos);
  
  std::string &findName(const std::string& aName); 
  void debugMatch(TFOperand &aOpd, bool aType);
};

extern TFScripal nullScripal;

}; // end of namespace
#endif
