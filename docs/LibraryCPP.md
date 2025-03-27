# Scripal: C++ library

## using the library under Linux

Include **scripal/scripalLib.hpp** in your code and link against scripal library.

compile with gcc (g++) or Microsoft MSVC (tested)
Remember that minimum c++ standard to be supported is C++17.

example for gcc:<br> 
g++ ./main.cpp -o ./main -I. -std=c++17 -lscripal 


## initialization and threads
Scripal is thread-safe. Every new process or thread attaching/loading the library must call 
<pre>
initLibrary(const std::string aConfigPath, int aEncoding = ENC_UTF8);
</pre>

Where aConfigPath ist the path to the config file, aEncoding the file's character encoding.
If aConfigPath is "", the default file is used. \
In case character encoding is omitted, the file is presumed to be UTF8 encoded. Else all input is converted from the given encoding to UTF-8 and all results converted from UTF-8 to the encoding of the environment.

At the end of a thread or entire process call 
<pre>
finalizeLibrary();
</pre>

If you use string containers other than std::string you may convert your strings using the functions:
<pre>
std::string conv(const std::u16string &aText);
std::string conv(const std::u32string &aText);
std::string conv(const std::wstring &aText);
</pre>

Logging may be switched on by calling 
<pre>
void initLog(const std::string aLogChannel);
</pre>
use **"stdout"**, **"buffer"** or a file path \
If buffer is used, getLogBuffer() will return the result.

The config object is unique to every thread and may be accessed with:
<pre>
TFConfig config;
</pre>
Templates are held in a global singleton:
<pre>
TFTemplate templates;
</pre>
The log buffer in case logging is set to a string buffer instead of stdout: 
<pre>
getLogBuffer();
</pre>
The last error message, message explained and error position:
<pre>
std::string* getErrorMsg();
std::string* getErrorExp();
std::size_t  getErrorPos();
</pre>


## encodings used in Scripal library
Scripal uses UTF-8 internally , but can deal with various encodings for input and output parameters.
If ENC_DEFAULT is specified, Scripal tries to guess the encoding used, the default behaviour.

<pre>
  identifier integer code
  "default", ENC_DEFAULT
  "utf8",    ENC_UTF8
  "utf16l",  ENC_UTF16L
  "utf16b",  ENC_UTF16B
  "utf32l",  ENC_UTF32L
  "utf32b",  ENC_UTF32B
  "ascii",   ENC_ASCII
  "cp932",   ENC_CP932
  "cp936",   ENC_CP936
  "cp949",   ENC_CP949
  "cp950",   ENC_CP950
  "latin1",  ENC_LATIN1
  "latin2",  ENC_LATIN2
  "latin9",  ENC_LATIN9
  "win847",  ENC_WIN874
  "win1250", ENC_WIN1250
  "win1251", ENC_WIN1251
  "win1252", ENC_WIN1252
  "win1253", ENC_WIN1253
  "win1254", ENC_WIN1254
  "win1255", ENC_WIN1255
  "win1256", ENC_WIN1256
  "win1257", ENC_WIN1257
  "win1258", ENC_WIN1258
</pre>

## class TFScripal

using the library in C++: 

There are 3 types of matching
 * using Scripal source (source match)
 * finding similar word or phrases in text using nearest algorithms (nearest match)
 * comparing text blocks and measuring how similar they are (block match)

the C++ namespace used is Scripal::  

## examples 

### find string in string
<pre>
#include &lt;stdio.h&gt; 
#include "scripal/scripalLib.hpp"

int main(int argc, char **argv) {
  using namespace Scripal;
  initLibrary(""); // put path to your own config file here, "" = use default

  std::cout << "version: " << Scripal::VERSION << "\n\n";

  // set time measurement on
  getConfig()->set("measureTime", true);

  // scripal object
  auto obj = new TFScripal("match find('235')");;
  if (obj->match("(234) 235-5678")) {
    std::cout << "results: " << obj->results.size() << " , '" << \
    obj->results.text[0] << "'" << "  position: " << obj->results.positions[0][0] << \
    ", " << obj->results.positions[0][1] << "\n\n";
  }
  exit(0);

};
</pre>

### split text separated by blanks
<pre>
#include &lt;stdio.h&gt; 
#include "scripal/scripalLib.hpp"
int main() {

std::string source = "match find( blank ); ifMatch end; loop;"; 
std::string text = "    test1 test2    test3   "; 

clearError();

TFScripal* obj = new TFScripal(source);
if (error != ERR_NONE)
  exit(0);;

obj->split(text);
for (auto value: obj->results.text) 
  output(value + "\n");

}
</pre>

### use nearest search for similar results
<pre>
#include  &lt;stdio.h&gt; 
#include "scripal/scripalLib.hpp"
int main() {
  std::string text = "find any tests where a test might be testing and tested stuff is the best of the rest";
  std::string source = "Test";
  TFScripal* obj;

  config.patternNearest = PATTERN_LEVENPLUS_WORD;
  obj = new TFScripal(source, 0.6, MATCH_NEAREST);
  obj->match(text);

  output("pattern levensthein plus word\n");
  output("source :'" + source + "'\n");
  output("text   :'" + text + "'\n\n");
  for (size_t i = 0; i < obj->results.positions.size(); ++i) {
    output("rating : " + std::to_string(obj->results.ratings[i]) + " , '" + \
    obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + \
    "," + std::to_string(obj->results.positions[i][1]) + "]\n");
  }
}
</pre>

