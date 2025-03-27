# Scripal: C library, low-level interface

## using the library under Linux

Include **scripal/interface.hpp** in your code and link against scripal library.

## initialization and threads
Scripal is thread-safe. Every new process or thread attaching/loading the library must call 
libInit(); <br>
All functions returning buffers allocate the necessary memory and return the length.<br> 
Example:<br>
**void getVersion(const char\*\* aVersion, int\* aVersionLen)**<br>
Pass NULL on first call, you may then reuse **aVersion** for many calls.

The memory address returned in **aVersion** is handled by an internal buffer and must be freed with **deletePtr(\*aVersion)** !  

Example for gcc: <br>
gcc ./main.c -o ./main -I. -lscripal

<pre>
// ****************************************************************************
// // main initialization, call once for library
// ****************************************************************************
void baseInit();

// ****************************************************************************
// main finalization
// ****************************************************************************
void baseFinalize();

// ****************************************************************************
// intialize library in thread
// aConfigPath, aConfigPathLen - path to config file, if empty use default
// aEncoding                   - encoding used
// ****************************************************************************
void libInit(const char* aConfigPath, int aConfigPathLen, int aEncoding);

// ****************************************************************************
// intialize library in thread/process
// aConfigPath, aConfigPathLen - path to config file, if empty use default
// aEncoding                   - encoding used
// ****************************************************************************
void libInitC(const char* aConfigPath, int aConfigPathLen, int aEncoding);

// ****************************************************************************
// finalize library in thread
// ****************************************************************************
void libFinalize();

// ****************************************************************************
// intialize log system
// aLogChannel, aLogChannelLen  - log channel ("stdout", "buffer" or path to file)
// ****************************************************************************
void logInit(const char*  aLogChannel, int  aLogChannelLen);

// ****************************************************************************
// get log buffer
// aLogBuf, aLogBufLen  - contents of log buffer
// ****************************************************************************
void getLogBuf(const char** aLogBuf, int* aLogBufLen);

// ****************************************************************************
// clear log buffer
// ****************************************************************************
void clearLogBuf();

// ****************************************************************************
// print text on stdout
// aText, aTextLen - text in foreign encoding
// aEncoding - aText's encoding
// ****************************************************************************
void printText(const char* aText, int aTextLen, int aEncoding);

// ****************************************************************************
// delete char buffer owned by Scripal
// aPtr - buffer to free
// ****************************************************************************
void deletePtr(const char* aPtr);

// ****************************************************************************
// get last error message
// aError, aErrorLen - error (empty if none)
// ****************************************************************************
void getErrMsg(const char** aError, int* aErrorLen);

// ****************************************************************************
// get last explicit error
// aError, aErrorLen - error (empty if none)
// ****************************************************************************
void getErrExp(const char** aError, int* aErrorLen);

// ****************************************************************************
// get last error byte! position in original encoded text
// return - byte position
// ****************************************************************************
int getErrPos();

// ****************************************************************************
// clear last error 
// ****************************************************************************
void clearErr();

// ****************************************************************************
// get library version
// aVersion, aVersionLen - version
// ****************************************************************************
void getVersion(const char** aVersion, int* aVersionLen);

// ****************************************************************************
// set encoding of interface for current thread
// aEncoding - encoding ENC_xxx, set ENC_DEFAULT to use locale's encoding
// ****************************************************************************
void setEncoding(int aEncoding);

// ****************************************************************************
// create new Scripal object from code, if error occurs no object is created
//   and object ID is -1
// aSource, aSourceLen - source
// return object ID or -1, may set error
// ****************************************************************************
int newCodeObj(const char* aSource, int aSourceLen);

// ****************************************************************************
// create new Scripal object for nearest string match
// aSource, aSourceLen - source to look for
// aMax    - maximum distance (1.0 for perfect match) for result
// return object ID or -1, may set error
// ****************************************************************************
int newNearestObj(const char* aSource, int aSourceLen, double aMax);

// ****************************************************************************
// create new Scripal object for block string match
// aSource, aSourceLen - source to look for
// aMax    - maximum distance (1.0 for perfect match) for result
// return object ID or -1, may set error
// ****************************************************************************
int newBlockObj(const char* aSource, int aSourceLen, double aMax);

// ****************************************************************************
// delete Scripal object
// aObjID - object ID
// ****************************************************************************
void deleteObj(int aObjID);

