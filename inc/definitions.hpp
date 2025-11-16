// ***************************************************************************
// common defintions, constants and classes for Scripal
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef DEFINITIONS_HPP_INCLUDED
#define DEFINITIONS_HPP_INCLUDED

// includes
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <set>
#include <chrono>
#include <fstream>		 
#include <ostream>	
#include <stdio.h>
#include <array>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <memory>
#include <numeric>
#include <filesystem>
#include <random>
#include <thread>
#include <string_view>
#include <mutex>
#include <shared_mutex>
#include <sys/types.h>

// define for extra debug info
// #define DEBUG_SCRIPAL 1

// compiler/OS dependencies 
#ifdef __linux__
  // inline control
  #define isInline // inline __attribute__((always_inline))
  // internal define for LINUX type OS (gcc used)
  #define LINUX 
  #include <unistd.h>
  #include <sys/types.h>
  #include <pwd.h>
  #define LIB_EXPORT 
  #define STRDUP strdup
 #endif 

#ifdef _WIN32
  // inline control
  #define isInline 
  // internal define for Windows 32/64 type OS (gcc used)
  #define WINDOWS 
  #if defined(_DLL)
    #define LIB_EXPORT __declspec(dllexport)
  #else
    #define LIB_EXPORT 
#endif 

  #include <windows.h>

  #define STRDUP _strdup
#endif 


// all interface classes and data available in namespace "Scripal"  
namespace Scripal {
  
// library version
const std::string VERSION  = "1.0.3";
// V_1.0.3
// fixed repeat and findat operands: no default range if none specified

//  temporary files 
#ifdef LINUX
  static std::string tmpFile = "/tmp/scripal.tmp";
#endif
#ifdef WINDOWS
  static auto tmpDir = std::filesystem::temp_directory_path();
  static std::string tmpFile = tmpDir.string() + "scripal.tmp";
#endif
#ifdef _MSC_VER
  #pragma warning( disable : 4222 )
  #pragma warning( disable : 4244 )
  #pragma warning( disable : 4267 )
  #pragma warning( disable : 4101 )
#endif

// OS dependent path separator
const std::string pathSep = std::string(1, std::filesystem::path::preferred_separator);

// **********************************************
// character encoding 
// **********************************************
LIB_EXPORT constexpr int ENC_DEFAULT  = 1;
LIB_EXPORT constexpr int ENC_UTF8     = 2;
LIB_EXPORT constexpr int ENC_UTF16L   = 3;
LIB_EXPORT constexpr int ENC_UTF16B   = 4;
LIB_EXPORT constexpr int ENC_UTF32L   = 5;
LIB_EXPORT constexpr int ENC_UTF32B   = 6;
LIB_EXPORT constexpr int ENC_ASCII    = 10;  //  ASCII , Extended ASCII (up to 255)
LIB_EXPORT constexpr int ENC_CP932    = 11;  //  CP932 DBCS, Japanese characters
LIB_EXPORT constexpr int ENC_CP936    = 12;  //  CP93 6DBCS, simple Chinese characters
LIB_EXPORT constexpr int ENC_CP949    = 13;  //  CP949 DBCS, Korean characters
LIB_EXPORT constexpr int ENC_CP950    = 14;  //  CP505 DBCS, Chinese Big5 characters
LIB_EXPORT constexpr int ENC_LATIN1   = 30;  //  West Europe Latin-1, ISO 8859-15
LIB_EXPORT constexpr int ENC_LATIN2   = 31;  //  Middle Europe Latin-2, ISO 8859-15
LIB_EXPORT constexpr int ENC_LATIN9   = 32;  //  West Europe Latin-9, ISO 8859-15
LIB_EXPORT constexpr int ENC_WIN874   = 50;  //  Windows Codepage 874, Thai characters
LIB_EXPORT constexpr int ENC_WIN1250  = 51;  //  Windows Codepage 1251, Middle Europe
LIB_EXPORT constexpr int ENC_WIN1251  = 52;  //  Windows Codepage 1251, Cyrillic
LIB_EXPORT constexpr int ENC_WIN1252  = 53;  //  Windows Codepage 1252, West Europe
LIB_EXPORT constexpr int ENC_WIN1253  = 54;  //  Windows Codepage 1253, Greek
LIB_EXPORT constexpr int ENC_WIN1254  = 55;  //  Windows Codepage 1254, Turkish
LIB_EXPORT constexpr int ENC_WIN1255  = 56;  //  Windows Codepage 1255, Hebrew
LIB_EXPORT constexpr int ENC_WIN1256  = 57;  //  Windows Codepage 1256, Arabic
LIB_EXPORT constexpr int ENC_WIN1257  = 58;  //  Windows Codepage 1257, Baltic
LIB_EXPORT constexpr int ENC_WIN1258  = 59;  //  Windows Codepage 1258, Vietnamese

LIB_EXPORT extern std::map<std::string, int> encNames;

// **********************************************
// grammar elements
// **********************************************

// source grammar types: operator, operand, condition, control, attribute, management
enum TFGrammarTypesID { SP_GRAMMAR_OPR, SP_GRAMMAR_OPD, SP_GRAMMAR_COND, SP_GRAMMAR_CTRL,  SP_GRAMMAR_ATTR };

// **********************************************
// operators

enum TFOperatorID     { SP_OPR_MATCH,SP_OPR_MATCHEND,\
                        SP_OPR_RESET, SP_OPR_END, SP_OPR_ENDLAST, SP_OPR_REPLACE, \
                        SP_OPR_NAME, SP_OPR_NAMEADD, SP_OPR_TAG, \
                        SP_OPR_SETMATCH, SP_OPR_SETNOMATCH, \
                        SP_OPR_LOOP, SP_OPR_TOTAL, SP_OPR_MOVEON, SP_OPR_EXIT, SP_OPR_NULL, \
                        _SP_OPR_END }; // leave end marker at end!

// **********************************************
// operands

/*  operand types 
 SP_OPD_NULL        : not specified / no operand
 SP_OPD_TEXT        : text operand
 SP_OPD_NAME        : name operand 
 SP_OPD_NUMBER      : number operand
 SP_OPD_RANGE       : range operand
 SP_OPD_BLOCK       : block operand
 SP_OPD_LOGICAL     : logical operand
*/ 
enum TFOpdTypeID      { SP_OPD_NULL, SP_OPD_TEXT, SP_OPD_NAME, SP_OPD_NUMBER, SP_OPD_RANGE, SP_OPD_BLOCK, SP_OPD_LOGICAL, _SP_OPD_END }; // leave end marker at end!

// logical operand match controls, bits
const uint16_t SP_CONTINUE     = 0x0001;   // continue with logical operand, no result yet
const uint16_t SP_BREAKLOOP    = 0x0002;   // break loop over logical operands, if SP_BREAKLOOP is set, retry from first suboperand
const uint16_t SP_BREAKOPD     = 0x0004;   // break entire operand
const uint16_t SP_MATCH        = 0x0008;   // result: operand matched
const uint16_t SP_NOMATCH      = 0x0010;   // result: operand did not match 
const uint16_t SP_MOVEON       = 0x0020;   // move on to next position in text 
const uint16_t SP_RESTORELOOP  = 0x0040;   // restore state to last main loop

// operand group logic (use operand types also als code IDs)
enum TFLogicID        { _SP_LOGIC_BEGIN =  _SP_OPD_END + 1, SP_LOGIC_ALL, SP_LOGIC_ANY, SP_LOGIC_EACH, \
                         SP_LOGIC_EVERY, SP_LOGIC_FIND, SP_LOGIC_FINDAT, SP_LOGIC_BREAK, \
                         SP_LOGIC_REPEAT, SP_LOGIC_ISNUMBER, SP_LOGIC_ISWORD, SP_LOGIC_ISUPPER, SP_LOGIC_ISLOWER, \
                        _SP_LOGIC_END };  // leave end marker at end!

enum TFBlockID        { _SP_BLOCK_BEGIN = _SP_LOGIC_END + 1, SP_BLOCK_SPACE, SP_BLOCK_BLANK, SP_BLOCK_CHAR, \
                         SP_BLOCK_LETTER, SP_BLOCK_DIGIT, SP_BLOCK_WORD, \
                         SP_BLOCK_BOS, SP_BLOCK_EOS, SP_BLOCK_BOL, SP_BLOCK_EOL, \
                         SP_BLOCK_BOT, SP_BLOCK_EOT, SP_BLOCK_BOW, SP_BLOCK_EOW, SP_BLOCK_BOMARK, SP_BLOCK_MOVE, \
                        _SP_BLOCK_END };  // leave end marker at end!

// attributes
enum TFAttributeID    {  SP_ATTR_NOCASE, SP_ATTR_NOT,  SP_ATTR_INT, SP_ATTR_PURE, SP_ATTR_HEX, SP_ATTR_OCT, SP_ATTR_BIN, SP_ATTR_LANNUM, \
                         SP_ATTR_AT, SP_ATTR_SKIP, SP_ATTR_TEST, SP_ATTR_TRY, SP_ATTR_LAST, SP_ATTR_NOTLAST, SP_ATTR_UNLESS, \
                        _SP_ATTR_END };  // leave end marker at end!

// match conditons
enum TFConditionID    { SP_COND_IFMATCH, SP_COND_IFNOMATCH, SP_COND_ANYMATCH, SP_COND_IFMATCHED, SP_COND_IFNOMATCHED, SP_COND_ELSE, \
                        _SP_COND_END };  // leave end marker at end!

