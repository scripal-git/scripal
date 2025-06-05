// ****************************************************************************
// program to call Scripal functions in a command shell 
// ****************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemeski
// License:  BSL 1.0

#define NOMINMAX


#include "definitions.hpp"
#include "examples.hpp"
#include "mainTest.hpp"
#include "singleTest.hpp"
#include "general.hpp"
#include "scripalLib.hpp"
#include "text.hpp"
#include "unitTest.hpp"
#include "TFAccess.hpp"
#include "TFCompile.hpp"
#include "TFTemplate.hpp"
#include "TFScripal.hpp"
#include <chrono>

using namespace Scripal;

// pre defines
std::string runFileSearch(); 

// index of first example to run  
std::size_t startIndex = 0;
// max. no. of examples to run, starting at startIndex  
std::size_t noItems = 10000;

// run match
bool scripalRun = true;

bool exitStatus = 0;

// ***************************
// program states
 
// show usage
bool showUsage = false;

// run arguments
bool runArgument = false; 

// run examples
bool runExample = false; 

// run test instead of examples (maintest.hpp)
bool runMain = false; 

// run single test instead of examples (singletest.hpp)
bool runSingle = false; 

// run unit tests 
bool runUnitTest = false; 
std::string testType = "";

// ***************************
// control

// default operation type
int oprType = OPR_MATCH;

// default match type
int matchType = MATCH_SOURCE;

// if set use source and text from arguments
std::string source = ""; 
std::string text = ""; 

// if set use files for input and output
std::string sourceFile = ""; 
std::string textFile = ""; 

// path to config
std::string configFile = ""; 

// output file
std::string outFile = "";

// directoy to search
std::string dirPath = "";

// file types to search for in directories (dirPath)
std::string dirExtension = "*";

// critical distance for nearest or block match
double patternDist = 0.0;

// set user defined specific config 
bool setConfig = false;

// replace operation
bool runReplace = false;
std::string replaceStr = "";

// split operation
bool runSplit = false;

// if true -> recursive directory search
bool dirRecursive = false;

// result format
int resultFormat = OPT_HRF; 

// config shadow values
// 0 : set to false, 1 set to true, 2 leave as is
char measureTime = 2;
char verboseResult = 2; 
char debugCompile = 2;
char debugRun = 2;
char showCode = 2;

// ***************************
// verbosity

// show source before compilation
bool showSource = true;

// show text before running
bool showText = true;
 
// ***************************
// structures and objects

// examples to run
static std::vector<TFExmpl> examples;

// Scripal object
TFScripal* obj = nullptr;

// program options
std::map<TFOption*, std::string> opts;

// test arguments 
static std::vector<std::vector<std::string>> testCases = { \
  {"convert files", "-l", "-a", "#utf8#./test/source.text"} \
};


#include "scripalLib.hpp"
#include "algos.hpp"

// ****************************************************************************
// run test code before main execution
// ****************************************************************************
//#include <unistd.h>