## class TFScripal methods

<pre>
// ****************************************************************************
// create object for source match
// aSource   - source string
// aEncoding - source encoding 
// ****************************************************************************
TFScripal::TFScripal(const std::string &aSource, int aEncoding = ENC_UTF8);

// ****************************************************************************
// create object for distance matching (nearest or block match)
// aSource    - compare string 
// aDist      - critical distance for result [0..1]: 0 no similarity, 1 equal
// aMatchType - match algorithm (MATCH_NEAREST or MATCH_BLOCK)
// aEncoding  - source encoding
// ****************************************************************************
TFScripal::TFScripal(const std::string &aSource, double aDist, int aMatchType, int aEncoding = ENC_UTF8);
 
// ****************************************************************************
// match object against text
// aText     - string to parse 
// aEncoding - text encoding
// return - true if at least one result occured
// ***************************************************************************
bool TFScripal::match(const std::string &aText, int aEncoding = ENC_UTF8);
 
// ****************************************************************************
// match object against file
// aFile         - path to file 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return - true if at least one result occurred
// ****************************************************************************  
bool TFScripal::matchFile(const std::string &aText, int aFileEncoding = ENC_UTF8);

// ****************************************************************************
// match object against text and replace occurrences with given string
// single result gives modified text 
// aText     - string 
// aReplace  - string to replace with
// aEncoding - text encoding
// return    - true if at least one result occurred
// ****************************************************************************
bool TFScripal::replace(const std::string &aText, const std::string &aReplace, int aEncoding = ENC_UTF8);

// ****************************************************************************
// match object against file and replace occurrences with given string 
// aFileIn       - input file 
// aFileOut      - output file (must be different from input file)
//                 if empty append "_" to aFileIn  (aFileIn_)
// aReplace      - string to replace with 
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return        - true on success
// ****************************************************************************
bool TFScripal::replaceFile(const std::string &aFileIn, const std::string &aFileOut, const std::string &aReplace, int aFileEncoding = ENC_UTF8); 

// ****************************************************************************
// match object against text and split text at given matches, results will hold parts
// aText  - string 
// aEncoding - text encoding
// return - true if at least one result occured
// ****************************************************************************
bool split(const std::string &aText, int aEncoding = ENC_UTF8);   

// ****************************************************************************
// match object against file and split file, results will hold parts
// aFileIn       - input file 
// aFileOut      - output file (must be different from input file)
//                 if empty append "_" to aFileIn  (aFileIn_)
// aFileEncoding - encoding of file, ENC_DEFAULT if unknown
// return        - true if at least one result occured
// ****************************************************************************
bool splitFile(const std::string &aFileIn, const std::string &aFileOut, int aFileEncoding = ENC_UTF8);

// ****************************************************************************
// search for files where object matches, set global results!
// aPath         - path to search in
// aDirExtension - file types to search for in directories (default: "*")
// aRecursive    - if true, search all subdirectories
// return        - success (at least one result)
// ****************************************************************************
LIB_EXPORT bool fileSearch(const std::string &aPath, const std::string &aDirExtension, bool aRecursive);

// ****************************************************************************
// show object's result in human readable format, encode as desired 
// aEncoding - encoding of HRF format
// return    - result  
// **************************************************************************** 
std::string resultsToHRF(int aEncoding = ENC_UTF8); 

// ****************************************************************************
// show object's result in human readable format, encode as desired 
// aEncoding - encoding of result
// return    - result  
// **************************************************************************** 
std::string resultsToCSV(int aEncoding = ENC_UTF8);

// ****************************************************************************
// show object's result in JSON format, encode as desired 
// aEncoding - encoding of result
// return    - result  
// **************************************************************************** 
std::string resultsToJSON(int aEncoding = ENC_UTF8); 

// ****************************************************************************
// does object have a result? 
// return - true in case of result
// ****************************************************************************
bool isResult();

// ****************************************************************************
// is object source match?
// ****************************************************************************
bool isSource();

// ****************************************************************************
// is object nearest match?
// ****************************************************************************
bool isNearest();

// ****************************************************************************
// is object block match?
// ****************************************************************************
bool isBlock();

}
</pre>

## general library functions

<pre>
// ****************************************************************************
// thread/instance initialization
// aConfigPath  - path to config file, if empty use default
// aEncoding    - file encoding (UTF8 default) 
// ****************************************************************************
void initLibrary(const std::string aConfigPath, int aEncoding);

// ****************************************************************************
// finalize thread/instance 
// ****************************************************************************
void finalizeLibrary(); 

// ****************************************************************************
// init log system
// aLogChannel - log channel ("stdout", "buffer" or path to file)
// ****************************************************************************
void initLog(const std::string aLogChannel);

// ****************************************************************************
// read main config from file given
// aPath - path to file, if empty use default file
// aEncoding  - file encoding 
// ****************************************************************************
void initConfig(const std::string aPath, int aEncoding);