 // control types 
enum TFCtrlID         { SP_CTRL_TEXT, SP_CTRL_NAME, SP_CTRL_GROUP_BEGIN, SP_CTRL_GROUP_END,  \
                          SP_CTRL_BLOCK_BEGIN,  SP_CTRL_BLOCK_END, SP_CTRL_INFINITE,  \
                          SP_CTRL_COMMA, SP_CTRL_SEMICOLON, \
                          SP_CTRL_RANGE_BEGIN, SP_CTRL_RANGE_END,  \
                          SP_CTRL_NL, SP_CTRL_CR, SP_CTRL_EOT, \
                          SP_CTRL_COMMENT, SP_CTRL_TEMP_BEGIN, SP_CTRL_TEMP_END, SP_CTRL_CONFIG, \
                          _SP_CONTROL_END };  // leave end marker at end!

// **********************************************
// special characters, strings and text positions
// **********************************************

// illegal UNICODE codepoint (when converting encodings) = -1 as signend type
constexpr char32_t illegalChar = (std::numeric_limits<char32_t>::max)();

// general escape character in text 
LIB_EXPORT constexpr char escChar = '\\';

// general character to separate operators in print output
LIB_EXPORT constexpr char separateChar = ';';

// nullstring for references
LIB_EXPORT extern std::string nullString;

//  line end standard in OS
#ifdef LINUX
LIB_EXPORT extern std::string lineEnd;
#endif
#ifdef WINDOWS
  LIB_EXPORT extern std::string lineEnd;
#endif
  
// definitions for character classes
enum TFDef : int     { CL_DIGIT, CL_NO_DIGIT, CL_BIN_DIGIT, CL_NO_BIN_DIGIT, CL_OCT_DIGIT, CL_NO_OCT_DIGIT, CL_HEX_DIGIT, CL_NO_HEX_DIGIT, \
                       CL_LETTER, CL_NO_LETTER, CL_ALPHANUM, CL_NO_ALPHANUM, CL_WHITE, CL_NO_WHITE, CL_UPPER, CL_LOWER, \
                       CL_SENT_END,  CL_NO_SENT_END, CL_SEPARATOR, CL_NO_SEPARATOR };

// CL_DIGIT         : digit,  isDigit()
// CL_NO_DIGIT      : no digit,  isDigit()
// CL_BIN_DIGIT     : binary digit,  isDigit()
// CL_NO_BIN_DIGIT  : no binary digit,  isDigit()
// CL_OCT_DIGIT     : octal digit,  isDigit()
// CL_NO_OCT_DIGIT  : no octal digit,  isDigit()
// CL_HEX_DIGIT     : hex digit,  isDigit()
// CL_NO_HEX_DIGIT  : no hex digit,  isDigit()
// CL_LETTER        : true letters,  isLetter()
// CL_NO_LETTER     : no true letters,  isLetter()
// CL_ALPHANUM      : A-Z, a-z and 0-9, isAlphaNum()
// CL_NO_ALPHANUM   : no A-Z, a-z and 0-9, isAlphaNum()
// CL_WHITE         : spaces(s), blank, test for " "
// CL_NO_WHITE      : no space(s), test for " "
// CL_UPPER         : uppercase characters, is Upper()
// CL_LOWER         : lowercase characters, isLower()
// CL_SENT_END      : sentence end sign  (configurable, config.sentenceEnd)   
// CL_NO_SENT_END   : no sentence end sign  (configurable, config.sentenceEnd)   
// CL_SEPARTOR      : separators ,.;'!?":([ (configurable, config.separators)  
// CL_NO_SEPARTOR   : no separators ,.;  (configurable, config.separators)


// **********************************************
// program operation types
// **********************************************
constexpr int OPR_MATCH        = 1;
constexpr int OPR_CONVERT      = 2;
constexpr int OPR_FILE_SEARCH  = 3;
// OPR_MATCH    match block, nearest or source;
// OPR_CONVERT  convert file;
// OPR_FILE_SEARCH  serach for files

// **********************************************
// result position types
// **********************************************
LIB_EXPORT constexpr int POS_UTF8          = 1;
LIB_EXPORT constexpr int POS_OFFSET        = 2;
LIB_EXPORT constexpr int POS_COUNT         = 3;
// POS_UTF8      : result position relates to UTF-8 or are NaN if other encoding is used (fast)
// POS_OFFSET    : result position relates to encoding of text and is byte offset
// POS_COUNT     : result position relates to character count

// **********************************************
// match pattern types
// **********************************************
LIB_EXPORT constexpr int MATCH_SOURCE      = 1;
LIB_EXPORT constexpr int MATCH_NEAREST     = 2;
LIB_EXPORT constexpr int MATCH_BLOCK       = 3;
// MATCH_SOURCE  : match compiled source and match fragments
// MATCH_NEAREST : find nearest words
// MATCH_BLOCK   : compare text blocks 

LIB_EXPORT constexpr int PATTERN_LEVEN_WORD = 1;
LIB_EXPORT constexpr int PATTERN_LEVENPLUS_WORD = 2;
LIB_EXPORT constexpr int PATTERN_LEVEN = 3;
LIB_EXPORT constexpr int PATTERN_JARO = 100;
LIB_EXPORT constexpr int PATTERN_JAROWINKLER = 101;
LIB_EXPORT constexpr int PATTERN_JAROWINKLER_WORD = 102;
// PATTERN_LEVEN_WORD             : levenshtein match
// PATTERN_LEVENPLUS_WORD         : levenshtein improved for word match
// PATTERN_LEVEN_PHRASE           : levenshtein, phrase match
// PATTERN_JARO                   : Jaro match
// PATTERN_JAROWINKLER            : Jaro Winkler match
// PATTERN_JAROWINKLER_WORD       : Jaro Winkler improved for word match
 
// **********************************************
// number values
// **********************************************

// default number type in Scripal used to represent numbers in source
#define TFNum double
static TFNum epsilon = std::numeric_limits<TFNum>::epsilon();

// default number type constants
constexpr TFNum NO_VALUE = std::numeric_limits<TFNum>::quiet_NaN();;
LIB_EXPORT extern TFNum INFINTE_VALUE;
LIB_EXPORT extern TFNum MAX_VALUE;          
LIB_EXPORT extern TFNum MIN_VALUE;        

// size_t constants for text positions
constexpr std::size_t MIN_SIZE_T = (std::numeric_limits<std::size_t>::min)();
constexpr std::size_t MAX_SIZE_T = (std::numeric_limits<std::size_t>::max)();

// nPos denotes a positon past end of string
LIB_EXPORT constexpr std::size_t nPos = std::string::npos;

// nVal denotes NaN  
LIB_EXPORT constexpr std::size_t nVal = std::string::npos - 1;

