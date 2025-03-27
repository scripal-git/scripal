// ***************************************************************************
// interface functions for auxillary languages (external calls are in pure C)
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <stdbool.h> 

const int SCRIPAL_ENC_DEFAULT  = 1;
const int SCRIPAL_ENC_UTF8     = 2;
const int SCRIPAL_ENC_UTF16L   = 3;
const int SCRIPAL_ENC_UTF16B   = 4;
const int SCRIPAL_ENC_UTF32L   = 5;
const int SCRIPAL_ENC_UTF32B   = 6;
const int SCRIPAL_ENC_ASCII    = 10;  //  ASCII , Extended ASCII (up to 255)
const int SCRIPAL_ENC_CP932    = 11;  //  CP932 DBCS, Japanese characters
const int SCRIPAL_ENC_CP936    = 12;  //  CP93 6DBCS, simple Chinese characters
const int SCRIPAL_ENC_CP949    = 13;  //  CP949 DBCS, Korean characters
const int SCRIPAL_ENC_CP950    = 14;  //  CP505 DBCS, Chinese Big5 characters
const int SCRIPAL_ENC_LATIN1   = 30;  //  West Europe Latin-1, ISO 8859-15
const int SCRIPAL_ENC_LATIN2   = 31;  //  Middle Europe Latin-2, ISO 8859-15
const int SCRIPAL_ENC_LATIN9   = 32;  //  West Europe Latin-9, ISO 8859-15
const int SCRIPAL_ENC_WIN874   = 50;  //  Windows Codepage 874, Thai characters
const int SCRIPAL_ENC_WIN1250  = 51;  //  Windows Codepage 1251, Middle Europe
const int SCRIPAL_ENC_WIN1251  = 52;  //  Windows Codepage 1251, Cyrillic
const int SCRIPAL_ENC_WIN1252  = 53;  //  Windows Codepage 1252, West Europe
const int SCRIPAL_ENC_WIN1253  = 54;  //  Windows Codepage 1253, Greek
const int SCRIPAL_ENC_WIN1254  = 55;  //  Windows Codepage 1254, Turkish
const int SCRIPAL_ENC_WIN1255  = 56;  //  Windows Codepage 1255, Hebrew
const int SCRIPAL_ENC_WIN1256  = 57;  //  Windows Codepage 1256, Arabic
const int SCRIPAL_ENC_WIN1257  = 58;  //  Windows Codepage 1257, Baltic
const int SCRIPAL_ENC_WIN1258  = 59;  //  Windows Codepage 1258, Vietnamese

const int SCRIPAL_POS_UTF8          = 1;
const int SCRIPAL_POS_OFFSET        = 2;
const int SCRIPAL_POS_COUNT         = 3;

const int SCRIPAL_MATCH_SOURCE      = 1;
const int SCRIPAL_MATCH_NEAREST     = 2;
const int SCRIPAL_MATCH_BLOCK       = 3;

const int SCRIPAL_PATTERN_LEVEN_WORD = 1;
const int SCRIPAL_PATTERN_LEVENPLUS_WORD = 2;
const int SCRIPAL_PATTERN_LEVEN = 3;
const int SCRIPAL_PATTERN_JARO = 100;
const int SCRIPAL_PATTERN_JAROWINKLER = 101;
const int SCRIPAL_PATTERN_JAROWINKLER_WORD = 102;

#ifdef __linux__
 // inline control
  #if defined(_SHARED_LIB_)
    #define LIB_CTYPE extern "C"
  #else 
    #if defined(_SCRIPAL)
      #define LIB_CTYPE
    #else
      #define LIB_CTYPE extern "C"
    #endif     
  #endif

  #define STRDUP strdup
#endif 
#ifdef _WIN32
  // inline control
  #define isInline 
  // internal define for Windows 32/64 type OS (gcc used)
  #define WINDOWS 
  #if defined(_DLL)
    #ifndef _SCRIPAL
      #define LIB_CTYPE __declspec(dllexport)  
    #else
      #define LIB_CTYPE extern "C" __declspec(dllexport) 
    #endif 
  #else
    #ifndef _SCRIPAL
      #define LIB_CTYPE __declspec(dllexport)  
    #else
      #define LIB_CTYPE 
    #endif 
  #endif 
 
  #include <windows.h>
  #define STRDUP _strdup
#endif
// ****************************************************************************
// testing
// ****************************************************************************
LIB_CTYPE void printText(const char* aText, int aTextLen, int aEncoding);

