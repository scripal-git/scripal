// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "general.hpp"
#include "algos.hpp"
#include "text.hpp"
#include "TFAccess.hpp"
#include "TFCompile.hpp"
#include "TFScripal.hpp"
#include "scripalLib.hpp"

namespace Scripal {
  
// ****************************************************************************
// unit test for positioning in text
// ****************************************************************************
void TFScripal::unitTestPos() {
  TFOperand opd;
  std::size_t pBegin, pEnd; 
  reset();
  
  resultBegin = 0; resultEnd = nPos;
  pBegin = nPos; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = nPos; resultEnd = 10;
  pBegin = 0; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");

  resultBegin = 0; resultEnd = 8;
  pBegin = 9; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = 0; resultEnd = nPos;
  pBegin = 2; pEnd = 7; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = 7; resultEnd = 10;
  pBegin = 4; pEnd = 11; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = nPos; resultEnd = nPos;
  pBegin = nPos; pEnd = 7; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = nPos; resultEnd = nPos;
  pBegin = 1; pEnd = nPos; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
  
  resultBegin = 2; resultEnd = nPos;
  pBegin = 3; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");

  resultBegin = 2; resultEnd = nPos;
  pBegin = nPos; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");

  resultBegin = nPos; resultEnd = nPos;
  pBegin = 1; pEnd = 10; 
  output("pre:   resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + "\n");
  addPosMatch(pBegin,pEnd,opd);
  output("after: resultBegin:" + printPosition(resultBegin) + ", resultEnd:" + printPosition(resultEnd) + ", pBegin:" + printPosition(pBegin) + ", pEnd:" + printPosition(pEnd) + "\n\n");
}
  
}