// ****************************************************************************
// match object against text
// aObjID          - Scripal object ID
// aText,aTextLen  - text 
// return - true if at least one result occured
// ****************************************************************************
bool match(int aObjID, const char* aText, int aTextLen);

// ****************************************************************************
// match object against file
// aObjID          - Scripal object ID
// aFile,aFileLen  - name of file
// aFileEncoding   - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool matchFile(int aObjID, const char* aFile, int aFileLen, int aFileEncoding);

// ****************************************************************************
// match object against text and replace occurences
// aObjID                   - Scripal object ID
// aText,aTextLen           - text 
// aReplace,aReplaceLen     - replacement 
// return - true if at least one result occured, result[0] is replaced text
// ****************************************************************************
bool replace(int aObjID, const char* aText, int aTextLen, const char* aReplace, int aReplaceLen);

// ****************************************************************************
// match object against file and replace occurences
// aObjID                    - Scripal object ID
// aFileIn, aFileInLen       - source file 
// aFileOut,aFileOutLen      - output file 
// aReplace, aReplaceLen     - replacement 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool replaceFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, const char* aReplace, int aReplaceLen, int aFileEncoding); 

// ****************************************************************************
// split object 
// aObjID                 - Scripal object ID
// aText,aTextLen         - text 
// return - true if at least one result, result[x] is splitted text
// ***************************************************************************
bool split(int aObjID, const char* aText, int aTextLen);

// ****************************************************************************
// split file
// aObjID                - Scripal object ID
// aFileIn,aFileInLen    - source file 
// aFileOut,aFileOutLen  - output file 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occured
// ****************************************************************************
bool splitFile(int aObjID, const char* aFileIn, int aFileInLen, const char* aFileOut, int aFileOutLen, int aFileEncoding);

// ****************************************************************************
// search in files, results will be global (object ID = 0)
// aObjID                          - Scripal object ID
// aPath, aPathLen                 - path to search    
// aDirExtension,aDirExtensionLen  - file types to search for in directories (default: "*")
// aRecursive                      - if true, recurse through directories
// return - true if at least one result occured
// ****************************************************************************
bool searchFiles(int aObjID, const char* aPath, int aPathLen, const char* aDirExtension, int aDirExtensionLen , bool aRecursive);

// ****************************************************************************
// get object result size
// aObjID     - Scripal object ID 
// return     - no of results
// ****************************************************************************
int getResultSize(int aObjID);

// ****************************************************************************
// get object result specs
// aObjID        - Scripal object ID 
// aEncoding     - encoding
// aMatchType    - match type
// aPositionType - position type
// return        - no of results
// ****************************************************************************
LIB_CTYPE bool getResultSpecs(int aObjID, int aIndex, int* aEncoding, int* aMatchType, int* aPositionType);

// ****************************************************************************
// get object result string
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultStr(int aObjID, int aIndex, const char** aResult, int* aResultLen);

// ****************************************************************************
// get object result filename
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultFileName(int aObjID, int aIndex, const char** aResult, int* aResultLen);

// ****************************************************************************
// get object result tag
// aObjID               - Scripal object ID or 0 for global results
// aIndex               - result index  
// aResult, aResultLen  - result
// return     - true if result exists
// ****************************************************************************
bool getResultTag(int aObjID, int aIndex, const char** aResult, int* aResultLen);

// ****************************************************************************
// get object result byte position
// aObjID     - Scripal object ID or 0 for global results
// aIndex     - result index  
// aPosStart  - position start
// aPosEnd    - position end
// return     - true if result exists 
// ****************************************************************************
bool getResultPos(int aObjID, int aIndex, int* aPosStart, int* aPosEnd);

// ****************************************************************************
// get object result rating (block and nearest))
// aObjID     - Scripal object ID or 0 for global results
// aIndex     - result index  
// aRating    - rating 0..1 
// return     - true if result exists 
// ****************************************************************************
bool getResultRating(int aObjID, int aIndex, double* aRating);

// ****************************************************************************
// get result as JSON string
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result  
// return                 - true if result exists
// ***************************************************************************
bool getResultJSON(int aObjID, const char** aResult, int* aResultLen);

// ****************************************************************************
// get result as CSV separated string
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result 
// return                 - true if result exists
// ***************************************************************************
bool getResultCSV(int aObjID, const char** aResult, int* aResultLen);

// ****************************************************************************
// get result as string in human readable format
// aObjID                 - Scripal object ID or 0 for global results
// aResult, aResultLen    - result 
// return                 - true if result exists
// ***************************************************************************
bool getResultHRF(int aObjID, const char** aResult, int* aResultLen);