void testCode() { 
  /*
  obj = new TFScripal("test",  0.5, MATCH_NEAREST);
  bool result = obj->matchFile("test.txt");
  std::cout << "match file results: " << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.fileNames[i] << "', '" << obj->results.text[i] << "'" << \
    "  [" << obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "]\n";
  }
  std::cout << "\n";
  delete obj;
  exit(0);
  auto obj = new TFScripal("match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
  bool result = obj->replace("Find the words apple, ape plus award.", "___");
  std::cout << "source replace result: " << std::to_string(result) << ", " << obj->results.text[0] << "\n";
  std::cout << "\n";
  delete obj;
  exit(0);

  const char* source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  const char* text = "Find the words apple, ape plus award.";
  const char* replaceTxt = "___";
  const char* result;
  int resultLen;
  int posStart; int posEnd;
  
  int objID = newCodeObj((const char*)source, strlen(source));
  if (replace(objID, (const char*)text, strlen(text), (const char*)replaceTxt, strlen(replaceTxt))) {
    for (int i = 0; i < getResultSize(objID); ++i) { 
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      // result is UTF16L so convert to console encoding
      output("result[" + std::to_string(i) + "]: " + std::string(result) + " pos: " + printPosition(posStart) + "," + printPosition(posEnd) + "\n", ENC_UTF8);
      // make sure to free string memory when no longer used
      deletePtr(result);
    }  
  } 
  deleteObj(objID);
  exit(0);
  
  TFScripal obj("match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
  obj.replace("Find the words apple, ape plus award.", "___");
  std::cout << obj.results.text[0] << "\n";
  exit(0);
  config.fromJSON("{\"negSign\": \"+\"}");
  exit(0);

  TFScripal obj("test", 0.5, MATCH_NEAREST);
  obj.match("Find words like test,tests and rest");

   std::cout << "'" << obj.resultsToCSV() << "'\n";
   std::cout << "'" << obj.resultsToJSON() << "'\n";
   std::cout << "'" << obj.resultsToHRF() << "'\n";
  exit(0);

  TFScripal obj("test", 0.5, MATCH_NEAREST);
  int encoding = ENC_UTF8;
  obj.splitFile("./wrapper/python/test.txt", "./wrapper/python/testout.txt");
 // std::cout << obj.results.size() <<  ": " << obj.results.text[0] << " , " << obj.results.text[1] << " , " << obj.results.text[2] << " , " << obj.results.text[3] << "\n"; 
  exit(0);

  // nearest search
  output("\nnearest search: look for words similar to 'test'\n\n");
  TFScripal obj("test", 0.5, MATCH_NEAREST);
  int encoding = ENC_UTF8;
  obj.matchFile("./wrapper/python/test.txt", encoding);
  output(obj.resultsToJSON(encoding) + "\n");

  const char* source = "test";
  const char* text = "Find the words like test, tests and TEST.";
  int sourceSize = std::char_traits<char>::length(source);
  int textSize = std::char_traits<char>::length(text);
  const char* result;
  int resultLen;
  int posStart; int posEnd;
  
  // get console encoding
  // set interface encoding
  config.positionType = POS_UTF8; 

  int objID = newNearestObj((const char*)source, sourceSize, 0.5);
  if (match(objID, (const char*)text, textSize)) {
    for (int i = 0; i < getResultSize(objID); ++i) { 
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      // result is UTF16L so convert to console encoding
      output("result[" + std::to_string(i) + "]: " + std::string(result) + " pos: " + printPosition(posStart) + "," + printPosition(posEnd) + "\n", ENC_UTF8);
      // make sure to free string memory when no longer used
      deletePtr(result);
    }  
  } 
  exit(0);


  const char* source = "match find(any (' ', ',', '.')); ifMatch end; loop;";
  const char* text = "Find the words apple, ape and award.";
  int sourceSize = std::char_traits<char>::length(source);
  int textSize = std::char_traits<char>::length(text);
  const char* result;
  int resultLen;
  int posStart; int posEnd;
  
  // get console encoding
  // set interface encoding
  config.positionType = POS_UTF8; 

  int objID = newCodeObj((const char*)source, sourceSize);


  if (split(objID, (const char*)text, textSize)) {
    for (int i = 0; i < getResultSize(objID); ++i) { 
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      // result is UTF16L so convert to console encoding
      output("result[" + std::to_string(i) + "]: " + std::string(result) + " pos: " + printPosition(posStart) + "," + printPosition(posEnd) + "\n", ENC_UTF8);
      // make sure to free string memory when no longer used
      deletePtr(result);
    }  
  } 
  exit(0);

  std::string text = "-6.4889";
  TFNum conv;
  bool attrs[_SP_ATTR_END];
  memset(attrs, 0, _SP_ATTR_END);
  textToNum(text, conv, attrs, config.decimalPoint, config.thousandsSep);
  std::cout << conv << "\n";

  text = "-6";
  textToNum(text, conv, attrs, config.decimalPoint, config.thousandsSep);
  std::cout << conv << "\n";

  text = "0";
  textToNum(text, conv, attrs, config.decimalPoint, config.thousandsSep);
  std::cout << conv << "\n";

  text = "108";
  textToNum(text, conv, attrs, config.decimalPoint, config.thousandsSep);
  std::cout << conv << "\n";

  text = "108000.56";
  textToNum(text, conv, attrs, '.', ' ');
  std::cout << std::to_string(conv) << "\n";

  text = "108.000,56";
  textToNum(text, conv, attrs, ',', '.');
  std::cout << std::to_string(conv) << "\n";

  text = "108000,56";
  textToNum(text, conv, attrs, ',', ' ');
  std::cout << std::to_string(conv) << "\n";

  exit(0);

 obj = new TFScripal("Test: Look for the word test in all these these tests.", 0.95, MATCH_BLOCK);
  obj->matchFile("./test/test.txt");
  std::cout << "end\n";
  exit(0);

  std::string aString = "jäslssdasdd";
  char32_t codePoint;
  
  std::chrono::time_point<std::chrono::steady_clock> sTime;
  sTime = recTime();
  auto lenEnc = getCPUTF8(static_cast<std::string>(aString), 0, codePoint);
  printTime(sTime);
  exit(0);

 
  totalTime = 0;
   std::chrono::time_point<std::chrono::steady_clock> sTime;
  sTime = recTime();
  //usleep(1000000);
  stopTime(sTime);
  std::cout << totalTime << "\n";
  exit(0);


  TFAccess fileAccess;
  fileAccess.assignFile("/opt/logs/BGL.log");
  std::size_t index = 0;
  std::size_t fileSize = fileAccess.size();
  std::cout << "start\n";
  while (index < fileSize) {
    index = findStr(fileAccess,"0x",index);
    if (index == nPos)
      break;;
    std::cout << index << "\n";
    ++index;  
  };  
  std::cout << "stop\n";

  exit(0);
cd /opt
  std::cout << pathSep << "\n";
  testHex();
  std::cout << levenDistance("tests whe", "tests wha") << "\n"; 
  std::cout << levenPlusDistance("tests whe", "tests wha") << "\n"; 
  TFNum num;
  std::cout << textUIntToNum("-344", num)<< "\n";
  exit(0);

  std::array<std::size_t ,2> block;
  block = templates.getBlock("  h   test';   '> ", 0, '<', '>',ctrlEndPoints);
  std::cout << block[0] << " , " << block[1] << "\n";
  exit(0);
  std::cout << std::to_string(compareText("./test/nearestTxt.txt" , "./test/test1.docx")) << "\n";
  exit(0);

  TFNum a ;
  bool res = textTFNumToNum("", a);
  std::cout << std::to_string(a)  << " , " << std::to_string(res) << "\n";
  exit(0);
  unitTestInterface();
exit(0); 
   decodeFile("./test/test1.docx");
   exit(0);

   decodeFile("./test/test-3.pdf");
   exit(0);

  std::string source = "match find('test'); end; loop;";
  TFScripal* obj;
  
  bool dirRecursive = true;
  std::string dirPath = "./test";
  std::string dirExtension = "txt";
  
  std::cout << "block match: \n\n";
  obj = new TFScripal("Test: Look for the word test in all these these tests.", 0.95, MATCH_BLOCK);
  obj.fileSearch(dirPath, dirExtension, dirRecursive);
  std::cout << resultsToJSON(encoding) << "\n";
  delete obj;

  exit(0);

//  printFiles("./test", "txt", true);
  std::cout << "start unit test/n"; 
  unitTestFileSearch();
  exit(0); */
}

