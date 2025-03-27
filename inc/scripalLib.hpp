// ***************************************************************************
// Scripal library header, include file for C++ usage and basic functions
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef SCRIPALLIB_HPP_INCLUDED
#define SCRIPALLIB_HPP_INCLUDED

#include "definitions.hpp"
#include "general.hpp"
#include "TFScripal.hpp"
#include "TFConfig.hpp"
#include "TFTemplate.hpp"

namespace Scripal { 
LIB_EXPORT TFConfig* getConfig();
LIB_EXPORT TFResults* getResults();
LIB_EXPORT std::string* getLogBuffer();
LIB_EXPORT std::string* getErrorMsg() ;
LIB_EXPORT std::string* getErrorExp() ;
LIB_EXPORT std::size_t  getErrorPos();
LIB_EXPORT TFTemplate*  getTemplates();
    
LIB_EXPORT void initBase();
LIB_EXPORT void finalizeBase();
LIB_EXPORT bool initLibrary(const std::string aConfigPath, int aEncoding = ENC_DEFAULT);
LIB_EXPORT void finalizeLibrary();
LIB_EXPORT bool initLog(const std::string aLogChannel);
 
LIB_EXPORT std::string conv(const std::u16string &aText);
LIB_EXPORT std::string conv(const std::u32string &aText);
LIB_EXPORT std::string conv(const std::wstring &aText);
LIB_EXPORT std::string conv(const std::string &aText, int aEncoding);

LIB_EXPORT bool fileConvert(const std::string &aFileIn, const std::string &aFileOut, int aFileEncodingIn, int aFileEncodingOut);

}  // end of namespace

#endif // SCRIPAL_HPP_INCLUDED
  
