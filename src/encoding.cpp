// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski 
// License:  BSL 1.0

#include "general.hpp"
#include "extern.hpp"
#include "encoding.hpp"
#include "text.hpp"

namespace Scripal {

  // ****************************************************************************
// convert digit in native language to standard '0','1',... 
// aCodePoint - code point to convert
// return true if successfull
// ****************************************************************************
bool convertDigit(char32_t& aCodePoint) {
  try {
    aCodePoint = digits.at(aCodePoint);
    return(true);
  } catch (std::exception& aExc) {
    return(false);
  }  
}
  
// ****************************************************************************
// convert all digits in native language to standard '0','1',... 
// aString - string with native code points
// ****************************************************************************
void convertStrDigits(std::string &aString) {
  char32_t codePoint = 0;
  std::size_t pos = 0, len;
  std::string result = "";

  while (pos < aString.size()) {
    // convert
    len = getCP(aString, pos, codePoint);
    convertDigit(codePoint);
    result += toString(codePoint);
    pos += len;
  }
  aString = result;
}  
  


// ***************************************************************************
// get a specific code point size in string
// aText  - string
// aIndex - byte index 0..
// return - size in bytes needed for storage or 0 for no or invalid character
// ***************************************************************************

isInline int getCPSize8(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF8Encoding utf8;
  int nBytes = utf8.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeUTF16L(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
  int nBytes = utf16.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeUTF16B(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::BIG_ENDIAN_BYTE_ORDER);
  int nBytes = utf16.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeUTF32L(const std::string &aText, const std::size_t aIndex) {
  return(4);
}

isInline int getCPSizeUTF32B(const std::string &aText, const std::size_t aIndex) {
  return(4);
}

isInline int getCPSizeASCII(const std::string &aText, const std::size_t aIndex) {
  return(1);
}

isInline int getCPSizeCP932(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows932Encoding win932;
  int nBytes = win932.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeCP936(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows936Encoding win936;
  int nBytes = win936.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeCP949(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows949Encoding win949;
  int nBytes = win949.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeCP950(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows950Encoding win950;
  int nBytes = win950.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeLATIN1(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin1Encoding latin1;
  int nBytes = latin1.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeLATIN2(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin2Encoding latin2;
  int nBytes = latin2.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeLATIN9(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin9Encoding latin9;
  int nBytes = latin9.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN874(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows874Encoding win874;
  int nBytes = win874.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1250(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1250Encoding win1250;
  int nBytes = win1250.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1251(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1251Encoding win1251;
  int nBytes = win1251.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1252(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1252Encoding win1252;
  int nBytes = win1252.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1253(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1253Encoding win1253;
  int nBytes = win1253.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1254(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1254Encoding win1254;
  int nBytes = win1254.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1255(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1255Encoding win1255;
  int nBytes = win1255.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1256(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1256Encoding win1256;
  int nBytes = win1256.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1257(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1257Encoding win1257;
  int nBytes = win1257.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

isInline int getCPSizeWIN1258(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1258Encoding win1258;
  int nBytes = win1258.sequenceLength((const unsigned char*)&aText[aIndex], 4);
  if (nBytes <= 0) return(0); else return(nBytes);
}

// ***************************************************************************
// return UTF-xxx codepoint at given byte index
// aText   - input string
// aIndex  - byte index 0..
// return  - codepoint
// ***************************************************************************
isInline char32_t getCPUTF8(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF8Encoding utf8;
  char32_t result = utf8.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}

isInline char32_t getCPUTF16L(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
  char32_t result = utf16.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}

isInline char32_t getCPUTF16B(const std::string &aText, const std::size_t aIndex) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::BIG_ENDIAN_BYTE_ORDER);
  char32_t result = utf16.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}

isInline char32_t getCPUTF32L(const std::string &aText, const std::size_t aIndex) {
  char32_t result = *(char32_t*)((const unsigned char*)aText.c_str() + aIndex);
  if (isBigEndian) {
    swapBytes(result);
  }
  return(result); 
}

isInline char32_t getCPUTF32B(const std::string &aText, const std::size_t aIndex) {
  char32_t result = *(char32_t*)((const unsigned char*)aText.c_str() + aIndex);
  if (!isBigEndian) {
    swapBytes(result);
  }
  return(result); 
}

isInline char32_t getCPASCII(const std::string &aText, const std::size_t aIndex) {
  char32_t result = (char32_t)*((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}

isInline char32_t getCPCP932(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows932Encoding win932;

  char32_t result =  win932.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPCP936(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows936Encoding win936;

  char32_t result =  win936.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPCP949(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows949Encoding win949;

  char32_t result =  win949.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPCP950(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows950Encoding win950;

  char32_t result =  win950.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPLATIN1(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin1Encoding latin1;

  char32_t result =  latin1.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPLATIN2(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin2Encoding latin2;

  char32_t result =  latin2.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPLATIN9(const std::string &aText, const std::size_t aIndex) {
  Poco::Latin9Encoding latin9;

  char32_t result =  latin9.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN874(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows874Encoding win874;

  char32_t result =  win874.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1250(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1250Encoding win1250;

  char32_t result =  win1250.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1251(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1251Encoding win1251;

  char32_t result =  win1251.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1252(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1252Encoding win1252;

  char32_t result =  win1252.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1253(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1253Encoding win1253;

  char32_t result =  win1253.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1254(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1254Encoding win1254;

  char32_t result =  win1254.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1255(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1255Encoding win1255;

  char32_t result =  win1255.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1256(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1256Encoding win1256;

  char32_t result =  win1256.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1257(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1257Encoding win1257;

  char32_t result =  win1257.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

isInline char32_t getCPWIN1258(const std::string &aText, const std::size_t aIndex) {
  Poco::Windows1258Encoding win1258;

  char32_t result =  win1258.convert((const unsigned char*)aText.c_str() + aIndex);
  return(result); 
}  

// ***************************************************************************
// get a UTF-xx codepoint at given byte index
// aText      - input string
// aIndex     - byte index 0.. 
// aCodePoint - code point to read
// return - number of bytes in character , 0 if error
// ***************************************************************************
isInline int getCPUTF8(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSize8(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPUTF8(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPUTF16L(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeUTF16L(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPUTF16L(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPUTF16B(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeUTF16B(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPUTF16B(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPUTF32L(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeUTF32L(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPUTF32L(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPUTF32B(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeUTF32B(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPUTF32B(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPASCII(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeASCII(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPASCII(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPCP932(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeCP932(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPCP932(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPCP936(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeCP936(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPCP936(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPCP949(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeCP949(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPCP949(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPCP950(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeCP950(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPCP950(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPLATIN1(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeLATIN1(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPLATIN1(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPLATIN2(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeLATIN2(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPLATIN2(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPLATIN9(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeLATIN9(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPLATIN9(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN874(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN874(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN874(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1250(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1250(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1250(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1251(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1251(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1251(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1252(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1252(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1252(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1253(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1253(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1253(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1254(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1254(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1254(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1255(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1255(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1255(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1256(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1256(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1256(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1257(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1257(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1257(aText, aIndex);
	} 
  return(nBytes);
}

isInline int getCPWIN1258(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint) {
  int nBytes;

  nBytes = getCPSizeWIN1258(aText, aIndex);
  if (nBytes != 0) {
    aCodePoint = getCPWIN1258(aText, aIndex);
	} 
  return(nBytes);
}

// ***************************************************************************
// get a codepoint at given byte index using given encoding
// aText      - input string
// aIndex     - byte index 0.. 
// aCodePoint - code point to read
// aEncoding  - encoding used 
// return - number of bytes in character , 0 if error
// ***************************************************************************
int getCP(const std::string &aText, const std::size_t aIndex, char32_t &aCodePoint, int aEncoding) {
  int result = 0;
  switch (aEncoding) {
    case ENC_UTF8:
      result = getCPUTF8(aText, aIndex, aCodePoint);
      break;
    case ENC_UTF16L:
      result = getCPUTF16L(aText, aIndex, aCodePoint);
      break;
   case ENC_UTF16B:
      result = getCPUTF16B(aText, aIndex, aCodePoint);
      break;
    case ENC_UTF32L:
      result = getCPUTF32L(aText, aIndex, aCodePoint);
      break;
    case ENC_UTF32B:
      result = getCPUTF32B(aText, aIndex, aCodePoint);
      break;
    case ENC_ASCII:
      result = getCPASCII(aText, aIndex, aCodePoint);
      break;
    case ENC_CP932:
      result = getCPCP932(aText, aIndex, aCodePoint);
      break;
    case ENC_CP936:
      result = getCPCP936(aText, aIndex, aCodePoint);
      break;
    case ENC_CP949:
      result = getCPCP949(aText, aIndex, aCodePoint);
      break;
    case ENC_CP950:
      result = getCPCP950(aText, aIndex, aCodePoint);
      break;
    case ENC_LATIN1:
      result = getCPLATIN1(aText, aIndex, aCodePoint);
      break;
    case ENC_LATIN2:
      result = getCPLATIN2(aText, aIndex, aCodePoint);
      break;
    case ENC_LATIN9:
      result = getCPLATIN9(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN874:
      result = getCPWIN874(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1250:
      result = getCPWIN1250(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1251:
      result = getCPWIN1251(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1252:
      result = getCPWIN1252(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1253:
      result = getCPWIN1253(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1254:
      result = getCPWIN1254(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1255:
      result = getCPWIN1255(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1256:
      result = getCPWIN1256(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1257:
      result = getCPWIN1257(aText, aIndex, aCodePoint);
      break;
    case ENC_WIN1258:
      result = getCPWIN1258(aText, aIndex, aCodePoint);
      break;
      
    default:  
      result = 0;
      break;
  }
  return(result);
}

// ****************************************************************************
// convert code point to UTF-xx string 
// aCodePoint - code point
// return     - strîng
// ****************************************************************************
isInline std::string toStringUTF8(char32_t aCodePoint) {
  Poco::UTF8Encoding utf8;
  std::string result((size_t)4, 0);

  int len = utf8.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringUTF16L(char32_t aCodePoint) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
  std::string result((size_t)4, 0);

  int len = utf16.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringUTF16B(char32_t aCodePoint) {
  Poco::UTF16Encoding utf16(Poco::UTF16Encoding::BIG_ENDIAN_BYTE_ORDER);
  std::string result((size_t)4, 0);

  int len = utf16.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringUTF32L(char32_t aCodePoint) {
  std::string result((size_t)4, 0);
  if (isBigEndian)
    swapBytes(aCodePoint);

  memcpy((unsigned char*)result.c_str(), &aCodePoint, 4); 
  return(result);
}

isInline std::string toStringUTF32B(char32_t aCodePoint) {
  std::string result((size_t)4, 0);
  if (isBigEndian)
    swapBytes(aCodePoint);

  memcpy((unsigned char*)result.c_str(), &aCodePoint, 4); 
  return(result);
}

isInline std::string toStringASCII(char32_t aCodePoint) {
  std::string result(1, (char)aCodePoint);

  return(result);
}

isInline std::string toStringCP932(char32_t aCodePoint) {
  Poco::Windows932Encoding win932;
  std::string result((size_t)1, 0);

  int len =  win932.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringCP936(char32_t aCodePoint) {
  Poco::Windows936Encoding win936;
  std::string result((size_t)1, 0);

  int len =  win936.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringCP949(char32_t aCodePoint) {
  Poco::Windows949Encoding win949;
  std::string result((size_t)1, 0);

  int len =  win949.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringCP950(char32_t aCodePoint) {
  Poco::Windows950Encoding win950;
  std::string result((size_t)1, 0);

  int len =  win950.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringLATIN1(char32_t aCodePoint) {
  Poco::Latin1Encoding latin1;
  std::string result((size_t)1, 0);

  int len =  latin1.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringLATIN2(char32_t aCodePoint) {
  Poco::Latin2Encoding latin2;
  std::string result((size_t)1, 0);

  int len =  latin2.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringLATIN9(char32_t aCodePoint) {
  Poco::Latin9Encoding latin9;
  std::string result((size_t)1, 0);

  int len =  latin9.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringWIN874(char32_t aCodePoint) {
  Poco::Windows874Encoding win874;
  std::string result((size_t)1, 0);

  int len =  win874.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

isInline std::string toStringWIN1250(char32_t aCodePoint) {
  Poco::Windows1250Encoding win1250;
  std::string result((size_t)1, 0);

  int len =  win1250.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1251(char32_t aCodePoint) {
  Poco::Windows1251Encoding win1251;
  std::string result((size_t)1, 0);

  int len =  win1251.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1252(char32_t aCodePoint) {
  Poco::Windows1252Encoding win1252;
  std::string result((size_t)1, 0);

  int len =  win1252.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1253(char32_t aCodePoint) {
  Poco::Windows1253Encoding win1253;
  std::string result((size_t)1, 0);

  int len =  win1253.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1254(char32_t aCodePoint) {
  Poco::Windows1254Encoding win1254;
  std::string result((size_t)1, 0);

  int len =  win1254.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1255(char32_t aCodePoint) {
  Poco::Windows1255Encoding win1255;
  std::string result((size_t)1, 0);

  int len =  win1255.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1256(char32_t aCodePoint) {
  Poco::Windows1256Encoding win1256;
  std::string result((size_t)1, 0);

  int len =  win1256.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1257(char32_t aCodePoint) {
  Poco::Windows1257Encoding win1257;
  std::string result((size_t)1, 0);

  int len =  win1257.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

std::string toStringWIN1258(char32_t aCodePoint) {
  Poco::Windows1258Encoding win1258;
  std::string result((size_t)1, 0);

  int len =  win1258.convert(aCodePoint, (unsigned char*)result.c_str(), 4);
  result.resize(len);
  return(result);
}

// ***************************************************************************
// convert a UTF8 string to UTF32 for algos needing indexed access
// aString : string
// return - UTF32 string 
// ***************************************************************************
std::u32string utf8ToUtf32(const std::string &aString) {
  int encoding = (isBigEndian? ENC_UTF32B : ENC_UTF32L);
  std::string sConv = encSrcToDst(aString, ENC_UTF8, encoding);
  std::u32string result((const char32_t*)sConv.c_str(), sConv.size() / 4); 
  return(result);
}

// ****************************************************************************
// convert UTF-8 to string in other encoding using system endianess 
// aString    - UTF-8 string
// aEncode    - encoding
// return     - new encoded string
// ****************************************************************************
std::string utf8ToEnc(std::string_view aString, int aEncode) {
  return(encSrcToDst(aString, ENC_UTF8, aEncode));

}

// ****************************************************************************
// convert string in other encoding to UTF-8 string using system endianess 
// aString    - encoded string
// aEncode    - encoding
// return     - UTF-8 strîng
 // ****************************************************************************
std::string encToUtf8(std::string_view aString, int aEncode) {
  return(encSrcToDst(aString, aEncode, ENC_UTF8));
}

// ****************************************************************************
// convert string from any to any encoding  
// aString       - source string
// aEncodeSrc    - source encoding
// aEncodeDST    - destination encoding
// return        - newly encoded string
 // ****************************************************************************
std::string encSrcToDst(std::string_view aString, int aEncodeSrc, int aEncodeDst) {
  std::string convert;
  char32_t codePoint = 0;
  std::size_t lenEnc = 0, posEnc = 0;
  const std::string src = std::string(aString); 
   
  std::string result;
  
  // same encoding?
  if (aEncodeSrc == aEncodeDst)
    return(std::string(aString));

  std::chrono::time_point<std::chrono::steady_clock> sTime;
  while (posEnc < aString.size()) {
    // convert
    switch (aEncodeSrc) {
      case ENC_UTF8: 
        lenEnc = getCPUTF8(src, posEnc, codePoint);
        break;
      case ENC_UTF16L:
        lenEnc = getCPUTF16L(src, posEnc, codePoint);
        break;
      case ENC_UTF16B:
        lenEnc = getCPUTF16B(src, posEnc, codePoint);
        break;
      case ENC_UTF32L:
        lenEnc = getCPUTF32L(src, posEnc, codePoint);
        break;
      case ENC_UTF32B:
        lenEnc = getCPUTF32B(src, posEnc, codePoint);
        break;
      case ENC_ASCII:
        lenEnc = getCPASCII(src, posEnc, codePoint);
        break;
      case ENC_CP932:
        lenEnc = getCPCP932(src, posEnc, codePoint);
        break;
      case ENC_CP936:
        lenEnc = getCPCP936(src, posEnc, codePoint);
        break;
      case ENC_CP949:
        lenEnc = getCPCP949(src, posEnc, codePoint);
        break;
      case ENC_CP950:
        lenEnc = getCPCP950(src, posEnc, codePoint);
        break;
      case ENC_LATIN1:
        lenEnc = getCPLATIN1(src, posEnc, codePoint);
        break;
      case ENC_LATIN2:
        lenEnc = getCPLATIN2(src, posEnc, codePoint);
        break;
      case ENC_LATIN9:
        lenEnc = getCPLATIN9(src, posEnc, codePoint);
        break;
      case ENC_WIN874:
        lenEnc = getCPWIN874(src, posEnc, codePoint);
        break;
      case ENC_WIN1250:
        lenEnc = getCPWIN1250(src, posEnc, codePoint);
        break;
      case ENC_WIN1251:
        lenEnc = getCPWIN1251(src, posEnc, codePoint);
        break;
      case ENC_WIN1252:
        lenEnc = getCPWIN1252(src, posEnc, codePoint);
        break;
      case ENC_WIN1253:
        lenEnc = getCPWIN1253(src, posEnc, codePoint);
        break;
      case ENC_WIN1254:
        lenEnc = getCPWIN1254(src, posEnc, codePoint);
        break;
      case ENC_WIN1255:
        lenEnc = getCPWIN1255(src, posEnc, codePoint);
        break;
      case ENC_WIN1256:
        lenEnc = getCPWIN1256(src, posEnc, codePoint);
        break;
      case ENC_WIN1257:
        lenEnc = getCPWIN1257(src, posEnc, codePoint);
        break;
      case ENC_WIN1258:
        lenEnc = getCPWIN1258(src, posEnc, codePoint);
        break;
      default:
        return("");
        break;
    };  

    switch (aEncodeDst) {
      case ENC_UTF8:
        convert = toStringUTF8(codePoint);
        break;
      case ENC_UTF16L:
        convert = toStringUTF16L(codePoint);
        break;
      case ENC_UTF16B:
        convert = toStringUTF16B(codePoint);
        break;
      case ENC_UTF32L:
        convert = toStringUTF32L(codePoint);
        break;
      case ENC_UTF32B:
        convert = toStringUTF32B(codePoint);
        break;
      case ENC_ASCII:
        convert = toStringASCII(codePoint);
        break;
      case ENC_CP932:
        convert = toStringCP932(codePoint);
        break;
      case ENC_CP936:
        convert = toStringCP936(codePoint);
        break;
      case ENC_CP949:
        convert = toStringCP949(codePoint);
        break;
      case ENC_CP950:
        convert = toStringCP950(codePoint);
        break;
      case ENC_LATIN1:
        convert = toStringLATIN1(codePoint);
        break;
      case ENC_LATIN2:
        convert = toStringLATIN2(codePoint);
        break;
      case ENC_LATIN9:
        convert = toStringLATIN9(codePoint);
        break;
      case ENC_WIN874:
        convert = toStringWIN874(codePoint);
        break;
      case ENC_WIN1250:
        convert = toStringWIN1250(codePoint);
        break;
      case ENC_WIN1251:
        convert = toStringWIN1251(codePoint);
        break;
      case ENC_WIN1252:
        convert = toStringWIN1252(codePoint);
        break;
      case ENC_WIN1253:
        convert = toStringWIN1253(codePoint);
        break;
      case ENC_WIN1254:
        convert = toStringWIN1254(codePoint);
        break;
      case ENC_WIN1255:
        convert = toStringWIN1255(codePoint);
        break;
      case ENC_WIN1256:
        convert = toStringWIN1256(codePoint);
        break;
      case ENC_WIN1257:
        convert = toStringWIN1257(codePoint);
        break;
      case ENC_WIN1258:
        convert = toStringWIN1258(codePoint);
        break;
      default:  
        return("");
        break;
    };  

    result += convert;

    posEnc += lenEnc;
  }

  return(result);
}

std::string encSrcToDst(const char* aText, std::size_t aTextLen, int aEncodeSrc, int aEncodeDst) {
  return(encSrcToDst(std::string_view(aText, aTextLen), aEncodeSrc, aEncodeDst));
}

// ****************************************************************************
// find best fit character encoding 
// either it is given or use scheme according to current locale
// aEncoding - either determined or ENC_DEFAULT
// return - best fit encoding
// ****************************************************************************
int getBestEncoding(int aEncoding) {
  if (aEncoding == ENC_DEFAULT) {
    #ifdef LINUX
    std::locale loc("");
    std::string curLocale = upperText(loc.name());

    if (curLocale.find("UTF8") != nPos) {
      return(ENC_UTF8);
    } else if (curLocale.find("UTF-8") != nPos) {
      return(ENC_UTF8);
    } else if (curLocale.find("UTF16L") != nPos) {
      return(ENC_UTF16L);
    } else if (curLocale.find("UTF-16L") != nPos) {
      return(ENC_UTF16L);
    } else if (curLocale.find("UTF16B") != nPos) {
      return(ENC_UTF16B);
    } else if (curLocale.find("UTF-16B") != nPos) {
      return(ENC_UTF16B);
    } else if (curLocale.find("UTF16") != nPos) {
      if (isBigEndian)
        return(ENC_UTF16B);
      else  
        return(ENC_UTF16L);
    } else if ((curLocale.find("UTF32") != nPos) || (curLocale.find("UTF-32") != nPos)) {
      if (isBigEndian)
        return(ENC_UTF32B);
      else  
        return(ENC_UTF32L);
    } else {
      return(ENC_UTF8);
    }; 
    #endif
    #ifdef WINDOWS
    UINT cp = GetConsoleOutputCP();
    if (cp == 850)
        return(ENC_LATIN1);
    if (cp == 1200)
        return(ENC_UTF16L);
    if (cp == 1201)
        return(ENC_UTF16B);
    if (cp == 1250)
        return(ENC_WIN1250);
    if (cp == 1251)
        return(ENC_WIN1251);
    if (cp == 1252)
        return(ENC_WIN1252);
    if (cp == 1253)
        return(ENC_WIN1253);
    if (cp == 1254)
        return(ENC_WIN1254);
    if (cp == 1255)
        return(ENC_WIN1255);
    if (cp == 1256)
        return(ENC_WIN1256);
    if (cp == 1257)
        return(ENC_WIN1257);
    if (cp == 1258)
        return(ENC_WIN1258);
    if (cp == 65001)
        return(ENC_UTF8);
    return(ENC_UTF8);
    #endif
  };
  
  return(aEncoding);
}

  

}