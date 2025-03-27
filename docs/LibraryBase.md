# Scripal modules for various programming languages

## Java, C#, Javascript, Python

**Notice:** All functions throw exceptions in case of fatal errors.<br>
You may read the last error with getErrMsg() any time, an empty string indicates no error occured.<br>
See constants used by Scripal library at the end of the document.<br> 
Remember to use proper escape sequences and nested strings, depending on the language used.<br>
Example: **obj.match("match 't' ")** 

## module management and global data

* get library version<br>
**string getVersion()**

* initialize library base once and set path to the Scripal shared library<br>
aLibPath is the entire path to the Scripal shared libary (**libscripal.so** under Linux, **scripal.dll** under Windows systems)  
**baseInit(aLibPath)**

* finalize library base at end of process (no need to call)<br>
**baseFinalize()**

* initialize library, call on every new process and! for every thread<br>
aConfigPath is the path to the config file to be used, set empty string for default<br>
aEnconding is the charater encoding to be used, set to default to use the language's default, example **scripal.ENC_DEFAULT**<br>
**libInit(string aConfigPath, int aEncoding)**

* end library process<br>
**libFinalize()**

* initialize logs<br>
  aLogChannel is "default" (disable logs), "stdout", "buffer" or path to file<br>
**logInit(aLogChannel)**

* get contents of log buffer<br>
**string getLogBuf()**

* clear log buffer<br>
**clearLogBuf()**

* get last error message<br>
**string getErrMsg()**

* get last explicit error, with more details<br>
**string getErrExp()**

* get last error position<br>
**int getErrPos()**

* clear last error<br>
**clearErr()**

## Scripal object TFScripal

A Scripal object is based on class TFScripal, constructors are  

* create new code object form given source<br>
**obj TFScripal("source", string aSource)**

* create new object for nearest match with given source and maximum distance<br>
**obj TFScripal("nearest", string aSource, float aMax)**

* create new object for block match with given source and maximum distance<br>
**obj TFScripal("block", string aSource, float aMax)**

## TFScripal methods

* match object against text<br>
  return true if at least one occurence was found<br>
**bool obj.match(string aText)**

* match object against file<br>
  return true if at least one occurence was found<br>
  aFileEncoding is the character encoding of the file, set to ENC_DEFAULT to guess the encoding<br>
**bool obj.matchFile(string aFile, int aFileEncoding)**

* replace all matched occurences with given string aReplace<br>
  return true if at least one occurence was found<br>
**bool obj.replace(string aText, string aReplace)**

* replace all occurences with given string aReplace in aFileIn and write into new file<br>
  aFileEncoding is the character encoding of the file, set to ENC_DEFAULT to guess the encoding<br>
  return true if at least one occurence was found<br>
**bool obj.replaceFile(string aFileIn, string aFileOut, string aReplace, int aFileEncoding)**

* match against text and split text at given occurences<br>
  return true if at least one occurence was found<br>
**bool obj.split(string aText)**

* match against file and split text at given occurences, write to new file aFileOut<br> 
  aFileEncoding is the character encoding of the file, set to ENC_DEFAULT to guess the encoding<br>
  return true if at least one occurence was found<br>
**bool obj.splitFile(string fileIn, string  aFileOut, int aFileEncoding)**

* search in many files / directories and collect results<br> 
  aPath is the directory to serach in, aDirExtension the file extension (use "*" to find all files)<br>
  set aRecursive to true to look in all subdirectories as well<br>
**bool obj.searchFiles(string aPath, string aDirExtension, bool aRecursive)**

## results and class TFResult 

every **TFScripal** object has an object **TFResult**, all indexeces are in the range 0,1...

**TFResult** {<br>
&nbsp;&nbsp;  **fileNames[]**  -> file name of result, when using searchFiles() method of TFScripal<br>
&nbsp;&nbsp;  **text[]**       -> results strings<br>
&nbsp;&nbsp;  **positions[]**  -> positions depending on config.posType<br>
&nbsp;&nbsp;  **ratings[]**    -> ratings when using nearest or block match, ratings are in the range 0 (no match)...1 (perfect match)<br>
&nbsp;&nbsp;  **tags[]**       -> tags set in Scripal source<br> 
&nbsp;&nbsp;  **size()**       -> no of results<br>}

access with **obj.results.text[i]** etc.

results may be exported in the following formats 
* result in JSON format<br>
**string obj.getResultJSON()**

* result in human readable format<br>
**string obj.getResultHRF()**

* result in CSV format<br> 
**string obj.getResultCSV()**

## configuration 

The configuration object **config** is a thread/process based singleton.

* set config according to locale<br> 
**config.setLocale(string aLocale)**

* get config as JSON string<br>
**string config.toJSON()**

* get config or parts as JSON string<br>
aKeys may be "all" to get the entire config or just parts like "debugRun, debugCompile"<br> 
**string config.toJSONVal(string aKeys)**

