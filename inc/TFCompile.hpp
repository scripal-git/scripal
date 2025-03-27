// **********************************************
// Scripal compiler object
// **********************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef TFCOMPILE_INCLUDED
#define TFCOMPILE_INCLUDED

#include "definitions.hpp"

namespace Scripal {

class TFCompile {
  private: 
  friend TFScripal;
 
  std::bitset<_SP_OPD_END> expectedOpd = SP_OPD_NULL;  // expected operand type or NULL
  bool debug = false;                       // output debug information?  
  TFScripal* output = nullptr;              // generated Scripal object
  std::string source;                       // source code
  std::size_t posStart = 0, posEnd = nPos;  // start/end position of current element in source, posEnd = nPos if not element found
  std::size_t posOpr = 0;                   // start/end position of current operator in source
  bool operandOpen = false;                 // is single operand block (attribute(s) + operand) open?
  bool operatorOpen = false;                // is operator open?
  bool newGroup = false;                    // if true -> next operand will be in new group
  bool firstOpd = false;                    // if true -> first operand of operator  
  std::size_t lastBlock = nPos;             // start of last block
  std::vector<std::size_t> opdPath;         // operand path in Scripal , index zero is top operand 
  std::vector<std::size_t> blocks ;         // index into output's operators marking begin of blocks
  
  TFCompile (const std::string &aSource, TFScripal &aOutput); 

  bool run();

  void debugOn() { debug = true;};
  void debugOff() {debug = false;};

  void insertTemplates(const std::string &aSource);

  TFOperand& setOperand();
  void specialOperator();
  TFOperator& setOperator(const TFIndentifier&  aOperator);
  void assignOperator(const TFIndentifier&  aOperator);
  TFOperand* specifyOperand(std::size_t aCodeID);
  void assignOperand(const TFIndentifier&  aOperand);
  void closeOperand();
  void closeOperator();

  std::vector<TFOperand>& startGroup();
  void endGroup();
  void startBlock();
  void endBlock();

  void setAtom(const TFIndentifier &aAtom);
  void setControl(const TFIndentifier& aControl);
  void setCondition(const TFIndentifier&  aCondition);
  void setAttribute(const TFIndentifier&  aAttribute);

  std::size_t findAnyNumber(std::size_t aPosStart);

  void setNumber(const std::string &aText);

  std::size_t findText(std::size_t aPosStart, const std::string &aCtrlText);
  void setText(const std::string &aText);

  std::size_t findRange(std::size_t aPosStart);
  void setRange(const std::string &aRange);
  
  std::size_t findName(std::size_t aPosStart);
  void setName(const std::string &aName);

  void checkOprOpen();
  
  std::string extractContent(std::size_t aPosStart, std::size_t aPosEnd, const std::string &aDelimiter);

  TFOperator& accessOperator();
  TFOperand& accessOperand();
  TFOperand& accessParent();
  TFOperand& accessPath();
  std::vector<TFOperand>& accessGroup();
};

}; // end of namespace
#endif