TFOptions prgOptions;

// ****************************************************************************
// get program options and arguments and set map
// aArgs  - program arguments
// aOpts  - options map with { option, argument}
//          non value arguments are stored as 0,1..9 in aOpts
// return - error, empty if none
// ****************************************************************************
const std::string getOpt(const std::vector<std::string>& aArgs, std::map<TFOption*, std::string>& aOpts) {
  aOpts.clear();
  std::size_t index = 0, mainArg = 0;
  TFOption* opt = nullptr;

  if (aArgs.size() == 0)
    return("");
    
  while (index < aArgs.size()) {
    std::string arg = aArgs[index];
    trimText(arg);
 
    bool option = false, longOpt = false; 
    if (arg[0] == '-') {
      // new option
      option = true;
      if (arg.size() > 1) 
        if (arg[1] == '-')
          longOpt = true;
    } else {
      // no option -> main argument
      if (mainArg == 0) {
        aOpts.insert(std::make_pair(prgOptions.find(OPT_SOURCE), arg));
        ++mainArg;
      } else {  
        aOpts.insert(std::make_pair(prgOptions.find(OPT_TEXT), arg));
        ++mainArg;
      }
      ++index;
      continue;
    }        

    if (option && !longOpt) {
      // short option
      for (auto optVal : arg) {
        std::string curArg = std::string(1, optVal);
        if (curArg != "-") {
          opt = prgOptions.find(curArg);
          if (opt == nullptr) 
            // invalid option
            return("invalid option '" + curArg + "'");

          if (opt->argument) {
            // set new single option with associated argument
            if (index >= aArgs.size() - 1)
              return("missing argument for option '" + curArg + "'");
              
            aOpts.insert(std::make_pair(opt, aArgs[index + 1]));
            ++index; 
            break;
          } else {  
            // set new single option, no associated argument
            aOpts.insert(std::make_pair(opt ,""));
          }; 
        };
      };  
    } else if (option && longOpt) {
      // long option
      opt = prgOptions.find(arg.substr(2, nPos));
      std::size_t pos = arg.find("=");
      if (opt == nullptr) {
        if (pos == nPos) 
          return("invalid option '" + arg + "'");

        opt = prgOptions.find(arg.substr(2, pos - 2));
        if (opt == nullptr) 
          return("invalid option '" + arg + "'");
      };
 
      if (opt->argument) {
        if (pos == nPos) 
          return("missing argument for option '" + opt->name + "'");
        // set new long option with argument      
        aOpts.insert(std::make_pair(opt , arg.substr(pos + 1, nPos)));
        ++index;
      } else {  
        // set new long option, no associated argument
        aOpts.insert(std::make_pair(opt ,""));
      }  
    }
    if (opt)
      if (opt->option == OPT_SOURCE_FILE)
        ++mainArg;
    ++index; 
  }
  
  return("");
}