* set config or only selected parts from JSON string<br>
 example: config.fromJSON("{'debugRun' : true}")<br> 
**config.fromJSON(string aJSON)**

* store config in given file<br>
**config.store(string aFile)**

* restore config form given file<br>
**config.restore(string aFile)**

## templates

The templates object ist a global singleton, which is thread safe and can be accessed
by all processes and threads.  

* delete template with given name<br>
**templates.clear(string aName)**

* clear all templates<br>
**templates.reset()**

* get template with given name<br>
**string templates.get(string aName)**

* create template with given name<br>
**templates.set(string aName, string aSource)**

* store all templates in given file<br>
**template.store(string aFile)**

* restore all templates form given file<br>
**template.restore(string aFile)**

* get templates as JSON string<br>
**template.ToJSON(string aNames)**

* set templates from JSON string<br>
**template.fromJSON(string aJSON)**

## constants

### encodings
**ENC_DEFAULT  = 1** &nbsp;&nbsp; use Locales encoding <br>
**ENC_UTF8     = 2** &nbsp;&nbsp; UTF-8<br>
**ENC_UTF16L   = 3** &nbsp;&nbsp; UTF-16 little-endian<br>
**ENC_UTF16B   = 4** &nbsp;&nbsp; UTF-16 big-endian<br>
**ENC_UTF32L   = 5** &nbsp;&nbsp; UTF-32 little-endian<br>
**ENC_UTF32B   = 6** &nbsp;&nbsp; UTF-32 big-endian<br>
**ENC_ASCII    = 10** &nbsp;&nbsp; ASCII , Extended ASCII (up to 255) <br>
**ENC_CP932    = 11** &nbsp;&nbsp; CP932 DBCS, Japanese characters <br>
**ENC_CP936    = 12** &nbsp;&nbsp; CP93 6DBCS, simple Chinese characters <br>
**ENC_CP949    = 13** &nbsp;&nbsp; CP949 DBCS, Korean characters <br>
**ENC_CP950    = 14** &nbsp;&nbsp; CP505 DBCS, Chinese Big5 characters <br>
**ENC_LATIN1   = 30** &nbsp;&nbsp; West Europe Latin-1, ISO 8859-15 <br>
**ENC_LATIN2   = 31** &nbsp;&nbsp; Middle Europe Latin-2, ISO 8859-15 <br>
**ENC_LATIN9   = 32** &nbsp;&nbsp; West Europe Latin-9, ISO 8859-15 <br>
**ENC_WIN874   = 50** &nbsp;&nbsp; Windows Codepage 874, Thai characters <br>
**ENC_WIN1250  = 51** &nbsp;&nbsp; Windows Codepage 1251, Middle Europe <br>
**ENC_WIN1251  = 52** &nbsp;&nbsp; Windows Codepage 1251, Cyrillic <br>
**ENC_WIN1252  = 53** &nbsp;&nbsp; Windows Codepage 1252, West Europe <br>
**ENC_WIN1253  = 54** &nbsp;&nbsp; Windows Codepage 1253, Greek <br>
**ENC_WIN1254  = 55** &nbsp;&nbsp; Windows Codepage 1254, Turkish <br> 
**ENC_WIN1255  = 56** &nbsp;&nbsp; Windows Codepage 1255, Hebrew <br>
**ENC_WIN1256  = 57** &nbsp;&nbsp; Windows Codepage 1256, Arabic <br>
**ENC_WIN1257  = 58** &nbsp;&nbsp; Windows Codepage 1257, Baltic <br>
**ENC_WIN1258  = 59** &nbsp;&nbsp; Windows Codepage 1258, Vietnamese <br>

### matching algorithms (block and nearest)
**PATTERN_LEVEN_WORD = 1** &nbsp;&nbsp; Levenshtein distance match, use for matching words <br>
**PATTERN_LEVENPLUS_WORD = 2** &nbsp;&nbsp; optimized Levenshtein distance match, use for matching words <br>
**PATTERN_LEVEN = 3** &nbsp;&nbsp; Levenshtein distance match, use for matching phrases with any characters<br>
**PATTERN_JARO = 100** &nbsp;&nbsp; Jaro distance, use for matching phrases and text <br>
**PATTERN_JAROWINKLER = 101** &nbsp;&nbsp; Jaro-Winkler distance, use for matching phrases and text <br>
**PATTERN_JAROWINKLER_WORD = 102** &nbsp;&nbsp; Jaro-Winkler distance, use for matching words <br>

### result position types (set in config, element posType)
**POS_UTF8 = 1** &nbsp;&nbsp; positions relate to the position in the UTF-8 encoded text <br>
**POS_OFFSET = 2** &nbsp;&nbsp; result position relates to character encoding of the text and is byte offset! <br>
**POS_COUNT = 3** &nbsp;&nbsp; result position relates to character (UNICODE code point)  count <br>