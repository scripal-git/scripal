// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "algos.hpp"
#include "text.hpp"
#include "extern.hpp"

namespace Scripal {

// ****************************************************************************
// is code point a letter? 
// aCodePoint - code point
// return true if letter
// ****************************************************************************
bool isLetter(char32_t aCodePoint) {
  return(Poco::Unicode::isAlpha(aCodePoint));
}

// ****************************************************************************
// is code point alphanumerical? 
// aCodePoint - code point
// return true if alphanumerical
// ****************************************************************************
bool isAlphaNum(char32_t aCodePoint) {
  return(Poco::Unicode::isAlpha(aCodePoint) || Poco::Unicode::isDigit(aCodePoint));
}

// ****************************************************************************
// is code point a digit? 
// aCodePoint - code point
// aAttrs     - attributes controlling if hex or decimal digit ist involved
// return true if digit
// ****************************************************************************
bool isDigit(char32_t aCodePoint, bool aAttrs[_SP_ATTR_END]) {
  static std::set<char32_t> hexChars {'a','A','b','B','c','C','d','D','e','E','f','F','0','1','2','3','4','5','6','7','8','9' };
  static std::set<char32_t> octChars {'0','1','2','3','4','5','6','7'};
  static std::set<char32_t> binChars {'0','1'};
  if (config.translateDigits) convertDigit(aCodePoint);
  
  if (aAttrs[SP_ATTR_HEX]) { 
    if (hexChars.find(aCodePoint) != hexChars.end()) return(true);
     return(false);  
  } else if (aAttrs[SP_ATTR_OCT]) { 
    if (octChars.find(aCodePoint) != octChars.end()) return(true);
     return(false);  
  } else if (aAttrs[SP_ATTR_BIN]) { 
    if (binChars.find(aCodePoint) != binChars.end()) return(true);
     return(false);  
  } else {
    // decimal numbers
    return(Poco::Unicode::isDigit(aCodePoint));
  };
  
  return(false);  
}

// ****************************************************************************
// convert decimal number in string to TFNum type
// aString       - string holding number
// aNum          - converted float
// aDecimalPoint - decimal point
// aThousandsSep - thousands separator
// return true if successfull
// ****************************************************************************
bool textTFNumToNum(const std::string &aString, TFNum &aNum, char32_t aDecimalPoint, char32_t aThousandsSep) {
  try {
    if (config.translateDigits) {
      std::string buffer = aString;
      convertStrDigits(buffer);
      aNum = Poco::NumberParser::parseFloat(buffer, aDecimalPoint, aThousandsSep);
    } else {
      aNum = Poco::NumberParser::parseFloat(aString, aDecimalPoint, aThousandsSep);
    }
  } catch (std::exception &aExp) {
    aNum = NAN;
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert true unsigned integer number in string to TFNum type
// aString - string holding number
// aNum    - converted integer
// return true if successfull
// ****************************************************************************
bool textUIntToNum(const std::string &aString, TFNum &aNum) {
  try {
    if (config.translateDigits) {
      std::string buffer = aString;
      convertStrDigits(buffer);
      aNum = Poco::NumberParser::parseUnsigned64(buffer, config.thousandsSep);
    } else {
      aNum = Poco::NumberParser::parseUnsigned64(aString, config.thousandsSep);
    }
  } catch (std::exception &aExp) {
    aNum = NAN;
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert binary number in string to TFNum type
// aString - string holding number
// aNum    - converted binary
// return true if successfull
// ****************************************************************************
bool textBinToNum(const std::string &aString, TFNum &aNum) {
  std::size_t pos;
  try {
    if (config.translateDigits) {
      std::string buffer = aString;
      convertStrDigits(buffer);
      aNum = stoi(buffer, &pos, 2);
    } else {
      aNum = stoi(aString, &pos, 2);
    }  
    if (pos != aString.size()) {
      aNum = NAN;
      return(false);
    }
  } catch (std::exception &aExp) {
    aNum = NAN;
    return(false);
  }
  return(true);
}

// ****************************************************************************
// convert octal number in string to TFNum type
// aString - string holding number
// aNum    - converted octal
// return true if successfull
// ****************************************************************************
bool textOctToNum(const std::string &aString, TFNum &aNum) {
  try {
    if (config.translateDigits) {
      std::string buffer = aString;
      convertStrDigits(buffer);
      aNum = Poco::NumberParser::parseOct64(buffer);
    } else {
      aNum = Poco::NumberParser::parseOct64(aString);
    }  
  } catch (std::exception &aExp) {
    aNum = NAN;
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert hexadecimal number in string to TFNum type
// aString - string holding number
// aNum    - converted hex
// return true if successfull
// ****************************************************************************
bool textHexToNum(const std::string &aString, TFNum &aNum) {
  try {
    if (config.translateDigits) {
      std::string buffer = aString;
      convertStrDigits(buffer);
      aNum = Poco::NumberParser::parseHex64(buffer);
    } else {
      aNum = Poco::NumberParser::parseHex64(aString);
    }  
  } catch (std::exception &aExp) {
    aNum = NAN;
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert std::size_t number to string with fixed length, zero padded
// aNum   - number 
// aLen   - string length
// return - string
// ****************************************************************************
std::string intToFixText(std::size_t aNum, std::size_t aLen) {
  std::ostringstream ostr;

  ostr << std::setfill('0') << std::setw(aLen) << aNum ;
  return (ostr.str());
}

// ****************************************************************************
// convert number to string in binary representation
// aNum    - number
// aString - string holding number
// return true if successfull
// ****************************************************************************
bool numBinToText(TFNum &aNum, std::string &aString) {
  try {
    aString = std::bitset<128>(abs((int64_t)aNum)).to_string(); 
    while (aString[0] == '0')
      aString.erase(0, 1);
    if (aString == "")
      aString = "0";  
    if (aNum < 0) 
      aString.insert(0, "-");
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert number to string in octal representation
// aNum    - number
// aString - string holding number
// return true if successfull
// ****************************************************************************
bool numOctToText(TFNum &aNum, std::string &aString) {
  try {
    Poco::intToStr<int64_t>((int64_t)aNum, 8, aString, false, -1, ' ', 0);
    if (aNum < 0) 
      aString.insert(0, "-");
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert number to string in hex representation
// aNum    - number
// aString - string holding number
// return true if successfull
// ****************************************************************************
bool numHexToText(TFNum &aNum, std::string &aString) {
  try {
    Poco::intToStr<int64_t>((int64_t)aNum, 16, aString, false, -1, ' ', 0);
    if (aNum < 0) 
      aString.insert(0, "-");
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert number to string in float representation
// aNum    - number
// aString - string holding number
// return true if successfull
// ****************************************************************************
bool numFloatToText(TFNum &aNum, std::string &aString) {
  try {
    aString = "";
    Poco::NumberFormatter::append(aString, aNum);
    if (config.decimalPoint != '.') {
      replacePart(aString, ".", toString(config.decimalPoint));
    } 
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert number to string in float representation
// aNum       - number
// aString    - string holding number
// aPrecision - round to x decimals
// return true if successfull
// ****************************************************************************
bool numFloatToText(TFNum &aNum, std::string &aString, std::size_t aPrecision) {
  try {
    aString = "";
    Poco::NumberFormatter::append(aString, aNum, aPrecision);
    if (config.decimalPoint != '.') {
      replacePart(aString, ".", toString(config.decimalPoint));
    } 
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert number to string in float representation with decimal precision
// aNum       - number
// aString    - string holding number
// aPrecision - precision
// return true if successfull
// ****************************************************************************
bool numFloatToText(TFNum &aNum, std::string &aString, int aPrecision) {
  try {
    aString = "";
    Poco::NumberFormatter::append(aString, aNum, aPrecision);
    if (config.decimalPoint != '.') {
      replacePart(aString, ".", toString(config.decimalPoint));
    } 
  } catch (std::exception &aExp) {
    return(false);
  }  
  return(true);
}

// ****************************************************************************
// convert any number (controlled by attributes) in string to TFNum type
// aString - string holding number
// aNum    - converted number
// aAttrs  - attributes controlling if hex,oct etc. digits are involved
// aDecimalPoint - decimal point
// aThousandsSep - thousands separator
// return true if successfull
// ****************************************************************************
bool textToNum(const std::string &aString, TFNum &aNum, bool aAttrs[_SP_ATTR_END], char32_t aDecimalPoint, char32_t aThousandsSep) {
  if (aAttrs[SP_ATTR_HEX]) {
    return(textHexToNum(aString, aNum));
  } else if (aAttrs[SP_ATTR_OCT]) {
    return(textOctToNum(aString, aNum));
  } else if (aAttrs[SP_ATTR_BIN]) {
    return(textBinToNum(aString, aNum));
  } else {
    return(textTFNumToNum(aString, aNum, aDecimalPoint, aThousandsSep));
  }
  return(false);  
}

// ****************************************************************************
// convert any TFNum number (controlled by attributes) to string
// aNum    - number
// aAttrs  - attributes controlling if hex,oct etc. digits are involved
// return number as string
// ****************************************************************************
bool numToText(TFNum &aNum, std::string &aString, bool aAttrs[_SP_ATTR_END]) {
  if (aAttrs[SP_ATTR_HEX]) {
    return(numHexToText(aNum, aString));
  } else if (aAttrs[SP_ATTR_OCT]) {
    return(numOctToText(aNum, aString));
  } else if (aAttrs[SP_ATTR_BIN]) {
    return(numBinToText(aNum, aString));
  } else {
    return(numFloatToText(aNum, aString));
  }
  return(false);  
}

// ***************************************************************************
// replace all parts of string 
// aText    - string
// aSearch  - substring to find
// aReplace - string to substitute with
// ***************************************************************************
void replacePart(std::string &aText, const std::string &aSearch, const std::string &aReplace) {
  if (aSearch.size() == 0)
    return;
    
  size_t pos = aText.find(aSearch);
  while (pos != nPos) {
    aText.replace(pos, aSearch.size(), aReplace);
    pos = aText.find(aSearch, pos + aReplace.size());
  }
}

// ***************************************************************************
// unescape text 
// aText - string
// ***************************************************************************
void unescape(std::string &aText) {
 replacePart(aText, "\\\\", "\\");  
}

// ***************************************************************************
// show control characters in printable format 
// aText - string
// ***************************************************************************
void printCtrl(std::string &aText) {
 replacePart(aText, "\x00", "\\x00");  
 replacePart(aText, "\x01", "\\x01");  
 replacePart(aText, "\x02", "\\x02");  
 replacePart(aText, "\x03", "\\x03");  
 replacePart(aText, "\x04", "\\x04");  
 replacePart(aText, "\x05", "\\x05");  
 replacePart(aText, "\x06", "\\x06");  
 replacePart(aText, "\a", "\\a");  
 replacePart(aText, "\b", "\\b");  
 replacePart(aText, "\t", "\\t");  
 replacePart(aText, "\n", "\\n");  
 replacePart(aText, "\v", "\\v");  
 replacePart(aText, "\f", "\\f");  
 replacePart(aText, "\r", "\\r");  
 replacePart(aText, "\x0e", "\\x0e");  
 replacePart(aText, "\x0f", "\\x0f");  
 replacePart(aText, "\x10", "\\x10");  
 replacePart(aText, "\x11", "\\x11");  
 replacePart(aText, "\x12", "\\x12");  
 replacePart(aText, "\x13", "\\x13");  
 replacePart(aText, "\x14", "\\x14");  
 replacePart(aText, "\x15", "\\x15");  
 replacePart(aText, "\x16", "\\x16");  
 replacePart(aText, "\x17", "\\x17");  
 replacePart(aText, "\x18", "\\x18");  
 replacePart(aText, "\x19", "\\x19");  
 replacePart(aText, "\x1A", "\\x1A");  
 replacePart(aText, "\x1B", "\\x1B");  
 replacePart(aText, "\x1C", "\\x1C");  
 replacePart(aText, "\x1D", "\\x1D");  
 replacePart(aText, "\x1E", "\\x1E");  
 replacePart(aText, "\x1F", "\\x1F");  
}

// ***************************************************************************
// trim string
// aText  - string
// return - converted text
// ***************************************************************************
std::string trimText(const std::string &aText) {
  return(Poco::trim(aText));
}

// ***************************************************************************
// trim argument from enclosing "
// aText  - string
// return - converted text
// ***************************************************************************
std::string trimArg(const std::string &aText) {
  std::string result = aText;
  if (result[0] == '"')
    result.erase(0,1);
  if (result.back() == '"')
    result.pop_back();
  return(result);  
}

// ***************************************************************************
// convert string to lower case characters
// aText  - string
// return - converted text
// ***************************************************************************
std::string lowerText(const std::string &aText) {
  return(Poco::UTF8::toLower(aText));
}

// ***************************************************************************
// convert  convert string to upper case characters
// aText  - string
// return - converted text
// ***************************************************************************
std::string upperText(const std::string &aText) {
  return(Poco::UTF8::toUpper(aText));
}

// ***************************************************************************
// test if string is all lower case
// aText - string
// return - true if all lower
// ***************************************************************************
bool isLowerText(const std::string &aText) {
  return(Poco::UTF8::toLower(aText) == aText);
}

// ***************************************************************************
// test if string is all upper case
// aText - string
// return - true if all upper
// ***************************************************************************
bool isUpperText(const std::string &aText) {
  return(Poco::UTF8::toUpper(aText) == aText);
}

// ***************************************************************************
// compare two strings by given method case sensitive, longer string is bigger
// aText1   - first string
// aText2   - second string
// return   - COMPARE_EQUAL, COMPARE_LESS, COMPARE_GREATER
// ***************************************************************************
uint8_t compareText(const std::string &aText1, const std::string &aText2) {
  int cmp = aText1.compare(aText2);
  if (cmp == 0)
    return(CMP_EQU);
  else if(cmp < 0)   
    return(CMP_LSS);
  else   
    return(CMP_GTR);
}
 
// ***************************************************************************
// compare two strings by given method case insensitive
// aText1   - first string
// aText2   - second string
// return   - COMPARE_EQUAL, COMPARE_LESS, COMPARE_GREATER
// ***************************************************************************
uint8_t compareTextICase(const std::string &aText1, const std::string &aText2) {
  int cmp = Poco::UTF8::icompare(aText1, aText2);
  if (cmp == 0)
    return(CMP_EQU);
  else if(cmp < 0)   
    return(CMP_LSS);
  else   
    return(CMP_GTR);
  
  return(CMP_GTR);
}
 
// ***************************************************************************
// compare two collation keys
// aCollation1 - first string
// aCollation2 - second string
// return      - COMPARE_EQUAL, COMPARE_LESS, COMPARE_GREATER
// ***************************************************************************
uint8_t compareCollation(const std::string& aCollation1, const std::string& aCollation2) {
  int cp = aCollation1.compare(aCollation2);
  if (cp < 0)
    return(CMP_LSS);
  else if (cp == 0)  
    return(CMP_EQU);
  else   
    return(CMP_GTR);
}

// ***************************************************************************
// find a string in a string vector
// aVec     - vector holding string
// aString  - input string
// return   -  nPos if not found, else index
// ***************************************************************************
std::size_t findText(std::vector<std::string> &aVec, const std::string &aString) {
  for(std::vector<std::string>::iterator it = aVec.begin(); it < aVec.end(); it++) {
    if (compareText(*it, aString) == CMP_EQU)
      return it - aVec.begin();
  }
  return (nPos);
}

// ***************************************************************************
// get a UTF-8 code point size in string
// aText  - string
// aIndex - byte index 0..
// return - size in bytes needed for storage or 0 for no or invalid character
// ***************************************************************************
int getCPSize(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF8Encoding utf8;
  int nBytes = utf8.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) 
    return(0);
  else  
    return(nBytes);
}

// ***************************************************************************
// get a UTF-8 code point size in media
// aText  - text media
// aIndex - byte index 0..
// return - size in bytes needed for storage or 0 for no or invalid character
// ***************************************************************************
int getCPSize(TFAccess &aText, const std::size_t aIndex) {
  if (aText.accessType == M_STRING) {
    return(getCPSize(*aText.mString, aIndex));
  } else {
    return(getCPSize(aText.substr(aIndex, 4), 0));
  }
}

// ***************************************************************************
// return UTF-8 codepoint at given byte index
// aText   - input string
// aIndex  - byte index 0..
// return  - codepoint
// ***************************************************************************
char32_t getCP(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF8Encoding utf8;
  char32_t result = utf8.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}

// ***************************************************************************
// return UTF-8 codepoint at given byte index in media
// aText   - text media
// aIndex  - byte index 0..
// return  - codepoint
// ***************************************************************************
char32_t getCP(TFAccess &aText, const std::size_t aIndex) {
  if (aText.accessType == M_STRING) {
    return(getCP(*aText.mString, aIndex));
  } else {
    return(getCP(aText.substr(aIndex, 4), 0));
  }  
} 
  
// ***************************************************************************
// get a UTF-8 codepoint at given byte index
// aText      - input string
// aIndex     - byte index 0.. 
// aCodePoint - code point to read
// return - number of bytes in character , 0 if error
// ***************************************************************************
int getCP(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSize(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCP(aText, aIndex);
	} 
  return(nBytes);
}

// ***************************************************************************
// get a UTF-8 codepoint at given byte index in media
// aText      - text media
// aIndex     - byte index 0.. 
// aCodePoint - code point to read
// return     - number of bytes in character , 0 if error
// ***************************************************************************
int getCP(TFAccess &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  switch (aText.accessType) {
    case M_STRING:
      return(getCP(*aText.mString, aIndex, aCodePoint));
    case M_FILE:
      return(getCP(aText.substr(aIndex, 4), 0, aCodePoint));
  }    
  return(' ');
}

// ***************************************************************************
// find first code point unequal aCodePoint
// aText      - input string
// aIndex     - index in string
// aCodePoint - skip this codepoint
// return     - index of first different codepoint or nPos 
// ***************************************************************************
std::size_t skipCP(const std::string &aText, std::size_t aIndex, char32_t aCodePoint) {
  std::size_t index = aIndex;
  char32_t codePoint = 0;
 
  while (index < aText.size())  {
    int len = getCP(aText, index, codePoint);
    if (codePoint == aCodePoint) 
      index += len;
    else
      return(index);  
  }  

  return(nPos);  
}

// ***************************************************************************
// get no. of codepoints in string
// aString  - string  
// return   - no. of codepoints 
// **************************************************************************
std::size_t getCPLen(const std::string &aString) {
  std::size_t index = 0, len, result = 0;
  char32_t codePoint;
  while (index < aString.size()) {
    len = getCP(aString, index, codePoint);
    index += len;
    ++result;
  }
  return(result);  
}

// ****************************************************************************
// convert code point to UTF-8 string 
// aCodePoint - code point
// return     - strîng
// ****************************************************************************
std::string toString(char32_t aCodePoint) {
  Poco::UTF8Encoding utf8;
  std::string result((size_t)4, 0);

  int len = utf8.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

// ****************************************************************************
// convert UTF-8 string to code point  (only first codepoint in string)
// aCodePoint - code point
// return     - strîng
// ****************************************************************************
char32_t fromString(const std::string &aString) {
  return(getCP(aString, 0));
}

// ***************************************************************************
// get the index of next UTF-8 code point after given byte index
// aText    - input string
// aIndex   - byte index 0..
// return   - index of next char (nPos if no more found)
// ***************************************************************************
std::size_t getNextCP(const std::string &aText, std::size_t aIndex) {
  std::size_t result = aIndex; 
  char32_t cp;
 
  std::size_t len = getCP(aText, aIndex, cp);
  result += len;
  if (result >= aText.size())
    result = nPos;
  return(result);
}

// ***************************************************************************
// get the index of next aCount UTF-8 code points after given byte index
// aText    - input string
// aIndex   - byte index 0..
// aCount   - no. of code points to skip
// return   - index of char (nPos if no more found)
// ***************************************************************************
std::size_t getNextCP(TFAccess &aText, std::size_t aIndex, std::size_t aCount) {
  std::size_t result = aIndex; 
  char32_t cp;
 
  std::size_t len;
  for (std::size_t index = 0; index < aCount; ++index) {
    len = getCP(aText, aIndex, cp);
    if (len == nPos)
      return(nPos);
    result += len;
    if (result >= aText.size())
      return(nPos);
  };
  return(result);
}

// ***************************************************************************
// get the index of next UTF-8 code point after given byte index in media
// aText    - text  media
// aIndex   - byte index 0..
// return   - index of next char (nPos if no more found)
// ***************************************************************************
std::size_t getNextCP(TFAccess &aText, std::size_t aIndex) {
  switch (aText.accessType) {
    case M_STRING:
      return(getNextCP(*aText.mString, aIndex));
      break;
    case M_FILE:
      std::size_t pos = getNextCP(aText.substr(aIndex, 4 * 2), 0);
      if (pos == nPos) 
        return(nPos);
       else 
        return(aIndex + pos);       
      break;
  }
  return(0);
}

// ***************************************************************************
// get the index of previous UTF-8 code point before given byte index
// aText    - input string
// aIndex   - byte index 0.., must be at beginning of code point
// return   - index (nPos if no more found)
// ***************************************************************************
std::size_t getPrevCP(const std::string &aText, std::size_t aIndex) {
  Poco::UTF8Encoding utf8;
  if (aIndex == 0)
    return(nPos);
  
  if (aIndex == nPos) {
    aIndex = aText.size() - 1;
  } else {
    --aIndex;
  };
  
  // find beginning of previous code point
  while (!utf8.isLegal((const unsigned char*)&aText[aIndex],1)) {
    if (aIndex == 0)
      return(nPos);
    --aIndex;
  }  

  return(aIndex);
}

// ***************************************************************************
// get the index of previous UTF-8 code point before given byte index in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if no more found)
// ***************************************************************************
std::size_t getPrevCP(TFAccess &aText, std::size_t aIndex) {
  if (aText.accessType == M_STRING) {
    return(getPrevCP(*aText.mString, aIndex));
  } else {
    if (aIndex == 0)
      return(nPos);
    std::size_t curIndex, newIndex;
    if (aIndex <= 3)
      curIndex = 0;
    else 
      curIndex = aIndex - 4; // maximum size UTF-8
    newIndex = getPrevCP(aText.substr(curIndex, 8), aIndex - curIndex);
    if (newIndex == nPos)
      return(nPos);
    else   
      return(curIndex + newIndex); 
  };
}

// ***************************************************************************
// test for begin of line (first character in line) in media
// aText    - text media 
// aIndex   - byte index 0..
// return   - index (nPos if not BOL)
// ***************************************************************************
std::size_t testBOL(TFAccess &aText, std::size_t aIndex) {
  if (aIndex == 0)
    return(aIndex);

  if ( (aText.get(aIndex - 1) == '\r') || (aText.get(aIndex - 1) == '\n')) 
    return(aIndex);

  return(nPos);
}

// ***************************************************************************
// find end of line  (last end-of-line control character) in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if not EOL)
// ***************************************************************************
std::size_t findEOL(TFAccess &aText, std::size_t aIndex) {
  if (aIndex == aText.size() - 1) 
    return(aIndex);
    
  if ((aText.get(aIndex) == '\r') && (aText.get(aIndex + 1) == '\n')) 
      return(aIndex + 1);

  if ((aText.get(aIndex) == '\r') || (aText.get(aIndex) == '\n'))  
    return(aIndex);

  return(nPos);
}

// ***************************************************************************
// test for begin of word  (first character in word) in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if not BOL)
// ***************************************************************************
std::size_t testBOW(TFAccess &aText, std::size_t aIndex) {
  char32_t codePoint = 0;

  codePoint = getCP(aText, aIndex);
  if (isLetter(codePoint)) {
    std::size_t prevPos = getPrevCP(aText, aIndex);
    if (prevPos == nPos) 
      return(aIndex);  
    codePoint = getCP(aText, prevPos);
    if (!isLetter(codePoint))
      return(aIndex);  
  };

  return(nPos);
}

// ***************************************************************************
// find end of word (first character after word), may be end of previous
// character in media
// aText    - text media
// aIndex   - byte index 0..
// return - index (nPos if not BOL)
// ***************************************************************************
std::size_t findEOW(TFAccess &aText, std::size_t aIndex) {
  char32_t codePoint = 0;

  codePoint = getCP(aText, aIndex);
  if (!isLetter(codePoint)) {
    std::size_t prevPos = getPrevCP(aText, aIndex);
    if (prevPos == nPos) 
      return(nPos);  
    codePoint = getCP(aText, prevPos);
    if (isLetter(codePoint))
      return(aIndex - 1);  
  } 

  return(nPos);
}

// ***************************************************************************
// test for sentence begin (first character in sentence) in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if not BOS)
// ***************************************************************************
std::size_t testBOS(TFAccess &aText, std::size_t aIndex) {
  char32_t codePoint = 0;
  bool blankCtrl = false, endMarker = false;
  std::size_t index = aIndex;
  codePoint = getCP(aText, index);
  
  // sentence must start with non-space
  if (codePoint == ' ')
    return(nPos);
 
  // no other chars before alphanum -> BOS detected
  if (index == 0)
    return(aIndex);
    
  // skip space and controls
  while (index != 0) {
    --index;
    codePoint = getCP(aText, index);
    if (codePoint > ' ') {
      break;
    } else {
     blankCtrl = true;
     if (index == 0)
       return(aIndex);
    }  
  }
 
  // end of sentence marker found?
  for (std::size_t i = 0; i < config.sentenceEnd.size(); ++i) {
    std::size_t len = config.sentenceEnd[i].size();
    if (index >= len)  
      if (aText.substr(index, len) == config.sentenceEnd[i]) {   
        endMarker = true;
        break;
      }
  };

  if (!endMarker) 
    return(nPos);
 
  // at least one space and marker
  if (blankCtrl) 
    return(aIndex);

  return(nPos);
}

// ***************************************************************************
// test for end of sentence (first character after end-of-sentence punctuation)
// in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if not EOS)
// ***************************************************************************
std::size_t findEOS(TFAccess &aText, std::size_t aIndex) {
  bool eosSign = false;
  std::size_t index = aIndex, len;
  
  if (index < 2)
    return(nPos);
  
  for (std::size_t i = 0; i < config.sentenceEnd.size(); ++i) {
    len = config.sentenceEnd[i].size();
    if (index + len - 1 < aText.size())  
      if (aText.substr(index, len) == config.sentenceEnd[i]) {
        eosSign = true;
      }   
  }
  
  if (eosSign) {
    // no space but end of text
    if (aIndex == aText.size() - 1)
      return(aIndex);
 
    char32_t codePoint = 0;
    codePoint = getCP(aText, index + len);
    if (codePoint <= ' ') 
      // sentence ends at end of sentence sequence
      return(index + len - 1);
  };
  return(nPos);
}

// ***************************************************************************
// find end of UTF-8 BOM in media
// aText    - text media
// aIndex   - byte index 0..
// return   - index (nPos if no BOM)
// ***************************************************************************
std::size_t findBOMark(TFAccess &aText, std::size_t aIndex) {
  if (aIndex <= aText.size() - 3) {
    if (((unsigned char)aText.get(aIndex) == 0xEF) && ((unsigned char)aText.get(aIndex + 1) == 0xBB) &&  ((unsigned char)aText.get(aIndex + 2) == 0xBF))
      return(aIndex + 2);
  };
  return(nPos);
}

// ***************************************************************************
// get encoding depending on BOM (ENC_DEFAULT if no BOM is present) in media
// aText    - text media
// aIndex   - byte index 0..
// return   - ENC_XX
// ***************************************************************************
std::size_t getBOMEncoding(TFAccess &aText, std::size_t aIndex) {
  std::size_t bomEnd = findBOMark(aText, aIndex);
  if (bomEnd == nPos)
    return(ENC_DEFAULT);
  if (bomEnd - aIndex >= 4) {  
    if (((unsigned char)aText.get(aIndex) == 0x00) && ((unsigned char)aText.get(aIndex + 1) == 0x00) && ((unsigned char)aText.get(aIndex + 2) == 0xFE) && ((unsigned char)aText.get(aIndex + 3) == 0xFF))
      return(ENC_UTF32B);
    if (((unsigned char)aText.get(aIndex) == 0xFF) && ((unsigned char)aText.get(aIndex + 1) == 0xFE))
      return(ENC_UTF32L);
  };
  if (bomEnd - aIndex >= 2) {  
    if (((unsigned char)aText.get(aIndex) == 0xFE) && ((unsigned char)aText.get(aIndex + 1) == 0xFF))
      return(ENC_UTF16B);
    if (((unsigned char)aText.get(aIndex) == 0xFF) && ((unsigned char)aText.get(aIndex + 1) == 0xFE))
      return(ENC_UTF16L);
  };
  if (bomEnd - aIndex >= 1) {  
    if (((unsigned char)aText.get(aIndex) == 0xEF) && ((unsigned char)aText.get(aIndex + 1) == 0xBB) &&  ((unsigned char)aText.get(aIndex + 2) == 0xBF))
      return(ENC_UTF8);
  };

  return(ENC_DEFAULT);
}
// ***************************************************************************
// test if codepoint is part of character class
// aCodePoint - codepoint
// aClass     - class
// return     - true if part of class
// ***************************************************************************
isInline bool isClass(char32_t aCodePoint, uint16_t aClass) {
  bool match = false; 
  switch (aClass) {
    case CL_LETTER:
      match = isLetter(aCodePoint); break;
    case CL_NO_LETTER:
      match = !isLetter(aCodePoint); break;
    case CL_ALPHANUM:
      match = isAlphaNum(aCodePoint); break;
    case CL_NO_ALPHANUM:
      match = !isAlphaNum(aCodePoint); break;
    case CL_WHITE:
      match = (aCodePoint == ' '); break;
    case CL_NO_WHITE:
      match = (aCodePoint != ' '); break;
    case CL_SENT_END:
      if  (std::find(config.sentenceEnd.begin(), config.sentenceEnd.end(), toString(aCodePoint)) != config.sentenceEnd.end())
         match = true; 
      break;
    case CL_NO_SENT_END:
      if  (std::find(config.sentenceEnd.begin(), config.sentenceEnd.end(), toString(aCodePoint)) == config.sentenceEnd.end())
         match = true; 
      break;
    case CL_SEPARATOR:
      if  (std::find(config.separators.begin(), config.separators.end(), toString(aCodePoint)) != config.separators.end())
         match = true; 
      break;
    case CL_NO_SEPARATOR:
      if  (std::find(config.separators.begin(), config.separators.end(), toString(aCodePoint)) == config.separators.end())
         match = true; 
      break;
  };
  return(match);
}

// ***************************************************************************
// get the end position of any well defined block starting at current position
// that is not preceeded or succeeded by other character class  (in media)
// aText    - text media 
// aIndex   - byte index 0..
// aDefine  - CL_LETTER, CL_ALPHANUM, CL_NONWITE...  class to match
// aExclude - character class to exclude before or after block
// return   - index of last byte (nPos if no more found)
// ***************************************************************************
std::size_t findBlock(TFAccess &aText, std::size_t aIndex, uint8_t aDefine, uint8_t aExclude) {
  std::size_t index = aIndex, result = nPos;
  char32_t codePoint = 0;

  // is there a character before start of word
  std::size_t curPos = getPrevCP(aText, index);
  if (curPos != nPos) {
    codePoint = getCP(aText, curPos);
    
    // is previous character same as match character?
    if (isClass(codePoint, aDefine))
      return(nPos);

    // is previous character exclude character?
    if (isClass(codePoint, aExclude))
      return(nPos);
  } 

  while (index < aText.size())  {
    std::size_t len= getCP(aText, index, codePoint);

    // is character same as match character?
    if (isClass(codePoint, aDefine)) {
      if (result == nPos) {
        result = aIndex + len - 1;
      } else {  
        result += len;
      } 
      index += len;
    } else {
      if (index > aIndex) {
        if (index < aText.size())
          if (isClass(codePoint, aExclude))
            return(nPos);

        return(result);
      } else {  
        return(nPos);
      }
    }
  }  

  return(result);
}

// ***************************************************************************
// get end position of next  block starting with aStart and ending with aEnd
// embedded markers are skipped (in media)
// aText         - text media 
// aIndex        - byte index 0..
// aStartMarker  - start marker
// aEndMarker    - end marker (if same as start marker no nesting is allowed
// aInvalidChars - characters not allowed in block
// aMetaChars    - markers embedded in meta chars are ignored 
// return        - index of last byte of end marker
// ***************************************************************************
std::size_t findMarkedBlock(TFAccess &aText, std::size_t aIndex, const std::string &aStartMarker, const std::string &aEndMarker, \
                            const std::vector<std::string> aInvalidChars, const std::vector<std::string> aMetaChars) {
  std::size_t index = aIndex, len = 0;
  char32_t codePoint = 0;
  std::size_t block = 0;
  bool sameMarker = false;
  std::string metaChar = "";

  // is there a character before start of word
  index = aIndex;
  if (aText.substr(index, aStartMarker.size()) != aStartMarker) 
    return(nPos);

  while (index < aText.size())  {
    // invalid character?
    for (auto it:  aInvalidChars) {
      if (aText.substr(index, it.size()) == it) 
        return(nPos);
    }
      
    // check for meta region  
    for (auto it: aMetaChars) {
      if (aText.substr(index, it.size()) == it) { 
        if (metaChar == "") {
          // meta region starts
          metaChar = it;
        } else {
          if (metaChar == it)
            // meta region ends
            metaChar = "";
        }
      }
    }

    if ((aText.substr(index, aStartMarker.size()) == aStartMarker) && (!sameMarker) && (metaChar == "")) {
      // start marker
      // first block?
      ++block;
      index += aStartMarker.size();
      if (aStartMarker == aEndMarker)
        sameMarker = true; 
      continue;
    };

    if ((aText.substr(index, aEndMarker.size()) == aEndMarker) && (metaChar == "")) {
      // end marker
      
      // block open? -> else error
      if (block == 0) 
        return(nPos);
 
      --block;
      if (block == 0) {
        // all blocks closed -> end
        return(index + aEndMarker.size() - 1);
      };  
      
      index += aEndMarker.size();
      continue;
    };
    
    len = getCP(aText, index, codePoint);
    index += len;
  }  

  return(nPos);
}

// ***************************************************************************
// find begin of first of any given strings in media
// aText         - text media 
// aIndex        - byte index 0..
// aStrings      - strings to look for
// return        - index of last byte of end marker
// ***************************************************************************
std::size_t findFirstOf(TFAccess &aText, std::size_t aIndex, const std::vector<std::string> aStrings) {
  std::size_t index = aIndex, len = 0;
  char32_t codePoint = 0;
  
  while (index < aText.size())  {
    // invalid character?
    for (auto it:  aStrings) {
      if (aText.substr(index, it.size()) == it) 
        return(index);
    }
    
    len = getCP(aText, index, codePoint);
    index += len;
  }  

  return(nPos);
}

// ***************************************************************************
// get the end position of whitespace starting at current position in media
// aText    - text media 
// aIndex   - byte index 0..
// return   - index of last byte (nPos if no more found)
// ***************************************************************************
std::size_t findBlank(TFAccess &aText, std::size_t aIndex) {
  return(findBlock(aText, aIndex, CL_WHITE, CL_WHITE)); 
}

// ***************************************************************************
// get the end position of a word starting at current position in media
// aText    - text media 
// aIndex   - byte index 0..
// return   - index of last byte (nPos if no more found)
// ***************************************************************************
std::size_t findWord(TFAccess &aText, std::size_t aIndex) {
  return(findBlock(aText, aIndex, CL_LETTER, CL_NO_SEPARATOR));
}

// ***************************************************************************
// get the end position of a number starting at current position in media
// aText    - text media 
// aIndex   - byte index 0..
// aStrict  - if true, number must begin at aIndex, no digits etc. allowed as successor
// aAttr    - attributes ,SP_ATTR_INT, SP_ATTR_REAL, SP_ATTR_PURE...
//                        SP_ATTR_DEC, SP_ATTR_HEX
// return - index (nPos if no more found)
// ***************************************************************************
std::size_t findNumber(TFAccess &aText, std::size_t aIndex, bool aStrict, bool aAttrs[_SP_ATTR_END]) {
  std::size_t index = aIndex, result;
  char32_t codePoint = 0, nextCodePoint;
  bool decimal = false;
  bool thousands = false;
  bool digit = false;

  // what character is before start of number?
  std::size_t curPos = getPrevCP(aText, index);

  if (aStrict) {
    if (curPos != nPos) {
      codePoint = getCP(aText, curPos);
      if (isDigit(codePoint, aAttrs)) 
        // preceeded by digit -> not start of number
        return(nPos);

      if (isLetter(codePoint) || (codePoint == config.decimalPoint)) {
        // decimal point, if not end of sentence -> quit
        if (findEOS(aText, curPos) == nPos)
          if (!aAttrs[SP_ATTR_PURE])
            // pure number -> preceeded by letter or decimal point, cannot be a number unless we are looking for pure digits
            return(nPos);
      }    
      if ((codePoint == config.posSign) || (codePoint == config.negSign)) {
        curPos = getPrevCP(aText, curPos);
        if (curPos != nPos) {
          codePoint = getCP(aText, curPos);
          if (!isDigit(codePoint, aAttrs))
            // number before decimal point, no end of sentence-> no number
           return(nPos);
        };  
      }
    } 
  };
  
  // result is one character back
  result = curPos;

  while (index < aText.size())  {
    bool advance = false;
    std::size_t len = getCP(aText, index, codePoint);

    if (isDigit(codePoint, aAttrs)) {
      // digit -> advance
      digit = true;
      thousands = false;
      advance = true;      
    } else if ((codePoint == config.posSign) || (codePoint == config.negSign)) {
      // +/- -> advance unless number has begun
      if (index != aIndex) 
        break;
      thousands = false;
      advance = true;      
    } else if (codePoint == config.decimalPoint) {
      // decimal point, break if second occurence or integer is expected 
      if (aAttrs[SP_ATTR_INT]) {
        // see if number is a real -> break
        if (index == aIndex) {
          return(nPos);
        } else {  
          curPos = getNextCP(aText, index);
          nextCodePoint = getCP(aText, curPos);
          if (nextCodePoint == nPos)
            // decimal point as last character -> number is valid
            break;
          if (isDigit(nextCodePoint, aAttrs))
            // digit after decimal point, not a sentence end -> break
            return(nPos);
          else
           break;  
        } 
      } else if (aAttrs[SP_ATTR_PURE]) {
        // pure number, break
        if (index == aIndex)
          return(nPos);
        else   
          break;
        }  
      // second decimal point?
      if (decimal)
        break;
      decimal = true;  
      thousands = false;
      advance = true;      
    } else if (codePoint == config.thousandsSep) {
      // thousands separator
      if (aAttrs[SP_ATTR_PURE]) {
        // pure number, break
        if (index == aIndex)
          return(nPos);
        else   
          break;
      }  

      if (thousands)
        return(nPos);
      if (index == aIndex) 
        return(nPos);

      curPos = getNextCP(aText, index);
      nextCodePoint = getCP(aText, curPos);
      if (nextCodePoint == nPos)
        // separator as last character -> number is valid
        break;
      if (!isDigit(nextCodePoint, aAttrs))
        // digit after decimal point, not a sentence end -> break
        break;

      thousands = true;
      advance = true;  
    } else {
      // no digit or other character used in numbers
      if (index == aIndex)
        return(nPos);
      if (aAttrs[SP_ATTR_PURE]) {
        // pure number must have adjacent letter
        if (isLetter(codePoint))
          return(nPos);
      }  
      
      break;
    }
    if (advance) {
      result = index + len - 1; 
      index += len;
    }
  }  

  // no digit found?
  if (!digit) 
    return(nPos);

  return(result);
}

// ***************************************************************************
// get the starting starting position of a number, before aIndex in media
// current position is anywhere in it
// aText    - text media 
// aIndex   - byte index 0.. before which number must lie
// aAttr    - attributes ,SP_ATTR_INT, SP_ATTR_REAL, SP_ATTR_PURE...
//                        SP_ATTR_DEC, SP_ATTR_HEX, SP_ATTR_OCT, SP_ATTR_BIN, SP_ATTR_DATE 
// return - start of number or nPos
// ***************************************************************************
std::size_t findNumberBegin(TFAccess &aText, std::size_t aIndex, bool aAttrs[_SP_ATTR_END]) {
  std::size_t start = aIndex - 1, end, lastEnd, lastStart = nPos;
  bool found = false;

  while (start != nPos) {
    if ((end = findNumber(aText, start, false, aAttrs)) != nPos) {
      lastEnd = end;
      lastStart = start;
      start = getPrevCP(aText, start);
      found = true;
    } else {
      start = lastStart;
      break;
    }
  } 
 
  if ((start == aIndex) || (lastEnd >= aIndex) || !found) {
    return(nPos);
  } else {
    if (start == nPos)
      start = 0;
      
    return(start);
  }  

  return(nPos);
}

// ***************************************************************************
// find a string in another string, case sensitive in media
// aText    - text media 
// aSearch  - string to search for
// aStart   - start position
// return   - nPos if not found, else index
// ***************************************************************************
std::size_t findStr(TFAccess &aText, const std::string &aSearch, std::size_t aStart) {
  return(aText.find(aSearch, aStart));
}

// ***************************************************************************
// find a string in another string, case insensitive
// aString  - string to look at
// aSearch  - string to search for
// aStart   - start position
// return   - nPos if not found, else index
// ***************************************************************************
std::size_t findStrCase(const std::string &aString, const std::string &aSearch, std::size_t aStart) {
  std::string upperSearch = upperText(aSearch);
  std::size_t len = upperSearch.size();

  if (len > aString.size())
    return (nPos);

  wchar_t c1 =  Poco::Unicode::toUpper(getCP(aSearch, 0)); 

  for(std::size_t index = aStart; index < aString.size() - len + 1; ++index) {
    if (c1 <= 127) {
      if (c1 != std::toupper(getCP(aString, index)))
        continue;     
    } else if (c1 != Poco::Unicode::toUpper(getCP(aString, index))) {
      continue;     
    };
    
    if (Poco::UTF8::icompare(upperSearch, aString.substr(index, len)) == 0) 
      return(index);  
  }

  return (nPos);
}

// ***************************************************************************
// find a string in another string, case insensitive in media
// aText    - text media 
// aSearch  - string to search for
// aStart   - start position
// return   - nPos if not found, else index
// ***************************************************************************
std::size_t findStrCase(TFAccess &aText, const std::string &aSearch, std::size_t aStart) {
  // special case string
  if (aText.accessType == M_STRING)  
    return(findStrCase(*aText.mString, aSearch, aStart));

  if (aText.accessType == M_FILE) 
   return(nPos); 

  // special case file
  std::string upperSearch = upperText(aSearch);
  std::size_t len = upperSearch.size();

  if (len > aText.size())
    return (nPos);

  wchar_t c1 =  Poco::Unicode::toUpper(getCP(aSearch, 0)); 

  for(std::size_t index = aStart; index < aText.size() - len + 1; ++index) {
    if (c1 <= 127) {
      if (c1 != std::toupper(getCP(aText, index)))
        continue;     
    } else if (c1 != Poco::Unicode::toUpper(getCP(aText, index))) {
      continue;     
    };
    
    if (Poco::UTF8::icompare(upperSearch, aText.substr(index, len)) == 0) 
      return(index);  
  }

  return (nPos);

}

// ***************************************************************************
// read string from file
// aFile  - path to file  
// aStr   - string to fill
// return - true is succesful
// **************************************************************************
bool readFromFile(const std::string &aFile, std::string &aStr) {
  std::ifstream file(aFile, std::ios_base::binary); 
  aStr = "";
  if(file) {
    std::ostringstream ss;
    ss << file.rdbuf();
    aStr = ss.str();
    return(true);
  };
  return(false);
}   

// ***************************************************************************
// write string to file
// aFile  - path to file  
// aStr   - string to write
// return - true is succesfull
// **************************************************************************
bool writeToFile(const std::string &aFile, const std::string &aStr) {
  std::ofstream file(aFile, std::ios_base::binary);
  if(file) {
    file << aStr;
    return(true);
  };
  return(false);
}   

// ***************************************************************************
// tokenize a line
// aList      - list of tokens 
// aSeparator - separator, white space and control characters at 
//              begining/end are eliminated
// aTokens    . tokens
// **************************************************************************
void tokenize(const std::string &aList, const std::string &aSeperator, std::vector<std::string> &aTokens) {
  using Poco::StringTokenizer;
  aTokens.clear();
  StringTokenizer t1(aList, aSeperator);
  for (auto it : t1) {
    aTokens.push_back(Poco::trim(it));
  }
}  
}