// ****************************************************************************
// set internals depending on command line options
// aOpts - options map with { option name, argument}
// return - error, empty if none
// ****************************************************************************
const std::string setOpt(std::map<TFOption*, std::string>& aOpts) {
  runArgument = true;
  for (auto const& opt : aOpts) {
    try {
      if (opt.first->option == OPT_SOURCE)  { 
        runArgument = true; 
        source = opt.second;  
      }
      if (opt.first->option == OPT_TEXT)  { 
        text = opt.second; 
      };   

      if (opt.first->option == OPT_SHOW_USAGE) { showUsage = true; };
      if (opt.first->option == OPT_SOURCE_FILE) {  sourceFile = opt.second; };
      if (opt.first->option == OPT_TEXT_FILE)  { textFile = opt.second;};
      if (opt.first->option == OPT_CONFIG) { setConfig = true; configFile = opt.second;};
      if (opt.first->option == OPT_OUT_FILE) { outFile = opt.second; };
      if (opt.first->option == OPT_EXAMPLE)  { runExample = true; runArgument = false;};
      if (opt.first->option == OPT_NEAREST) { oprType = OPR_MATCH; matchType = MATCH_NEAREST; patternDist = std::stod(opt.second); };
      if (opt.first->option == OPT_BLOCK) { oprType = OPR_MATCH; matchType = MATCH_BLOCK; patternDist = std::stod(opt.second); };
      if (opt.first->option == OPT_CONVERT) { oprType = OPR_CONVERT; };
      if (opt.first->option == OPT_MAIN) { runMain = true; runArgument = false;};
      if (opt.first->option == OPT_SINGLE)  { runSingle = true; runArgument = false;};
      if (opt.first->option == OPT_RECURSIVE)  { dirRecursive= true;};
      if (opt.first->option == OPT_PATH)  { oprType = OPR_FILE_SEARCH; dirPath = opt.second;};
      if (opt.first->option == OPT_EXTENSION)  { dirExtension = opt.second;};
      if (opt.first->option == OPT_REPLACE) { runReplace = true; replaceStr = opt.second; };
      if (opt.first->option == OPT_SPLIT)  { runSplit = true; };
      if (opt.first->option == OPT_TIME)  { measureTime = 1; };
      if (opt.first->option == OPT_CSV)  { resultFormat = OPT_CSV; };
      if (opt.first->option == OPT_JSON)  { resultFormat = OPT_JSON; };
      if (opt.first->option == OPT_VERBOSE)  { verboseResult = 1; };
      if (opt.first->option == OPT_DEBUG_COMP) { debugCompile = 1; };
      if (opt.first->option == OPT_DEBUG_RUN) { debugRun = 1; };
      if (opt.first->option == OPT_SHOW_CODE) { showCode = 1; };
      if (opt.first->option == OPT_UNIT_TEST) { runUnitTest = true; testType = opt.second; runArgument = false; };
    } catch(std::exception const& except) {
      setError( ERR_PARAMETER + ", '"+ opt.second + "'");
      exitStatus = 1;
    };
  };

  return("");  
}

