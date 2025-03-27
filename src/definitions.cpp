// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski 
// License:  BSL 1.0

#include "definitions.hpp"

namespace Scripal {
 
// nullstring for references
std::string nullString = "";

//  line end standard in OS
#ifdef LINUX
std::string lineEnd = "\n";
#endif
#ifdef WINDOWS
std::string lineEnd = "\r\n";
#endif


TFNum INFINTE_VALUE = std::numeric_limits<TFNum>::infinity();       // infinite value  
TFNum MAX_VALUE = (std::numeric_limits<TFNum>::max)();              // maximum possible value
TFNum MIN_VALUE = (std::numeric_limits<TFNum>::min)();              // minimum possible value

std::map<std::string, int> encNames = { \
  {"default", ENC_DEFAULT}, \
  {"utf8",    ENC_UTF8}, \
  {"utf16l",  ENC_UTF16L}, \
  {"utf16b",  ENC_UTF16B}, \
  {"utf32l",  ENC_UTF32L}, \
  {"utf32b",  ENC_UTF32B}, \
  {"ascii",   ENC_ASCII}, \
  {"cp932",   ENC_CP932}, \
  {"cp936",   ENC_CP936}, \
  {"cp949",   ENC_CP949}, \
  {"cp950",   ENC_CP950}, \
  {"latin1",  ENC_LATIN1}, \
  {"latin2",  ENC_LATIN2}, \
  {"latin9",  ENC_LATIN9}, \
  {"win847",  ENC_WIN874}, \
  {"win1250", ENC_WIN1250}, \
  {"win1251", ENC_WIN1251}, \
  {"win1252", ENC_WIN1252}, \
  {"win1253", ENC_WIN1253}, \
  {"win1254", ENC_WIN1254}, \
  {"win1255", ENC_WIN1255}, \
  {"win1256", ENC_WIN1256}, \
  {"win1257", ENC_WIN1257}, \
  {"win1258", ENC_WIN1258} \
};

std::string ERR_NONE                 = "";
std::string ERR_FATAL                = "fatal error";
std::string ERR_SYNTAX               = "syntax error";
std::string ERR_NO_GROUP_BEGIN       = "begin of group missing or empty group";
std::string ERR_NO_GROUP_END         = "end of group missing";
std::string ERR_NO_BLOCK_BEGIN       = "begin of block missing";
std::string ERR_NO_BLOCK_END         = "end of block missing";
std::string ERR_NO_TEXT_END          = "end of text missing";
std::string ERR_NO_NAME_END          = "end of name missing";
std::string ERR_NO_NAME              = "name not found";
std::string ERR_NO_RANGE_BEGIN       = "begin of range missing";
std::string ERR_NO_RANGE_END         = "end of range missing";
std::string ERR_NO_OPR_BEGIN         = "begin of operator missing";
std::string ERR_NO_OPR_END           = "end of operator missing or wrong bracket type";
std::string ERR_NO_OPD_END           = "end of operand missing";
std::string ERR_NO_TEMPLATE_END      = "end of template missing";
std::string ERR_NO_TEMPLATE          = "template not found";
std::string ERR_NESTED_TEMPLATE      = "too many nested templates";
std::string ERR_TEMPLATE_NAME        = "illegal template name";
std::string ERR_DUPLICATE_TEMPLATE   = "duplicate template name";
std::string ERR_NO_CONFIG_END        = "end of config setting missing";
std::string ERR_INVALID_CONFIG       = "invalid config setting";
std::string ERR_WRONG_OPERAND        = "wrong number or type of operand(s)";
std::string ERR_EMPTY_OPERAND        = "sub operand(s) missing";
std::string ERR_BROKEN_RANGE         = "broken range";
std::string ERR_NUMBER_RANGE         = "number out of range";
std::string ERR_OPD_TYPE             = "unexpected operand type";
std::string ERR_INVALID_CODE         = "invalid code";
std::string ERR_UNKNOWN_IDENTIFIER   = "unknown identifier";
std::string ERR_FILE                 = "file error";
std::string ERR_SYSTEM               = "system error";
std::string ERR_SOURCE               = "invalid source";
std::string ERR_PARAMETER            = "invalid parameter";

}