// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "definitions.hpp"
#include "TFScripal.hpp"
#include "general.hpp"
#include "text.hpp"
#include "scripalLib.hpp"
#include "extern.hpp"
#include <csignal>  

namespace Scripal {
 
TFConfig* getConfig() { return(&config); };
std::string* getLogBuffer() { return(&logBuffer); };
std::string* getErrorMsg() { return(&errorMsg); };
std::string* getErrorExp() { return(&errorExp); };
std::size_t  getErrorPos() { return(errorPos); };
TFTemplate* getTemplates() { return(&templates); };

// ****************************************************************************
// main initialization
// ****************************************************************************
void initBase() {
  // set important control characters as abbreviations
  ctrlInfinite   = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_INFINITE);
  ctrlRangeBegin = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_RANGE_BEGIN);
  ctrlRangeEnd   = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_RANGE_END);
  ctrlBlockBegin = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_BLOCK_BEGIN);
  ctrlBlockEnd   = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_BLOCK_END);
  ctrlName       = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_NAME);
  ctrlSemicolon  = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_SEMICOLON);
  ctrlNL         = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_NL);
  ctrlCR         = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_CR);
  ctrlTempBegin  = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_TEMP_BEGIN);
  ctrlTempEnd    = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_TEMP_END);
  ctrlConfig     = findIdentifier(SP_GRAMMAR_CTRL,  SP_CTRL_CONFIG);
  ctrlEndPoints  = {{getCP(ctrlNL, 0)}, {getCP(ctrlCR, 0)}, {getCP(ctrlSemicolon, 0)}};

  // define main operand types for operators
  oprParms[SP_OPR_MATCH].opdtype.set(SP_OPD_TEXT); oprParms[SP_OPR_MATCH].opdtype.set(SP_OPD_NAME); 
  oprParms[SP_OPR_MATCH].opdtype.set(SP_OPD_NUMBER); oprParms[SP_OPR_MATCH].opdtype.set(SP_OPD_BLOCK); 
  oprParms[SP_OPR_MATCH].opdtype.set(SP_OPD_LOGICAL);

  oprParms[SP_OPR_MATCHEND].opdtype = oprParms[SP_OPR_MATCH].opdtype;
  
  oprParms[SP_OPR_RESET].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_END].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_ENDLAST].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_REPLACE].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_NAME].opdtype.set(SP_OPD_NAME);

  oprParms[SP_OPR_NAMEADD].opdtype.set(SP_OPD_NAME);

  oprParms[SP_OPR_TAG].opdtype.set(SP_OPD_TEXT);

  oprParms[SP_OPR_SETMATCH].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_SETNOMATCH].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_LOOP].opdtype.set(SP_OPD_NULL);

  oprParms[SP_OPR_TOTAL].opdtype.set(SP_OPD_NULL); oprParms[SP_OPR_TOTAL].opdtype.set(SP_OPD_NUMBER);

  oprParms[SP_OPR_MOVEON].opdtype.set(SP_OPD_NULL); oprParms[SP_OPR_MOVEON].opdtype.set(SP_OPD_NUMBER);

  oprParms[SP_OPR_EXIT].opdtype.set(SP_OPD_NULL);
 
  oprParms[SP_OPR_NULL].opdtype.set(SP_OPD_NULL);
  
  initialized = true;
};

// ****************************************************************************
// main finalization
// ****************************************************************************
void finalizeBase() {
};

// ****************************************************************************
// thread/instance initialization
// aConfigPath  - path to config file, if empty use default
// aEncoding    - file encoding, if ENC_DEFAULT use config value
//                else override config
// ****************************************************************************
bool initLibrary(const std::string aConfigPath, int aEncoding) {
  if (!initialized)
    initBase();
  
  errorMsg = ERR_NONE;
  errorExp = ERR_NONE;
  errorPos = nPos;
  templateLevel = 0;

  initConfig(aConfigPath, ENC_UTF8);
  if (errorMsg != ERR_NONE)
    return(false);

  initLog(config.logChannel);
  if (errorMsg != ERR_NONE)
    return(false);


  // character enconding environment and files 
  // aEncoding overrides config.encoding
  if (aEncoding == ENC_DEFAULT) {
    encoding = config.encoding;
    if (encoding == ENC_DEFAULT)
      encoding = getBestEncoding(encoding);
  } else {
    encoding = aEncoding;
  }  
  if (config.encoding != ENC_DEFAULT) {
    encoding = config.encoding;
  };  

  // character enconding for logs
  logEncoding = config.logEncoding;
  if (logEncoding == ENC_DEFAULT)
    logEncoding = encoding;

  return(true);
};

// ****************************************************************************
// finalize thread/instance 
// ****************************************************************************
void finalizeLibrary() {

}