 // integer representing "no position or nPos" , if used only for positive values
LIB_EXPORT constexpr int noPos = -1;

// integer representing "NaN" , if used only for positive values
LIB_EXPORT constexpr int noVal = -2;

// **********************************************
// text identifier representing grammatical atom
// **********************************************

class TFIndentifier {
  public:  
  uint16_t grammarType = 0;    // SP_GRAMMAR_... type
  uint16_t codeID = 0;         // code ID of grammatical element
  std::string identifier = "";  // string identifier

  TFIndentifier() { };
  TFIndentifier(uint16_t aType, uint16_t aCodeID, const std::string &aIdentifier) { grammarType = aType; codeID = aCodeID; identifier = aIdentifier;};

  inline bool operator== (const TFIndentifier& aCmp) const { if ((aCmp.grammarType == grammarType) && (aCmp.codeID == codeID) && (aCmp.identifier == identifier)) return(true); else return(false); };
  inline bool operator!= (const TFIndentifier& aCmp) const { if ((aCmp.grammarType == grammarType) && (aCmp.codeID == codeID) && (aCmp.identifier == identifier)) return(false); else return(true); };
};

// list of all identifiers used
// SP_CTRL_xx must be single codepoint!
const std::vector<TFIndentifier> identifiers = { \
  {SP_GRAMMAR_OPR, SP_OPR_MATCH , "match"}, {SP_GRAMMAR_OPR, SP_OPR_MATCHEND , "matchend"}, \
  {SP_GRAMMAR_OPR, SP_OPR_RESET ,"reset"}, {SP_GRAMMAR_OPR, SP_OPR_END ,"end"}, {SP_GRAMMAR_OPR, SP_OPR_ENDLAST ,"endlast"}, {SP_GRAMMAR_OPR, SP_OPR_REPLACE ,"replace"}, \
  {SP_GRAMMAR_OPR, SP_OPR_NAME ,"name"}, {SP_GRAMMAR_OPR, SP_OPR_NAMEADD ,"nameadd"}, {SP_GRAMMAR_OPR, SP_OPR_TAG ,"tag"}, \
  {SP_GRAMMAR_OPR, SP_OPR_SETMATCH ,"setmatch"}, {SP_GRAMMAR_OPR, SP_OPR_SETNOMATCH ,"setnomatch"}, {SP_GRAMMAR_OPR, SP_OPR_LOOP ,"loop"}, \
  {SP_GRAMMAR_OPR, SP_OPR_TOTAL ,"total"}, {SP_GRAMMAR_OPR, SP_OPR_MOVEON ,"moveon"}, {SP_GRAMMAR_OPR, SP_OPR_EXIT ,"exit"},  {SP_GRAMMAR_OPR, SP_OPR_NULL ,"null"}, \
  \
  {SP_GRAMMAR_OPD, SP_LOGIC_ALL, "all"}, {SP_GRAMMAR_OPD, SP_LOGIC_ANY, "any"},  {SP_GRAMMAR_OPD, SP_LOGIC_EACH, "each"}, \
  {SP_GRAMMAR_OPD, SP_LOGIC_EVERY, "every"}, {SP_GRAMMAR_OPD, SP_LOGIC_REPEAT, "repeat"}, {SP_GRAMMAR_OPD, SP_LOGIC_FIND, "find"}, {SP_GRAMMAR_OPD, SP_LOGIC_FINDAT, "findat"}, \
  {SP_GRAMMAR_OPD, SP_LOGIC_BREAK, "break"}, {SP_GRAMMAR_OPD, SP_LOGIC_ISNUMBER, "isnumber"}, {SP_GRAMMAR_OPD, SP_LOGIC_ISWORD, "isword"}, \
  {SP_GRAMMAR_OPD, SP_LOGIC_ISUPPER, "isupper"}, {SP_GRAMMAR_OPD, SP_LOGIC_ISLOWER, "islower"}, \
  \
  {SP_GRAMMAR_OPD, SP_BLOCK_SPACE, "space"}, {SP_GRAMMAR_OPD, SP_BLOCK_BLANK, "blank"}, {SP_GRAMMAR_OPD, SP_BLOCK_CHAR, "char"},  \
  {SP_GRAMMAR_OPD, SP_BLOCK_LETTER, "letter"}, {SP_GRAMMAR_OPD, SP_BLOCK_DIGIT, "digit"}, {SP_GRAMMAR_OPD, SP_BLOCK_WORD, "word"}, \
  {SP_GRAMMAR_OPD, SP_BLOCK_BOS, "bos"}, {SP_GRAMMAR_OPD, SP_BLOCK_EOS, "eos"}, \
  {SP_GRAMMAR_OPD, SP_BLOCK_BOL, "bol"}, {SP_GRAMMAR_OPD, SP_BLOCK_EOL, "eol"}, \
  {SP_GRAMMAR_OPD, SP_BLOCK_BOT, "bot"}, {SP_GRAMMAR_OPD, SP_BLOCK_EOT, "eot"}, \
  {SP_GRAMMAR_OPD, SP_BLOCK_BOW, "bow"}, {SP_GRAMMAR_OPD, SP_BLOCK_EOW, "eow"}, \
  {SP_GRAMMAR_OPD, SP_BLOCK_BOMARK, "bomark"}, {SP_GRAMMAR_OPD, SP_BLOCK_MOVE, "move"},  \
  \
  {SP_GRAMMAR_ATTR,  SP_ATTR_NOCASE, "~"}, {SP_GRAMMAR_ATTR,  SP_ATTR_NOT, "!"}, \
  {SP_GRAMMAR_ATTR,  SP_ATTR_INT, "int"}, {SP_GRAMMAR_ATTR,  SP_ATTR_PURE, "pure"}, \
  {SP_GRAMMAR_ATTR,  SP_ATTR_HEX, "hex"}, {SP_GRAMMAR_ATTR,  SP_ATTR_OCT, "oct"}, {SP_GRAMMAR_ATTR,  SP_ATTR_BIN, "bin"}, \
  {SP_GRAMMAR_ATTR,  SP_ATTR_AT, "at"}, {SP_GRAMMAR_ATTR,  SP_ATTR_SKIP, "skip"}, {SP_GRAMMAR_ATTR,  SP_ATTR_TEST, "test"},  \
  {SP_GRAMMAR_ATTR,  SP_ATTR_TRY, "try"}, {SP_GRAMMAR_ATTR,  SP_ATTR_LAST, "last"}, {SP_GRAMMAR_ATTR,  SP_ATTR_NOTLAST, "notlast"}, {SP_GRAMMAR_ATTR,  SP_ATTR_UNLESS, "unless"}, \
  \
  {SP_GRAMMAR_COND, SP_COND_IFMATCH, "ifmatch"}, {SP_GRAMMAR_COND, SP_COND_IFNOMATCH, "ifnomatch"}, {SP_GRAMMAR_COND, SP_COND_ANYMATCH, "anymatch"}, \
  {SP_GRAMMAR_COND, SP_COND_IFMATCHED, "ifmatched"}, {SP_GRAMMAR_COND, SP_COND_IFNOMATCHED, "ifnomatched"}, {SP_GRAMMAR_COND, SP_COND_ELSE, "else"}, \
  \
  {SP_GRAMMAR_CTRL, SP_CTRL_RANGE_BEGIN ,"["}, {SP_GRAMMAR_CTRL, SP_CTRL_RANGE_END ,"]"}, {SP_GRAMMAR_CTRL, SP_CTRL_INFINITE ,"@"}, \
  {SP_GRAMMAR_CTRL, SP_CTRL_GROUP_BEGIN ,"("}, {SP_GRAMMAR_CTRL, SP_CTRL_GROUP_END ,")"}, {SP_GRAMMAR_CTRL, SP_CTRL_TEXT ,"'"}, {SP_GRAMMAR_CTRL, SP_CTRL_TEXT ,"\""}, {SP_GRAMMAR_CTRL, SP_CTRL_TEXT ,"`"},\
  {SP_GRAMMAR_CTRL, SP_CTRL_BLOCK_BEGIN ,"{"}, {SP_GRAMMAR_CTRL, SP_CTRL_BLOCK_END ,"}"}, {SP_GRAMMAR_CTRL, SP_CTRL_NAME ,"#"}, \
  {SP_GRAMMAR_CTRL, SP_CTRL_COMMA ,","}, { SP_GRAMMAR_CTRL, SP_CTRL_SEMICOLON ,";"}, {SP_GRAMMAR_CTRL, SP_CTRL_NL ,"\n"}, {SP_GRAMMAR_CTRL, SP_CTRL_CR ,"\r"}, \
  {SP_GRAMMAR_CTRL, SP_CTRL_COMMENT ,"//"}, {SP_GRAMMAR_CTRL, SP_CTRL_TEMP_BEGIN ,"<"},  {SP_GRAMMAR_CTRL, SP_CTRL_TEMP_END ,">"},  {SP_GRAMMAR_CTRL, SP_CTRL_CONFIG ,"%"}, 
};

// **********************************************
// class to represent the current state of 
// matches and results
// **********************************************

class TFState {
  public:
  std::size_t matchBegin = nPos;                          // pointer to start of single match region
  std::size_t matchEnd = nPos;                            // pointer to end of single match region
  std::size_t resultBegin = nPos;                         // pointer to start of result region
  std::size_t resultEnd = nPos;                           // pointer to end of result region
  std::size_t resultSize = 0;                             // no. of results so far
}; 

// **********************************************
// Scripal operand 
// **********************************************

class TFOperand {
public:
  // basic information
  uint16_t codeID;                           // operand code ID (any,all...) 
  uint16_t opdtype;                          // is operand null, text, logical...? 