// ****************************************************************************
// convert UTF16x string to UTF8 std::string
// aText - UTF16 string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::u16string &aText);

// ****************************************************************************
// convert UTF32x string to UTF8 std::string
// aText - UTF32 string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::u32string &aText);

// ****************************************************************************
// convert wide string to UTF8 std::string
// aText - wide string in system endianess
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::wstring &aText);

// ****************************************************************************
// convert string in given encoding to UTF8 std::string
// aText - string 
// aEncoding - encoding of string
// return - UTF8 string for TFScripal
// ****************************************************************************
std::string conv(const std::string &aText, int aEncoding);

// ****************************************************************************
// convert file encoding 
// aFileIn          - input file 
// aFileOut         - output file 
// aFileEncodingIn  - encoding of file, ENC_DEFAULT if unknown
// aFileEncodingOut - encoding of file, ENC_DEFAULT if unknown
// true is succesfull
// ****************************************************************************
bool fileConvert(const std::string &aFileIn, const std::string &aFileOut, int aFileEncodingIn, int aFileEncodingOut);
</pre>

## Scripal configuration
[see explanation of configuration](/docs/Language.md#configuration) <br>

<pre>

class TFConfig {

bool debugCompile = false;      // compile source with debug option
bool debugRun = false;          // run code with debug option
bool showCode = false;          // show compiled code 
bool measureTime = false;       // if true measure time in milliseconds
bool useEmpty = false;          // use empty results in replace/split operation,true/false
bool translateDigits = false;   // if true translate digits in foreign languages to '0','1'... 
bool verboseResult = false;     // if true, make result verbose
  
char32_t decimalPoint = '.';    // decimal point
char32_t thousandsSep = ',';    // thousands separator
char32_t posSign = '+';         // mathematical plus sign
char32_t negSign = '-';         // mathematical minus sign

int encoding = ENC_UTF8;        // default encoding of environment, ENC_DEFAULT default 
int logEncoding = ENC_UTF8;     // default encoding used in logs, ENC_DEFAULT default  
int maxFileSize = 1000;         // max. size of file (file search) to load in MB 
                                // unless file is UTF-8 encoded 
int patternNearest = PATTERN_LEVENPLUS_WORD;   // default pattern for nearest search
int patternBlock = PATTERN_JARO;               // default pattern for block match
int positionType = POS_UTF8;                   // result position type

std::string logChannel = "stdout";      // log channel to use, stdout or a file
std::string pdfReader = "pdftotext";    // entire path to PDF parser , default: pdftotext
std::string frmReader = "pandoc";       // entire path to multi parser , default: pandoc
std::string appPath = system dependent; // entire path to application data

std::vector<std::string> sentenceEnd =  {".", "!", "?"};    // sentence end 
std::vector<std::string> separators  =  {" ", ".", "!", "?", ",", ";", ":", "/", "(", ")", "[", "]", "{", "}"};                      // word separators to identify word
std::vector<std::string> abbreviations; // abbreviations to distinguish word and end of sentence


// ****************************************************************************
// convert config to JSON format
// aVec - specify names to store or {"all"} for entire config
// return - config as JSON
// ****************************************************************************
std::string TFConfig::toJSON(std::vector<std::string> aVec);

// ****************************************************************************
// read config from JSON format (also only parts)
// aJSON - config as JSON
// return - true if valid JSON 
// ****************************************************************************
bool TFConfig::fromJSON(const std::string &aJSON);

// ****************************************************************************
// store config in given file 
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFConfig::store(const std::string &aFile);

// ****************************************************************************
// restore config from given file
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool TFConfig::restore(const std::string &aFile) ;

};
</pre>

## Scripal templates
[see explanation of templates](/docs/Language.md#templates) <br>

<pre>
class TFTemplate {

// ****************************************************************************
// get a template by name
// aName - template name
// return source
// ****************************************************************************
std::string TFTemplate::get(const std::string &aName);
 
// ****************************************************************************
// add or reset a template 
// aName   - template name
// aSource - template source
// ****************************************************************************
void set(const std::string &aName, const std::string &aSource);  

// ****************************************************************************
// delete a template 
// aName - template name
// ****************************************************************************
void clear(const std::string &aName);                             

// ****************************************************************************
// erase all templates 
// ****************************************************************************
void reset();      

// ****************************************************************************
// find a template by name
// aName - template name
// return source
// ****************************************************************************
std::string *find(const std::string &aName);                           

// ****************************************************************************
// store templates in given file
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool store(const std::string &aFile); 

// ****************************************************************************
// restore templates from given file
// aFile   - file name
// return  - true on success
// ****************************************************************************
bool restore(const std::string &aFile);

// ****************************************************************************
// convert templates to JSON format
// aVec - specify names to store or {"all"} for entire config
// return - config as JSON
// ****************************************************************************
std::string toJSON(std::vector<std::string> aVec);

// ****************************************************************************
// read templates from JSON format (also only parts)
// aJSON - templates as JSON
// ****************************************************************************
bool fromJSON(const std::string &aJSON);

}
</pre>
