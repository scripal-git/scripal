// ***************************************************************************
// algorithms used in Scripal
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef ALGOS_HPP_INCLUDED
#define ALGOS_HPP_INCLUDED

#include "general.hpp"

// export under namespace Scripal
namespace Scripal {

TFNum levenDistance(const std::string &aStr1, const std::string aStr2);
TFNum levenPlusDistance(const std::string &aStr1, const std::string aStr2); 
bool findNearest(const std::string &aSearch, TFAccess &aText, TFResults &aResults, TFNum aMin);
bool compareBlock(const std::string &aSource, TFAccess &aText, TFResults &aResults, TFNum aMin);
std::string generateString(std::size_t len, const std::string &aChars);
TFNum jaroDistance(const std::string &aStr1, const std::string &aStr2);
TFNum jaroWinklerDistance(const std::string &aStr1, const std::string &aStr2);

}; // end of namespace

#endif