  // operand entities, depending on operand type
  std::array<TFNum,2> values;                // number or number range as operand
  std::string text;                          // text operand 
  std::vector<TFOperand> operands;           // sub-operands in logical operand 
  bool attrs[_SP_ATTR_END];                  // attributes as bit mask according to enum type
  bool saveState;                            // if true, operand has attributes requiring state storage
  std::array<TFNum,2> attrValues;            // attribute number or number range

  // match information 
  std::size_t matchBegin;                    // start of match (begin of first code point or nPos for virtual)
  std::size_t matchEnd;                      // end of match (end of last code point or nPos for virtual)
  uint16_t matchCount;                       // number of matches for operands 
  uint16_t subCount;                         // number of matches in all sub operands in a single loop over operands
  uint16_t noMatchCount;                     // number of mismatches for operands
  bool hasMatch = false;                             // operand has matched? 
  bool breakOpd = false;                             // set to true to break operand immediately  
  std::size_t unlessPos;                     // if set, position where unless clause is valid 
  bool lastTry = false;                              // set to true if after attribute is involved in loop
  std::size_t lastPos;                       // set to true if previous after condition is fullfilled  
  bool lastLogic = false;                            // logic for last attribute 
  bool emptyRun = false;                             // if true, empty running loop detected
  std::size_t startPosPtr;                   // start position of operand (used only for special operations)                        