// ****************************************************************************
// print program usage 
// ****************************************************************************
void printUsage() {
  output("scripal (VERSION " + VERSION + "): [options] [source] [text]\n\n"
               "\t-?                           : show version, usage and end\n" 
               "\t-a path (--sourcefile=path)  : path to file holding source, may hold encoding e.g. utf8//path \n" 
               "\t-b path (--textfile=path)    : path to file holding text, may hold encoding e.g. utf8//path\n" 
               "\t-c path (--config=path)      : use alternative config file, create if missing\n" 
               "\t                               'reset' to create/reset default config file\n"
               "\t-d path (--outfile=path)     : path to output file\n" 
               "\t-e (--example)               : run built-in examples\n" 
               "\t-f dist (--nearest=dist)     : no source-> use nearest algorithm to find text\n"
               "\t                               store results with distance >= dist, dist = 0..1, 1 = equal\n" 
               "\t-g dist (--block=dist)       : no source-> block comparison\n"
               "\t                               match if distance >= dist, 0 means not at all, 1 fully equal\n" 
               "\t-k (--convert)               : convert file -a (specify encoding in path enc//..) to\n"
               "\t                               file -b enc//..\n" 
               "\t-m (--main)                  : run main test (for developers)\n" 
               "\t-n (--single)                : run single test (for developers)\n" 
               "\t-o (--recursive)             : scan all files in subdirectories, use with -p\n" 
               "\t-p path (--path=path)        : scan all files in path\n" 
               "\t-q ext (--extension=ext)     : scan all files with given extension, use with -p\n" 
               "\t-r string (--replace=string) : replace occurrences with given string\n"
               "\t                               if in file, '_' is appended to output filename, filename_\n"
               "\t-s (--split)                 : split file\n" 
               "\t-t (--time)                  : show execution time\n" 
               "\t-u (--csv)                   : results as CSV, else human readable format\n"
               "\t-v (--json)                  : result as JSON, else human readable format\n"
               "\t-w (--verbose)               : results in verbose readable format\n"
               "\t                               ([file], result, tag, pos start, pos end)\n" 
               "\t-x (--debugcomp)             : debug compilation\n" 
               "\t-y (--debugrun)              : debug run\n" 
               "\t-z (--showcode)              : show compiled code\n" 
               "\n" ); 
};

// ****************************************************************************
// find argument associated with option
// aOpts   - options map with { option name, argument}
// aOption - option 
// ****************************************************************************
const std::string* findOpt(std::map<TFOption*, std::string>& aOpts, int aOption) {
  runArgument = true;
  for (auto const& opt : aOpts) {
    if (opt.first->option == aOption) 
      return(&opt.second);
  };
  return(nullptr);  
}

