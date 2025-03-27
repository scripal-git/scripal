// ***************************************************************************
// functions related to character encodings
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef ENCODING_HPP_INCLUDED
#define ENCODING_HPP_INCLUDED

#include "definitions.hpp"

// export under namespace Scripal
namespace Scripal {
bool convertDigit(char32_t& aCodePoint);
void convertStrDigits(std::string &aString);
  
isInline int getCPSizeUTF8(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeUTF16L(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeUTF16B(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeUTF32L(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeUTF32B(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeASCII(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeCP932(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeCP936(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeCP949(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeCP950(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeLATIN1(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeLATIN2(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeLATIN9(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN874(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1250(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1251(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1252(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1253(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1254(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1255(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1256(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1257(const std::string &aText, const std::size_t aIndex);
isInline int getCPSizeWIN1258(const std::string &aText, const std::size_t aIndex);

isInline char32_t getCPUTF8(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPUTF16L(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPUTF16B(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPUTF32L(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPUTF32B(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPASCII(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPCP932(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPCP936(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPCP949(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPCP950(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPLATIN1(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPLATIN2(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPLATIN9(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN874(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1250(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1251(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1252(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1253(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1254(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1255(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1256(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1257(const std::string &aText, const std::size_t aIndex);
isInline char32_t getCPWIN1258(const std::string &aText, const std::size_t aIndex);

int getCPUTF8(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPUTF16L(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPUTF16B(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPUTF32L(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPUTF32B(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPASCII(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPCP932(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPCP936(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPCP949(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPCP950(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPLATIN1(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPLATIN2(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPLATIN9(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1250(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN874(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1251(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1252(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1253(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1254(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1255(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1256(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1257(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);
int getCPWIN1258(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint);

isInline std::string toStringUTF16L(char32_t aCodePoint);
isInline std::string toStringUTF16B(char32_t aCodePoint);
isInline std::string toStringUTF32L(char32_t aCodePoint);
isInline std::string toStringUTF32B(char32_t aCodePoint);
isInline std::string toStringASCII(char32_t aCodePoint); 
isInline std::string toStringCP932(char32_t aCodePoint); 
isInline std::string toStringCP936(char32_t aCodePoint); 
isInline std::string toStringCP949(char32_t aCodePoint); 
isInline std::string toStringCP950(char32_t aCodePoint); 
isInline std::string toStringLATIN1(char32_t aCodePoint); 
isInline std::string toStringLATIN2(char32_t aCodePoint); 
isInline std::string toStringLATIN9(char32_t aCodePoint); 
isInline std::string toStringWIN874(char32_t aCodePoint); 
isInline std::string toStringWIN1250(char32_t aCodePoint); 
isInline std::string toStringWIN1251(char32_t aCodePoint); 
isInline std::string toStringWIN1252(char32_t aCodePoint); 
isInline std::string toStringWIN1253(char32_t aCodePoint); 
isInline std::string toStringWIN1254(char32_t aCodePoint); 
isInline std::string toStringWIN1255(char32_t aCodePoint); 
isInline std::string toStringWIN1256(char32_t aCodePoint); 
isInline std::string toStringWIN1257(char32_t aCodePoint); 
isInline std::string toStringWIN1258(char32_t aCodePoint); 

std::u32string utf8ToUtf32(const std::string &aString);
std::string utf8ToEnc(std::string_view aString, int aEncode);
std::string encToUtf8(std::string_view aString, int aEncode);
std::string encSrcToDst(std::string_view aString, int aEncodeSrc, int aEncodeDst);
std::string encSrcToDst(const char* aText, std::size_t aTextLen, int aEncodeSrc, int aEncodeDst);
int getBestEncoding(int aEncoding);
int getCP(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint, int aEncoding);

}; // end of namespace

#endif