  // set default values for operand
  TFOperand() {codeID = SP_OPD_NULL; opdtype = SP_OPD_NULL; 
                 std::fill(std::begin(values), std::end(values), NO_VALUE); 
                 text = ""; operands.clear(); saveState = false; memset(attrs, 0, _SP_ATTR_END);
                 std::fill(std::begin(attrValues), std::end(attrValues), NO_VALUE); reset();
  };
                 
  // reset match information 
  isInline void reset() { matchBegin = nPos; matchEnd = nPos; matchCount = subCount = noMatchCount = 0; hasMatch = false; \
                          breakOpd = false; unlessPos = nPos; lastTry = false; lastPos = nPos; lastLogic = true; \
                          emptyRun = false; startPosPtr = nPos; };
};

// null operand
static TFOperand nullOperand ;
static TFIndentifier nullOprIdentifier = { SP_GRAMMAR_OPR, SP_OPR_NULL, "null"};

// dummy operand
static TFOperand dummyOperand ;

// **********************************************
// Scripal operator
// **********************************************

class TFOperator {
  public:
  uint16_t codeID;       // operator code ID 
  uint16_t condition;    // condition to execute operator
  size_t jumpTo;         // for if operations -> jump to address
  size_t count;          // count for program loops
  
  std::map<size_t, TFState> states;     // Scripal state to remember for given operator at code pos   
  