// ****************************************************************************
// program operation: run built-in examples
// ****************************************************************************
void runExamples() {
  bool matchFailed = false;

  // set result format to HRF else result comparison will fail
  config.set("resultFormat", OPT_HRF);
 
  try {
    for (size_t i = startIndex ; i < std::min((startIndex + noItems), examples.size()); ++i) {
      output("-------------------------------------------------------------------------------\n");
      output("example " +  std::to_string(i  + 1) + "\n\n");
      output(examples[i].explain + "\n\n");

      if (showSource)
        output("----- source -----\n\n" + examples[i].source + "\n\n");
      
      if (obj != nullptr)
         delete obj;

      clearError();
      obj = new TFScripal(examples[i].source);
 
      if (errorMsg != ERR_NONE) {
        matchFailed = true;
      } else {
        if (showText)
          if (examples[i].text.size() < 1000)  
            output("----- text -----\n\n" + examples[i].text + "\n\n");

        if (scripalRun) {
          clearError();
          obj->match(examples[i].text);
  
          if (errorMsg != ERR_NONE) { 
            matchFailed = true;
            output("\nmatch failed\n");
          } else {
            if (obj->hasMatched)
              output("----- results -----\n\n"); 

            for (size_t i = 0; i < obj->results.size(); ++i) {
              output("match: '" + obj->results.text[i] + "'");
              output(", pos[" + printPosition(obj->results.positions[i][0]) + \
                                       ", " + printPosition(obj->results.positions[i][1]) + "]");
              if (obj->results.tags[i] != "")
                output("  tag: '" + obj->results.tags[i] + "'");
              output("\n");  
            }

            std::string match  = examples[i].matchResults(obj->results.text); 
            if (match != "match") {
              output("match failed\n");
              matchFailed = true;
            }

            if (obj != nullptr) {
              output("\ntotal results: " + std::to_string(obj->results.size())); 
 
              if (config.measureTime) 
                output(", time(ms): " + std::to_string(obj->processTime)); 
              output("\n");   
            };
          }
        }; 
      };  

     output("\n");      
    };      

    output("-------------------------------------------------------------------------------\n");
    if (matchFailed)
      output("\nresult match failed\n\n");      
    else 
      output("\nall results match\n\n");      

  } catch(std::exception const& except) {
    output(std::string(except.what()) + "\n...\n");
    std::cin.get();
  }  

}