// ****************************************************************************
// init log system
// aLogChannel - log channel ("stdout", "buffer" or path to file)
// return      - true if succesful  
// ****************************************************************************
bool initLog(const std::string aLogChannel) {
  if(aLogChannel == "default") {
    logChannel = LOG_NULL;
    return(true);
  } else if(aLogChannel == "stdout") {
    logChannel = LOG_STDOUT;
    return(true);
  } else if(aLogChannel == "buffer") {
    logChannel = LOG_BUFFER;
    logBuffer = "";
    return(true);
  } else {
    if (aLogChannel.find('.' ,0) == std::string::npos) {
      setError(ERR_FILE + ", invalid log file name " + aLogChannel + "\n");
      return(false);
    }  
    try {
      fileChannel.open(aLogChannel);
      logChannel = LOG_FILE;
      return(true);
    } catch (const std::exception &aExc) {
      logChannel = LOG_STDOUT;
      setError(ERR_FILE + ", cannot open log file " + aLogChannel + "\n");
      return(false);
    }  
  }
}

// ****************************************************************************
// convert UTF16x string to UTF8 std::string
// aText - UTF16 string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::u16string &aText) { 
  if (isBigEndian)
    return(encSrcToDst((const char*)aText.c_str(), aText.size() * 2, ENC_UTF16B, ENC_UTF8));
  else
    return(encSrcToDst((const char*)aText.c_str(), aText.size() * 2, ENC_UTF16L, ENC_UTF8));
}

// ****************************************************************************
// convert UTF32x string to UTF8 std::string
// aText - UTF32 string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::u32string &aText) { 
  if (isBigEndian)
    return(encSrcToDst((const char*)aText.c_str(), aText.size() * 4, ENC_UTF32B, ENC_UTF8));
  else
    return(encSrcToDst((const char*)aText.c_str(), aText.size() * 4, ENC_UTF32L, ENC_UTF8));
}
    
// ****************************************************************************
// convert wide string to UTF8 std::string
// aText - wide string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::wstring &aText) { 
  std::size_t mul = sizeof(wchar_t);

  if (mul == 4) {
    if (isBigEndian)
      return(encSrcToDst((const char*)aText.c_str(), aText.size() * mul, ENC_UTF32B, ENC_UTF8));
    else
      return(encSrcToDst((const char*)aText.c_str(), aText.size() * mul, ENC_UTF32L, ENC_UTF8));
  } else {
    if (isBigEndian)
      return(encSrcToDst((const char*)aText.c_str(), aText.size() * mul, ENC_UTF16B, ENC_UTF8));
    else
      return(encSrcToDst((const char*)aText.c_str(), aText.size() * mul, ENC_UTF16L, ENC_UTF8));
  }    
}

// ****************************************************************************
// convert string in given encoding to UTF8 std::string
// aText - string 
// aEncoding - encoding of string
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::string &aText, int aEncoding) { 
  return(encSrcToDst((const char*)aText.c_str(), aText.size(), aEncoding, ENC_UTF8));
}

// ****************************************************************************
// convert file encoding 
// aFileIn          - input file 
// aFileOut         - output file 
// aFileEncodingIn  - encoding of file, ENC_DEFAULT if unknown
// aFileEncodingOut - encoding of file, ENC_DEFAULT if unknown
// true is succesfull
// ****************************************************************************
bool fileConvert(const std::string &aFileIn, const std::string &aFileOut, int aFileEncodingIn, int aFileEncodingOut) {
  std::string fileContent, source, dest;
  TFAccess text; 
  std::ofstream outFile(aFileOut, std::ios_base::binary);

  clearError();
  text.assignFile(aFileIn);
  
  if (errorMsg != ERR_NONE) 
    return(false);

  if (!outFile.is_open()) {
    setError( ERR_FILE + ", cannot open '" + aFileOut + "'"); 
    return(false);
  }
  
  if (aFileEncodingIn == ENC_DEFAULT) { 
    // find out encoding 
    aFileEncodingIn = getBOMEncoding(text, 0);
    
    // if no BOM -> use config's default ecoding
    if (aFileEncodingIn == ENC_DEFAULT)
      aFileEncodingIn = config.encoding;
  };
 
  // read file entirely to convert to UTF-8
  if ((int)text.size() > config.maxFileSize * 1000000) {    
    setError( ERR_FILE + ", file too big to scan '" + aFileIn + "'"); 
    return(false);
  };      

  if (aFileEncodingIn != ENC_UTF8) {
    fileContent = text.read();
    source = encToUtf8((std::string_view)fileContent, aFileEncodingIn);
  } else {
    // read file 
    source = text.read();
  }
  
  if (aFileEncodingOut != ENC_UTF8) {
    // convert to output format
    dest = utf8ToEnc(source, aFileEncodingOut);
  } else {
    // set destination from source
    dest = source;
  }

  outFile.write(dest.c_str(), dest.size());

  return(true);
}

}
 