  TFOperand operand = nullOperand;      // sub operand(s) 
  
  TFOperator() {condition = SP_COND_ANYMATCH; codeID = SP_OPR_NULL; jumpTo = nPos; count = 0; };
  TFOperator(uint16_t aCondition, uint16_t aCode, TFOperand &aOperand) { condition = aCondition; codeID = aCode; operand = aOperand; jumpTo = nPos; count = 0; };
};
  
// null operator 
static TFOperator nullOperator;

// operator parameters, specifying operator syntax (waht operand types allowed)
class TFOprParms {
  public:
  std::bitset<_SP_OPD_END> opdtype = SP_OPD_NULL;     // main operand types for parameter, set in bitmask what is allowed
};

// **********************************************
// match results
// **********************************************
constexpr int floatPrecision = 8;                          // precison of float results 

class TFResults {
  public:
  int  encoding = ENC_UTF8;                                // encoding of text to be scanned and results 
  int  matchType = MATCH_SOURCE;                           // match type
  int  positionType = POS_UTF8;                            // result position type, UTF-8 or nan for non-UTF8 text
  std::vector<std::string> fileNames;                      // if associated -> file names
  std::vector<std::string> text;                           // actual result (all matches)
  std::vector<std::vector<std::size_t>> positions;         // result positions (start of match), positions in byte array, not character count!
  std::vector<TFNum> ratings;                              // ratings for pattern matching
  std::vector<std::string> tags;                           // tags associated with result (all matches)

  // initialize
  void init(int aMatchType, int aPositionType) {
    matchType = aMatchType;
    positionType = aPositionType;
    clear();
  }
  
  // copy from other
  void copy(TFResults& aResults) {
    clear();
    push_back(aResults);
  }

  // get size
  std::size_t size() const {return(fileNames.size());}; 

  // resize 
  void resize(std::size_t aSize) {
    fileNames.resize(aSize); text.resize(aSize); positions.resize(aSize); ratings.resize(aSize); tags.resize(aSize);
  };
  
  // add to end
  void push_back(const std::string &aFilename, const std::string &aText, const std::vector<std::size_t> &aPosition, \
                TFNum aRating, const std::string &aTag) {  
    fileNames.push_back(aFilename); text.push_back(aText); positions.push_back(aPosition); 
    ratings.push_back(aRating); tags.push_back(aTag); 
  };                              

  // add to end
  void push_back(const TFResults &aResults) {  \
    fileNames.insert(std::end(fileNames), std::begin(aResults.fileNames), std::end(aResults.fileNames)); 
    text.insert(std::end(text), std::begin(aResults.text), std::end(aResults.text)); 
    positions.insert(std::end(positions), std::begin(aResults.positions), std::end(aResults.positions)); 
    ratings.insert(std::end(ratings), std::begin(aResults.ratings), std::end(aResults.ratings)); 
    tags.insert(std::end(tags), std::begin(aResults.tags), std::end(aResults.tags)); 
  };

  // insert at index
  void insert(std::size_t aIndex, const std::string &aFilename, const std::string &aText, const std::vector<std::size_t> &aPosition, \
                TFNum aRating, const std::string &aTag) {  
    fileNames.insert(fileNames.begin() + aIndex, aFilename); text.insert(text.begin() + aIndex, aText); 
    positions.insert(positions.begin() + aIndex, aPosition); ratings.insert(ratings.begin() + aIndex, aRating);
    tags.insert(tags.begin()+ aIndex, aTag); };                              

  // insert at index
  void insert(std::size_t aIndex, const TFResults &aResults) { \
    fileNames.insert(std::begin(fileNames) + aIndex, std::begin(aResults.fileNames), std::end(aResults.fileNames)); 
    text.insert(text.begin() + aIndex, std::begin(aResults.text), std::end(aResults.text)); 
    positions.insert(positions.begin() + aIndex, std::begin(aResults.positions), std::end(aResults.positions)); 
    ratings.insert(ratings.begin() + aIndex, std::begin(aResults.ratings), std::end(aResults.ratings)); 
    tags.insert(tags.begin() + aIndex, std::begin(aResults.tags), std::end(aResults.tags)); 
  };
  