// ****************************************************************************
// program operation: run arguments
// ****************************************************************************
void runArguments() {
  std::string outStr;
  int sourceEncoding, textEncoding;
  
  clearError();

  // read input from files?
  if (sourceFile != "") {
    sourceEncoding = getFileEncoding(sourceFile);
    if (errorMsg != ERR_NONE) {
      exitStatus = 1;
      return;
    }

    if (!readFromFile(sourceFile, source)) {
      setError( ERR_FILE + ", cannot open '" + sourceFile + "'"); 
      exitStatus = 1;
      return;
    }
 
    if (oprType == OPR_MATCH) {
      if (sourceEncoding != encoding)
        encSrcToDst(source, sourceEncoding, encoding);
    }   
  }
  
  if ((textFile != "") && (textFile != "stdin")) {
    // match against file
    textEncoding = getFileEncoding(textFile);
    if (errorMsg != ERR_NONE) {
      exitStatus = 1;
      return;
    };
  };

  if (oprType == OPR_CONVERT) {
    // *********** conversion *************
    if (!fileConvert(sourceFile, textFile, sourceEncoding, textEncoding)) 
      exitStatus = 1;

    return;
  };
  
  if (oprType == OPR_FILE_SEARCH) {
    // *********** file search *************
    outStr = runFileSearch(); 
  };

  if (source == "") {
    exitStatus = 0;
    return;
  } 

  if (oprType == OPR_MATCH) {
    // *********** matching *************

    clearError();
  
    // create objects
    if ((matchType == MATCH_NEAREST) || (matchType == MATCH_BLOCK)) {
      obj = new TFScripal(source, patternDist, matchType, encoding);
    } else if (matchType == MATCH_SOURCE) {
      obj = new TFScripal(source, encoding);
    }  

    // error handling
    if (errorMsg != ERR_NONE) { 
      exitStatus = 1;
      return;
    }  
    
    clearError();
    if ((textFile != "") && (textFile != "stdin")) {
    // match against file
  
      if (runReplace) {
        obj->replaceFile(textFile, outFile, replaceStr, textEncoding);
      } else if (runSplit) {
        obj->splitFile(textFile, outFile, textEncoding);
      } else { 
        obj->matchFile(textFile, textEncoding);
        if (resultFormat == OPT_JSON)
          outStr = anyResultsToJSON(obj->results, encoding);
        else if (resultFormat == OPT_CSV)    
          outStr = anyResultsToCSV(obj->results, encoding);
        else   
          outStr = anyResultsToHRF(obj->results, encoding);
      }
    } else if (textFile == "") {
      // match against string
      if (runReplace) {
        if (obj->replace(text, replaceStr, encoding))
          outStr = utf8ToEnc(obj->results.text[0], encoding);
      } else if (runSplit) {
        obj->split(text, encoding);
          for (auto elm: obj->results.text) {
            outStr += utf8ToEnc(elm,encoding) + "\n";
          }  
      } else { 
        obj->match(text);
        if (resultFormat == OPT_JSON)
          outStr = anyResultsToJSON(obj->results, encoding);
        else if (resultFormat == OPT_CSV)   
          outStr = anyResultsToCSV(obj->results, encoding);
        else   
          outStr = anyResultsToHRF(obj->results, encoding);
      }
    }  
  }

  if (obj != nullptr) {
    outStr += "\ntotal results: " + std::to_string(obj->results.size()); 
 
    if (config.measureTime) 
       outStr += ", time(ms): " + std::to_string(obj->processTime); 
    outStr += "\n";   
  };     
 
  // error handling
  if (errorMsg != ERR_NONE) { 
    exitStatus = 1;
    return;
  }  
 
  // output to stdout or file
  if (outStr.size() != 0) {
    if (outFile == "")
      std::cout << outStr;
    else 
      if (!writeToFile(outFile, outStr)) {
        setError( ERR_FILE + ", cannot open '" + outFile + "'"); 
        exitStatus = 1;
        return;
      }
  }
  output("\n"); 
 //std::cout << "totalTIme: " << totalTime << "\n";
  
  return; 
}

// ****************************************************************************
// search files 
// return  - formatted result
// ****************************************************************************
std::string runFileSearch() {
  std::string results = "";
  TFScripal* obj  = nullptr;

  if (matchType == MATCH_SOURCE) {
    obj = new TFScripal (source);
  } else if ((matchType == MATCH_NEAREST) || (matchType == MATCH_BLOCK)) {
    obj = new TFScripal (source, patternDist, matchType);
  };
  
  if (errorMsg != ERR_NONE) {
    exitStatus = 1;
    return(results);
  };
  
  if (obj)
    if (!obj->fileSearch(dirPath, dirExtension, dirRecursive)) {
      exitStatus = 1;
      return(results);
    };

  if (obj->results.size() > 0) {
    if (resultFormat == OPT_JSON)
       results = obj->resultsToJSON(encoding);
    else if (resultFormat == OPT_CSV)   
       results = obj->resultsToCSV(encoding);
    else   
       results = obj->resultsToHRF(encoding);
  }

  delete(obj);

  return(results);
};