// ****************************************************************************
// management
// ****************************************************************************
//get version
LIB_CTYPE void getVersion(const char** aVersion, int* aVersionLen);
// initialize library base (optional is initialized with first object)
LIB_CTYPE void baseInit();
// finalize library base
LIB_CTYPE void baseFinalize();
// initialize library 
LIB_CTYPE void libInit(const char* aConfigPath, int aConfigPathLen, int aEncoding);
// initialize library for C usage
LIB_CTYPE void libInitC(const char* aConfigPath, int aConfigPathLen, int aEncoding);
// finalize library in process
LIB_CTYPE void libFinalize();
// initialize logs
LIB_CTYPE void logInit(const char*  aLogChannel, int  aLogChannelLen);
// get log buffer 
LIB_CTYPE void getLogBuf(const char** aLogBuf, int* aLogBufLen);
// clear log buffer
LIB_CTYPE void clearLogBuf();
// delete any portion of memory returned via thesi interface
LIB_CTYPE void deletePtr(const char* aPtr);
// get last error message
LIB_CTYPE void getErrMsg(const char** aError, int* aErrorLen);
// get last explicit error
LIB_CTYPE void getErrExp(const char** aError, int* aErrorLen);
// get last error position
LIB_CTYPE int getErrPos();
// clear last error
LIB_CTYPE void clearErr();
// set encoding
LIB_CTYPE void setEncoding(int aEncoding);

// ****************************************************************************
// Scripal objects 
// ****************************************************************************

// create new code object form given source
LIB_CTYPE int newCodeObj(const char* aSource, int aSourceLen);
// create new object for nearest match form given source
LIB_CTYPE int newNearestObj(const char* aSource, int aSourceLen, double aMax);
// create new object for block match form given source
LIB_CTYPE int newBlockObj(const char* aSource, int aSourceLen, double aMax);
// delete managed Scripal object
LIB_CTYPE void deleteObj(int aObjID);

// ****************************************************************************
// matching
// ****************************************************************************

LIB_CTYPE bool match(int aObjID, const char* aText, int aTextLen);
LIB_CTYPE bool matchFile(int aObjID, const char* aFile, int aFileLen, int aFileEncoding);
LIB_CTYPE bool replace(int aObjID, const char* aText, int aTextLen, const char* aReplace, int aReplaceLen);
LIB_CTYPE bool replaceFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, const char* aReplace, int aReplaceLen, int aFileEncoding);
LIB_CTYPE bool split(int aObjID, const char* aText, int aTextLen);
LIB_CTYPE bool splitFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, int aFileEncoding);
LIB_CTYPE bool searchFiles(int aObjID, const char* aPath, int aPathLen, const char* aDirExtension, int aDirExtensionLen , bool aRecursive);
LIB_CTYPE int  getResultSize(int aObjID);
LIB_CTYPE bool getResultSpecs(int aObjID, int aIndex, int* aEncoding, int* aMatchType, int* aPositionType);
LIB_CTYPE bool getResultStr(int aObjID, int aIndex, const char** aResult, int* aResultLen);
LIB_CTYPE bool getResultFileName(int aObjID, int aIndex, const char** aResult, int* aResultLen);
LIB_CTYPE bool getResultTag(int aObjID, int aIndex, const char** aResult, int* aResultLen);
LIB_CTYPE bool getResultPos(int aObjID, int aIndex, int* aPosStart, int* aPosEnd);
LIB_CTYPE bool getResultRating(int aObjID, int aIndex, double* aRating);
LIB_CTYPE bool getResultJSON(int aObjID, const char** aResult, int* aResultLen);
LIB_CTYPE bool getResultCSV(int aObjID, const char** aResult, int* aResultLen);
LIB_CTYPE bool getResultHRF(int aObjID, const char** aResult, int* aResultLen);

// ****************************************************************************
// config
// ****************************************************************************

// set config according to locale 
LIB_CTYPE bool configSetLocale(const char* aLoc, int aLocLen);

// get JSON string from config
LIB_CTYPE bool configToJSON(const char** aJSON, int* aLen);

// get selected config values as JSON string, list in aNámes separated by ','
LIB_CTYPE bool configToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen);

// get config from JSON string
LIB_CTYPE bool configFromJSON(const char* aJSON, int aLen);

// store config in given file
LIB_CTYPE bool configStore(const char* aFile, int aLen);

// restore config form given file
LIB_CTYPE bool configRestore(const char* aFile, int aLen);

// ****************************************************************************
// templates
// ****************************************************************************

// create template with given name
LIB_CTYPE bool templateSet(const char* aName, int aNameLen, const char* aSource, int aSourceLen);

// delete template with given name
LIB_CTYPE bool templateClear(const char* aName, int aNameLen);

// reset all templates
LIB_CTYPE void templateReset();

// find template
LIB_CTYPE bool templateGet(const char* aName, int aNameLen, const char** aTemp, int* aTempLen);

// get templates as JSON string
LIB_CTYPE bool templatesToJSON( const char** aValue, int* aValueLen);

// get selected templates as JSON string, list in aNámes separated by ','
LIB_CTYPE bool templatesToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen);

// set templates from JSON string
LIB_CTYPE bool templatesFromJSON(const char* aValue, int aValueLen);

// store all templates in given file
LIB_CTYPE bool templateStore(const char* aFile, int aLen);

// restore all templates form given file
LIB_CTYPE bool templateRestore(const char* aFile, int aLen);


#endif