  // clear all esults
  void clear() { 
    fileNames.clear(); text.clear(); positions.clear(); 
    ratings.clear(); tags.clear(); };                              

  // delete last elements
  void pop_back() { 
    fileNames.pop_back(); text.pop_back(); positions.pop_back(); 
    ratings.pop_back(); tags.pop_back(); };         

  // erase elements from to just before
   void erase(std::size_t aStartIndex, std::size_t aEndIndex) {
     fileNames.erase(fileNames.begin() + aStartIndex, fileNames.begin() + aEndIndex); text.erase(text.begin() + aStartIndex, text.begin() + aEndIndex);
     positions.erase(positions.begin() + aStartIndex, positions.begin() + aEndIndex);
     ratings.erase(ratings.begin() + aStartIndex, ratings.begin() + aEndIndex); tags.erase(tags.begin() + aStartIndex, tags.begin() + aEndIndex); };
      
   // get result a index
  TFResults get(std::size_t aIndex) {
    TFResults result;
    if (aIndex < size()) {
      result.fileNames.push_back(fileNames[aIndex]);
      result.text.push_back(text[aIndex]);
      result.positions.insert(std::end(positions), std::begin(positions), std::end(positions));
      result.ratings.push_back(ratings[aIndex]);
      result.tags.push_back(tags[aIndex]);
    }
    return(result);
  }                     

  // set single! result a index
  TFResults set(std::size_t aIndex, const TFResults& aResults) {
    TFResults result;
    if (aIndex < size()) {
      fileNames[aIndex] = aResults.fileNames[0];
      text[aIndex] = aResults.text[0];
      positions.clear();
      positions.insert(std::end(positions), std::begin(aResults.positions), std::end(aResults.positions));
      ratings[aIndex] = aResults.ratings[0];
      tags[aIndex] = aResults.tags[0];
    }
    return(result);
  }                     
};
 
// **********************************************
// Scripal usage examples
// **********************************************

class TFExmpl {
  public:
  TFExmpl() {};
  TFExmpl(const std::string &aExplain , const std::string &aSource , const std::string &aText, const std::vector<std::string> &aResults) {\
  explain = aExplain; source = aSource; text = aText; results = aResults; };
  
  std::string explain;                // explain match method 
  std::string source;                 // Scripal source
  std::string text;                   // text to match
  std::vector<std::string> results;   // all expected results
  