// ****************************************************************************
// run unit tests depending on map settings (unitTests)  
// ****************************************************************************
void runUnitTests() {
  std::string test = lowerText(testType);
  if (test == "lib")
    unitTestLib();
  if (test == "digits")
    unitTestDigits();
  if (test == "split")
    unitTestSplit();
  if (test == "replace")
    unitTestReplace();
  if (test == "nearest")
    unitTestNearest();
  if (test == "nearestalgo")
    unitTestNearestAlgo();
  if (test == "block")
    unitTestBlock();
  if (test == "config")
    unitTestConfig();
  if (test == "template")
    unitTestTemplate();
  if (test == "interface")
    unitTestInterface();
  if (test == "decodefile")
    unitTestDecodeFile();
  if (test == "filesearch")
    unitTestFileSearch();
  if (test == "filereplace")
    unitTestFileReplace();
};
  
// ****************************************************************************
// main application
// ****************************************************************************
int main(int argc, char** argv) {
  // measure time
  auto beg = std::chrono::high_resolution_clock::now();

  // *************************************
  // get console encoding and set ouput
  #ifdef LINUX 
  encoding = getBestEncoding(ENC_DEFAULT);
  #endif

  #ifdef WINDOWS
    /* built in to convert UTF-16 parameters 
    std::vector<std::string> argList(argv + 1, argv + argc);
    unsigned int consoleEnc = GetConsoleCP();
    if (consoleEnc != CP_UTF8) {
      if (consoleEnc == CP_UTF16) 
        encoding = ENC_UTF16;

      // encode arguments in UTF8
      for (std::size_t i= 0; i < argList.size(); ++i) {
        argList[i] = encToUtf8(argList[i], encoding);
      };
    };
    */
  
  // force UTF8 in console 
  std::setlocale(LC_CTYPE, ".UTF8");
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8); 
  encoding = getBestEncoding(ENC_DEFAULT);
  #endif
  
  std::vector<std::string> argList(argv + 1, argv + argc);
  //for (auto elm: argList) { std::cout << "'" << elm << "'\n";};

  // convert arguments from environment's encoding to UTF-8
  if (encoding != ENC_UTF8)
    for (auto elm: argList) 
      elm = encToUtf8(elm, encoding);
  
  // *************************************
  // parse arguments and analyze options

  std::string optResult;
  optResult = getOpt(argList, opts);
  if (optResult != "") {
    output(optResult + "\n");
    return(1);
  }

  if (opts.size() == 0) {
    // default operation : show usage
     showUsage = true;
  } else {   
    optResult = setOpt(opts); 
    if (exitStatus == 1)
      exit(exitStatus);
    if (optResult != "") {
      output(optResult + "\n");
      return(1);
    };
  };
  
  // init library
  initLibrary(configFile, encoding);
  if (errorMsg != ERR_NONE) {
    output(errorMsg + "\n");
    exitStatus = 1;
    exit(exitStatus);
  }  

  if (config.logChannel == "default")
    initLog("stdout");
  else 
    initLog(config.logChannel);

  if (errorMsg != ERR_NONE) {
    output(errorMsg + "\n");
    exitStatus = 1;
    exit(exitStatus);
  }  

  // change config settings requested by program args
  if (measureTime != 2)
    config.measureTime = measureTime;
  if (verboseResult != 2)  
    config.verboseResult = verboseResult;
  if (debugCompile != 2)  
    config.debugCompile = debugCompile;
  if (debugRun != 2)  
    config.debugRun = debugRun;
  if (showCode != 2)  
    config.showCode = showCode;

  // use for testing code before body executes
  testCode();

  // *******************
  // operation
  if (showUsage) {
    // operation: show usage
    printUsage(); 
    exit(exitStatus);
  }  

  if (runArgument) {
    // operation: run arguments
    runArguments(); 
  } else if (runExample) {
    // operation: run bulit-in examples 
    examples = userExamples;
    // create JSON file with examples
    writeToFile("examples.json", examplesToJSON(examples, encoding));
    runExamples();
  } else if (runMain) {
    // operation: run main test
    examples = testExamples;
    runExamples();
  } else if (runSingle) {  
    // operation: run single test
    examples = singleExample;
    runExamples();
  } else if (runUnitTest) {  
    // operation: run unit test
    runUnitTests();
  };  

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
  //std::cout << duration.count()<< " microseconds total \n";
  exit(exitStatus);
};