// ****************************************************************************  
// set config according to given locale 
// aLoc, aLocLen - locale
// ****************************************************************************
bool configSetLocale(const char* aLoc, int aLocLen);

// ****************************************************************************  
// get/set boolean key in config, aKey, aKeyLen: key aValue: value to set
// ****************************************************************************  
bool configGetBool(const char* aKey, int aKeyLen, bool* aValue);

bool configSetBool(const char* aKey, int aKeyLen, bool aValue);

// ****************************************************************************  
// get/set character key in config, aKey, aKeyLen: key aValue: value to set
// ****************************************************************************  
bool configGetChar(const char* aKey, int aKeyLen, char32_t* aValue);

bool configSetChar(const char* aKey, int aKeyLen, char32_t aValue);

// ****************************************************************************  
// get/set integer key in config, aKey, aKeyLen: key,  aValue: value to set
// ****************************************************************************  
bool configGetInt(const char* aKey, int aKeyLen, int* aValue);

bool configSetInt(const char* aKey, int aKeyLen, int aValue;

// ****************************************************************************  
// get/set string key in config, aKey, aKeyLen: key aValue, aValueLen: value to set
// ****************************************************************************  
bool configGetStr(const char* aKey, int aKeyLen, const char** aValue, int* aValueLen);

bool configSetStr(const char* aKey, int aKeyLen, const char* aValue, int aValueLen);

// ****************************************************************************
// get string from config vector, aKey,aKeyLen: key name, aIndex: index
// aValue,aValueLen: value to get
// ****************************************************************************
bool configGetPart(const char* aKey, int aKeyLen, int aIndex, const char** aValue, int* aValueLen);

// ****************************************************************************
// add string to config vector, aKey,aKeyLen: key name, aValue,aValueLen: value to add
// ****************************************************************************
bool configAddPart(const char* aKey, int aKeyLen, const char* aValue, int aValueLen);

// ****************************************************************************
// clear vector in config, aKey, aKeyLen: key 
// ****************************************************************************
bool configClear(const char* aKey, int aKeyLen);

// ****************************************************************************
// config as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool configToJSON( const char** aValue, int* aValueLen);

// ****************************************************************************
// parts of config as JSON string
// aValue,aValueLen: JSON string
// aNames, aNameLen : list of config values, separated by ','
// ****************************************************************************
bool configToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen);

// ****************************************************************************
// config as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool configFromJSON(const char* aValue, int aValueLen);

// ****************************************************************************
// store config in given file, aFile, aFileLen - path to file
// ****************************************************************************
bool configStore(const char* aFile, int aFileLen);

// ****************************************************************************
// restore config from given file, aFile, aFileLen - path to file
// ****************************************************************************
bool configRestore(const char* aFile, int aFileLen);

// ****************************************************************************
// store templates in given file, aFile, aFileLen - path to file
// ****************************************************************************
bool templateStore(const char* aFile, int aFileLen);

// ****************************************************************************
// restore templates from given file, aFile, aFileLen - path to file
// ****************************************************************************
bool templateRestore(const char* aFile, int aFileLen);

// ****************************************************************************
// set template source with given name, aName, aNamelen - name of template
// aSource, aSourceLen - template source
// ****************************************************************************
bool templateSet(const char* aName, int aNameLen, const char* aSource, int aSourceLen);

// ****************************************************************************
// clear template with given name, aName, aNamelen - name of template
// ****************************************************************************
bool templateClear(const char* aName, int aNameLen);

// ****************************************************************************
// clear all templates
// ****************************************************************************
void templateReset();

// ****************************************************************************
// find template with given name, aName, aNamelen - name of template
// aSource, aSourceLen - template source
// ****************************************************************************
bool templateGet(const char* aName, int aNameLen, const char** aSource, int* aSourceLen);

// ****************************************************************************
// all templates as JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool templatesToJSON( const char** aValue, int* aValueLen);

// ****************************************************************************
// some parts of templates as JSON string
// aValue,aValueLen: JSON string
// aNames, aNameLen : list of template names, separated by ','  
// ****************************************************************************
bool templatesToJSONVal( const char** aValue, int* aValueLen, const char* aNames, int aNamesLen);

// ****************************************************************************
// templates from JSON string,  aValue,aValueLen: JSON string
// ****************************************************************************
bool templatesFromJSON(const char* aValue, int aValueLen);

</pre>