  // ****************************************************************************
  // match result and example
  // aResults - result vector
  // return - "match" in case of match or error 
  // ****************************************************************************
  std::string matchResults(std::vector<std::string> aResults) {
    
    if (results.size() != aResults.size())
      return("no match: different sizes");

    if (results.size() == 0)
    return("match");

    if (aResults.size() == 0)
      return("no match: no result");

    for (std::size_t i = 0 ; i < aResults.size(); ++i) {
      if (results[i] != aResults[i])
        return("no match: difference at " + results[i] + " and " + aResults[i]);
    };    
    return("match");
  };

};

// **********************************************
// logchannel stdout is default
// **********************************************
enum {LOG_NULL, LOG_STDOUT, LOG_BUFFER, LOG_FILE};

// **********************************************
// error messages 
// **********************************************

LIB_EXPORT extern std::string ERR_NONE                 ;
LIB_EXPORT extern std::string ERR_FATAL                ;
LIB_EXPORT extern std::string ERR_SYNTAX               ;
LIB_EXPORT extern std::string ERR_NO_GROUP_BEGIN       ;
LIB_EXPORT extern std::string ERR_NO_GROUP_END         ;
LIB_EXPORT extern std::string ERR_NO_BLOCK_BEGIN       ;
LIB_EXPORT extern std::string ERR_NO_BLOCK_END         ;
LIB_EXPORT extern std::string ERR_NO_TEXT_END          ;
LIB_EXPORT extern std::string ERR_NO_NAME_END          ;
LIB_EXPORT extern std::string ERR_NO_NAME              ;
LIB_EXPORT extern std::string ERR_NO_RANGE_BEGIN       ;
LIB_EXPORT extern std::string ERR_NO_RANGE_END         ;
LIB_EXPORT extern std::string ERR_NO_OPR_BEGIN         ;
LIB_EXPORT extern std::string ERR_NO_OPR_END           ;
LIB_EXPORT extern std::string ERR_NO_OPD_END           ;
LIB_EXPORT extern std::string ERR_NO_TEMPLATE_END      ;
LIB_EXPORT extern std::string ERR_NO_TEMPLATE          ;
LIB_EXPORT extern std::string ERR_NESTED_TEMPLATE      ;
LIB_EXPORT extern std::string ERR_TEMPLATE_NAME        ;
LIB_EXPORT extern std::string ERR_DUPLICATE_TEMPLATE   ;
LIB_EXPORT extern std::string ERR_NO_CONFIG_END        ;
LIB_EXPORT extern std::string ERR_INVALID_CONFIG       ;
LIB_EXPORT extern std::string ERR_WRONG_OPERAND        ;
LIB_EXPORT extern std::string ERR_EMPTY_OPERAND        ;
LIB_EXPORT extern std::string ERR_BROKEN_RANGE         ;
LIB_EXPORT extern std::string ERR_NUMBER_RANGE         ;
LIB_EXPORT extern std::string ERR_OPD_TYPE             ;
LIB_EXPORT extern std::string ERR_INVALID_CODE         ;
LIB_EXPORT extern std::string ERR_UNKNOWN_IDENTIFIER   ;
LIB_EXPORT extern std::string ERR_FILE                 ;
LIB_EXPORT extern std::string ERR_SYSTEM               ;
LIB_EXPORT extern std::string ERR_SOURCE               ;
LIB_EXPORT extern std::string ERR_PARAMETER            ;

// **********************************************
// program/library options
// **********************************************

// program operations
constexpr int OPT_ARG0 = 0;
constexpr int OPT_ARG1 = 1;
constexpr int OPT_ARG2 = 2;
constexpr int OPT_ARG3 = 3;
constexpr int OPT_ARG4 = 4;
constexpr int OPT_ARG5 = 5;
constexpr int OPT_ARG6 = 6;
constexpr int OPT_ARG7 = 7;
constexpr int OPT_ARG8 = 8;
constexpr int OPT_ARG9 = 9;
constexpr int OPT_SOURCE = OPT_ARG0;
constexpr int OPT_TEXT = OPT_ARG1;
constexpr int OPT_SHOW_USAGE = 13;
constexpr int OPT_SOURCE_FILE = 14;
constexpr int OPT_TEXT_FILE = 15;
constexpr int OPT_CONFIG = 16;
constexpr int OPT_OUT_FILE = 17;
constexpr int OPT_NEAREST = 18;
constexpr int OPT_BLOCK = 19;
constexpr int OPT_EXAMPLE = 20;
constexpr int OPT_CSV = 21;
constexpr int OPT_JSON = 22;
constexpr int OPT_HRF = 23;
constexpr int OPT_VERBOSE = 24;
constexpr int OPT_CONVERT = 25;
constexpr int OPT_MAIN = 50;
constexpr int OPT_SINGLE = 51;
constexpr int OPT_RECURSIVE = 52;
constexpr int OPT_PATH = 53;
constexpr int OPT_EXTENSION = 54;
constexpr int OPT_REPLACE = 55;
constexpr int OPT_SPLIT = 56;
constexpr int OPT_TIME = 57;
constexpr int OPT_DEBUG_COMP = 100;
constexpr int OPT_DEBUG_RUN = 101;
constexpr int OPT_SHOW_CODE = 102;
constexpr int OPT_UNIT_TEST = 120;

// program option attributes
class TFOption {
  public:
  std::string name;  // name of option
  bool argument;     // does option have an argument ?
  int option;        // actual option OPT_xx
};

// all program options
class TFOptions {
  public:
  std::vector<TFOption> options = { \
                                    {"?", false, OPT_SHOW_USAGE}, \
                                    {"#", false, OPT_SOURCE}, {"#", false, OPT_TEXT}, \
                                    {"a", true, OPT_SOURCE_FILE}, {"sourcefile", true, OPT_SOURCE_FILE}, \
                                    {"b", true, OPT_TEXT_FILE}, {"textfile", true, OPT_TEXT_FILE}, \
                                    {"c", true, OPT_CONFIG}, {"config", true, OPT_CONFIG}, \
                                    {"d", true, OPT_OUT_FILE}, {"outfile", true, OPT_OUT_FILE}, \
                                    {"e", false, OPT_EXAMPLE}, {"example", false, OPT_EXAMPLE}, \
                                    {"f", true, OPT_NEAREST}, {"nearest", true, OPT_NEAREST}, \
                                    {"g", true, OPT_BLOCK}, {"block", true, OPT_BLOCK}, \
                                    {"k", false, OPT_CONVERT}, {"convert", false, OPT_CONVERT}, \
                                    {"m", false, OPT_MAIN}, {"main", false, OPT_MAIN}, \
                                    {"n", false, OPT_SINGLE}, {"single", false, OPT_SINGLE}, \
                                    {"o", false, OPT_RECURSIVE}, {"recursive", false, OPT_RECURSIVE}, \
                                    {"p", true, OPT_PATH}, {"path", true, OPT_PATH}, \
                                    {"q", true, OPT_EXTENSION}, {"extension", true, OPT_EXTENSION}, \
                                    {"r", true, OPT_REPLACE}, {"replace", true, OPT_REPLACE}, \
                                    {"s", false, OPT_SPLIT}, {"split", false, OPT_SPLIT}, \
                                    {"t", false, OPT_TIME}, {"time", false, OPT_TIME}, \
                                    {"u", false, OPT_CSV}, {"csv", false, OPT_CSV}, \
                                    {"v", false, OPT_JSON}, {"json", false, OPT_JSON}, \
                                    {"w", false, OPT_VERBOSE}, {"verbose", false, OPT_VERBOSE}, \
                                    {"x", false, OPT_DEBUG_COMP}, {"debugcomp", false, OPT_DEBUG_COMP}, \
                                    {"y", false, OPT_DEBUG_RUN}, {"debugrun", false, OPT_DEBUG_RUN}, \
                                    {"z", false, OPT_SHOW_CODE}, {"showcode", false, OPT_SHOW_CODE},
                                    {"unittest", true, OPT_UNIT_TEST} \
                                  };
                                  
  TFOption* find(std::string aName) { for (size_t i = 0; i < options.size(); ++i) {if (options[i].name == aName) return(&options[i]);}; return(nullptr);};
  TFOption* find(int aOption) { for (size_t i = 0; i < options.size(); ++i) {if (options[i].option == aOption) return(&options[i]);}; return(nullptr);};
};


};  // end of namespace

#endif // DEFINITIONS_HPP_INCLUDED
  
