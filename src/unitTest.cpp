// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#include "TFScripal.hpp"
#include "interface.hpp"
#include "algos.hpp"
#include "unitTest.hpp"
#include "general.hpp"
#include "text.hpp"
#include "scripalLib.hpp"

namespace Scripal {

// ****************************************************************************
// test C++ library functions
// ****************************************************************************
void unitTestLib() {
  output("library test: does type and encoding conversion work?\n\n");
  output("UTF16 match should return 'test16' \n");

  std::u16string source16 = u"match find ('test16')";
  TFScripal* obj = new TFScripal(conv(source16));

  obj->match(conv(u"look for test16 in this text"));
  if (obj->hasMatched)
    output("UTF16 match: '" +  obj->results.text[0] + "'\n\n");
  else
    output("no UTF16 match\n\n");
 
  delete obj;
  
  // *************************************************
  
  std::u32string source32 = U"match find ('test32')";
  output("UTF32 match should return 'test32' \n");

  obj = new TFScripal(conv(source32));

  obj->match(conv(U"look for test32 in this text"));

  if (obj->hasMatched)
    output("UTF32 match: '" + obj->results.text[0] + "'\n\n");
  else
    output("no UTF32 match\n\n");

  delete obj;

  // *************************************************

  std::wstring sourcew = L"match find ('testwäöƔƓ')";
  output("wstring match should return 'testwäöƔƓ' \n");

  obj = new TFScripal(conv(sourcew));

  obj->match(conv(U"look for testwäöƔƓ in this text"));

  if (obj->hasMatched)
    output("wstring match: '" + obj->results.text[0] + "'\n\n");
  else
    output("no wstring match\n\n");

  delete obj;

  output("find word 'test' and output in HRF, CSV, JSON  \n");
  std::string source = "match find ('test'); end; loop;";
  obj = new TFScripal(source);

  obj->match(conv(u"look for test in this test as a test."));
  if (obj->hasMatched) {
    output("\nHRF: \n");
    output(obj->resultsToHRF(ENC_UTF8));
    output("\nCSV: \n");
    output(obj->resultsToCSV(ENC_UTF8));
    output("\nJSON: \n");
    output(obj->resultsToJSON(ENC_UTF8));
  } else {
    output("no match\n\n");
  }
  delete obj;


}

// ****************************************************************************
// test international digits
// ****************************************************************************
void unitTestDigits() {
  output("non arabic digits test: are international digits interpreted correctly?\n\n");
  output("-൧.൨൩൪ should match (equivalent to -1.234) \n");
  std::string source =  "% {\"translateDigits\" : true } % match isNumber(repeat[1,@](char));";
  TFScripal* obj = new TFScripal(source);

  obj->match("-൧.൨൩൪");

  if (obj->hasMatched)
    output("digits match: '"+  obj->results.text[0] + "'\n\n");
  else
    output("no match\n\n");
 
  delete obj;

  config.translateDigits = false;
}

// ****************************************************************************
// test split function string and file 
// ****************************************************************************
void unitTestSplit() {
  output("split functionality test\n\n");
  output("result should show: \ntest1\ntest2\ntest3\n\n");
  std::string appPath = trimText(config.appPath);
  
  #ifdef LINUX
  if (appPath.substr(0,2) == "~/") 
    appPath.replace(0, 1, homeDir());
  #endif

  std::string source = "match find( blank ); ifMatch end; loop;"; 
  std::string text = "    test1 test2    test3   "; 
  std::string fileIn = appPath + pathSep + "testin.txt"; 
  std::string fileOut = appPath + pathSep + "testout.txt"; 
  
  std::ofstream iFile(fileIn, std::ios_base::binary);
  iFile << text; 
  iFile.close();
    
  output("string operation\n");
  clearError();
  TFScripal* obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
     return;

  obj->split(text);
  for (auto value: obj->results.text) 
    output(value + "\n");

  output("\nfile operation\n");
  int fileEncoding = ENC_UTF8;
  obj->splitFile(fileIn,fileOut, fileEncoding);

  std::string buffer;
  if (readFromFile(fileOut, buffer)) 
    output(buffer + "\n");
} 

// ****************************************************************************
// test replace function  
// ****************************************************************************
void unitTestReplace() {
  output("replace functionality test\n\n");
  output("result should show:\n    replace1 replace2    replace3   \n\n");
  std::string appPath = trimText(config.appPath);
  
  #ifdef LINUX
  if (appPath.substr(0,2) == "~/") 
    appPath.replace(0, 1, homeDir());
  #endif

  std::string source = "match find('test'); ifMatch end; loop;"; 
  std::string text = "    test1 test2    test3   "; 
  std::string fileIn = appPath + pathSep + "testin.txt"; 
  std::string fileOut = appPath + pathSep + "testout.txt"; 
  
  std::ofstream iFile(fileIn, std::ios_base::binary);
  iFile << text;
  iFile.close();
    
  clearError();    
  output("string operation\n");
  TFScripal* obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;
    
  obj->replace(text, "replace");
  if (obj->results.size() > 0) 
    output(obj->results.text[0] + "\n");

  output("\nfile operation\n");
  int curEncoding = ENC_DEFAULT;
  obj->replaceFile(fileIn, fileOut,"replace", curEncoding);

  std::string buffer;
  if (readFromFile(fileOut, buffer)) 
    output(buffer + "\n\n");
} 

// ****************************************************************************
// test nearest match  
// ****************************************************************************
void unitTestNearest() {
  output("nearest match test\n\n");
  output("find best matches dist >= 0.6\n\n");
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
    output("rating : " + std::to_string(obj->results.ratings[i]) + " , '" + obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + "," + std::to_string(obj->results.positions[i][1]) + "]\n");
  }
  delete obj;
  
  config.patternNearest = PATTERN_LEVEN_WORD;
  obj = new TFScripal(source, 0.7, MATCH_NEAREST);
  obj->match(text);

  output("\npattern levensthein word\n");
  output("source :'" + source + "'\n");
  output("text   :'" + text + "'\n\n");
  for (size_t i = 0; i < obj->results.positions.size(); ++i) {
    output("rating : " + std::to_string(obj->results.ratings[i]) + " , '" + obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + "," + std::to_string(obj->results.positions[i][1]) + "]\n");
  }
  delete obj;

  text = "tests where we find occurrences in any tests what might be successful and Tests where tests when and why failtests where";
  source = "tests where";

  config.patternNearest = PATTERN_LEVEN;
  obj = new TFScripal(source, 0.7, MATCH_NEAREST);
  obj->match(text);

  output("\npattern leven (phrase)\n");
  output("source :'" + source + "'\n");
  output("text   :'" + text + "'\n\n");
  for (size_t i = 0; i < obj->results.positions.size(); ++i) {
    output("rating : " + std::to_string(obj->results.ratings[i]) + " , '" + obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + "," + std::to_string(obj->results.positions[i][1]) + "]\n");
  }
  delete obj;

  text = "tests where we find occurrences in any tests what might be successful and Tests where tests when and why fail";
  source = "tests where";

  config.patternNearest = PATTERN_JARO;
  obj = new TFScripal(source, 0.7, MATCH_NEAREST);
  obj->match(text);

  output("\npattern Jaro (phrase)\n");
  output("source :'" + source + "'\n");
  output("text   :'" + text + "'\n\n");
  for (size_t i = 0; i < obj->results.positions.size(); ++i) {
    output("rating : " + std::to_string(obj->results.ratings[i]) + " , '" + obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + "," + std::to_string(obj->results.positions[i][1]) + "]\n");
  }
  delete obj;

}

// ****************************************************************************
// test nearest match algorithms
// ****************************************************************************
void unitTestNearestAlgo() {
  std::string str1, str2;
  output("nearest algorithms test\n\n");
  output("show distances for two strings: levensthein and levensthein plus\n\n");
  str1 = "test"; str2 = "test";    output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "tests";   output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "TEST";    output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "testing"; output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "besting"; output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "best";    output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "best"; str2 = "test";    output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "test"; str2 = "temp";    output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  str1 = "testing"; str2 = "testings"; output("'" + str1 + "' , '" + str2 + "' : " + std::to_string(levenDistance(str1, str2)) + " , " + std::to_string(levenPlusDistance(str1, str2)) + "\n");
  output("\n");
}

// ****************************************************************************
// test block match  
// ****************************************************************************
void unitTestBlock() {
  std::string text;
  std::string source = "Match block1";
  TFScripal* obj;

  output("block match, source is '" + source +"'\n\n");
  obj = new TFScripal(source, 0.0, MATCH_BLOCK);
 
  text = "Match block1";
  obj->match(text);
  output("rating for '" + text + "' is " + std::to_string(obj->results.ratings[0]) + "\n");

  text = "Matched block1";
  obj->match(text);
  output("rating for '" + text + "' is " + std::to_string(obj->results.ratings[0]) + "\n");

  text = "Match block2";
  obj->match(text);
  output("rating for '" + text + "' is " + std::to_string(obj->results.ratings[0]) + "\n");

  text = "Matches blocks1";
  obj->match(text);
  output("rating for '" + text + "' is " + std::to_string(obj->results.ratings[0]) + "\n");
 
}

// ****************************************************************************
// test config 
// ****************************************************************************
void unitTestConfig() {
  std::string json;
  output("config test\n\n");
  output("test config to JSON conversion:\n\n");
  json = config.toJSON({"all"});
  output(json + "\n");

  config.logChannel = pathSep + "temp"; 
  config.measureTime = true; 
  config.fromJSON(json);

  output("\nlogChannel must be 'stdout': " + config.logChannel + "\n");
  output("measureTime must be 0: " + std::to_string(config.measureTime) + "\n\n");

  output("config set in code to show code and debug information(should be displayed)\n\n");
  std::string source = " % = { \"showCode\" : true, \"debugRun\" : true } \n match find( all ('bee'))";

  std::string text = "i can see a honey bee";

  TFScripal* obj = new TFScripal(source);

  obj->match(text);
}

// ****************************************************************************
// test templates
// ****************************************************************************
void unitTestTemplate() {
  output("test template\n\n");
  std::string source, text;
  TFScripal* obj = nullptr;

  output("result should show: \n1007 Mountain Drive, 63527 Gotham City\n1938 Sullivan Lane, 55555 Metropolis\n\n");
  source = "< roadMarker = { any( ~'avenue' ~'ave.' ~'road' ~'street' ~'boulevard'  ~'drive'  ~'lane' ) } >\n"
  "\n"
  "match find( int[1,10000] blank repeat[1,3]( !(<roadMarker>) word ) blank <roadMarker> ) \n"
  "ifMatch { \n"
  "  match ( ',' blank int[1,@] repeat[1,3]( blank word ) at any(',' eol eot ))\n"
  "  ifMatch end \n"
  "} \n"
  "loop ";

  text =   "He lives in 1007 Mountain Drive, 63527 Gotham City, and is a superhero with questionable reputation. His buddy lives in 1938 Sullivan Lane, 55555 Metropolis"; 

  clearError();
  obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;

  obj->match(text);
  if (obj->results.size() >= 2) {
    output(obj->results.text[0]  + "\n");
    output(obj->results.text[1]  + "\n\n");
  };  

  output("result should show: \ntest\n\n");
  source = "< findText = { match find(<1>) } >\n"
  "<findText {'test'} >";

  text =   "This is a test"; 

  clearError();
  delete(obj);

  obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;

  obj->match(text);
  if (obj->results.size() == 1) {
    output(obj->results.text[0]  + "\n\n");
  };  

  output("result should show: \ntest1test2test3\n\n");
  source = "< findText = { match find(<1><2>'test3')} >\n"
  "<findText {'test1'}{'test2'} >";

  text =   "This is test1test2test3"; 

  clearError();
  delete(obj);

  obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;

  obj->match(text);
  if (obj->results.size() == 1) {
    output(obj->results.text[0]  + "\n\n");
  };  

  output("result should show: \nlevel1level2\n\n");
  source = "< findSub = { match find(<1><2>)} >\n"
  "< findText = { <findSub {<1>} {<2>} > } >\n"
  "< findText {'level1'}{'level2'}>";

  text =   "This is level1level2"; 

  clearError();
  delete(obj);

  obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;

  obj->match(text);
  if (obj->results.size() == 1) {
    output(obj->results.text[0]  + "\n\n");
  };  

  output("result should show: \nlevel1level2level3level4\n\n");
  source = "< findSub2 = { match find(<1><2><3> 'level4')} >\n"
  "< findSub1 = { <findSub2 {<1>} {<2>} {<3>} > } >\n"
  "< findText = { <findSub1 {<1>} {<2>} {<3>}> } >\n"
  "< findText {'level1'}{'level2'}{'level3'}>";

  text =   "This is level1level2level3level4level5"; 

  clearError();
  delete(obj);

  obj = new TFScripal(source);
  if (errorMsg != ERR_NONE)
    return;

  obj->match(text);
  if (obj->results.size() == 1) {
    output(obj->results.text[0]  + "\n\n");
  };  

} 


// ****************************************************************************
// test auxiliary interface
// simulate UTF16L interface, output to any console
// ****************************************************************************
#pragma warning(disable:4996)
void unitTestInterface() {
  libInitC("", 0, SCRIPAL_ENC_UTF8);
  const char* logChannel = "buffer";
  logInit(logChannel, strlen(logChannel));

  const char* result = NULL;
  const char* result2 = NULL;
  const char* source = NULL;
  const char* text = NULL;
  int resultLen, resultLen2;
  bool resultBool;
  int intRes, objID, posStart, posEnd;

  //provoke error
  deletePtr((const char*)0x1);

  getErrMsg(&result, &resultLen);
  printf("getErr: %s\n", result);

  getErrExp(&result, &resultLen);
  printf("getErrExp: %s\n", result);

  intRes = getErrPos();
  printf("getErrPos: %d\n\n", intRes);

  // basic methods
  clearErr();
  setEncoding(SCRIPAL_ENC_UTF8);
  getVersion(&result, &resultLen);
  printf("Version: %s\n\n", result);

  // matches
  source = "match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')";
  text = "Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.";
  objID = newCodeObj(source, strlen(source));

  if (match(objID, text, strlen(text))) {
    printf("source match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      printf("'%s', [%d, %d]", result, posStart, posEnd);
    }
  }
  printf("\n\n");
  deleteObj(objID);

  clearLogBuf();
  const char* json = "{ \"debugRun\": \"true\" }";
  configFromJSON(json, strlen(json));
  source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  text = "Find the words apple, ape and award.";
  objID = newCodeObj(source, strlen(source));
  
  if (match(objID, text, strlen(text))) {
    getLogBuf(&result, &resultLen);
    printf("log buffer:\n%s", result);
  
    printf("source match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      printf("'%s', [%d, %d]\n", result, posStart, posEnd);
    }
  }
  printf("\n");
  getResultCSV(objID, &result, &resultLen);
  printf("results as CSV:\n%s\n", result);
  getResultJSON(objID, &result, &resultLen);
  printf("results as JSON:\n%s\n\n", result);
  getResultHRF(objID, &result, &resultLen);
  printf("results as HRF:\n%s\n", result);
  deleteObj(objID);

  source = "test";
  text = "Find the words like test, tests and TEST.";
  objID = newNearestObj(source, strlen(source), 0.5);
  if (match(objID, text, strlen(text))) {
    printf("nearest match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  if (matchFile(objID, "test.txt", strlen("test.txt"), SCRIPAL_ENC_UTF8)) {
    printf("match file result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  deleteObj(objID);

  source = "testing";
  objID = newBlockObj(source, strlen(source), 0.5);
  text = "resting";
  double rating;
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'resting'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  text = "testings";
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'testings'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  text = "nesting";
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'nesting'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  printf("\n");
  deleteObj(objID);

  source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  text = "Find the words apple, ape plus award.";
  objID = newCodeObj(source, strlen(source));
  resultBool = replace(objID, text, strlen(text), "___", strlen("___"));
  getResultStr(objID, 0, &result, &resultLen);
  printf("source replace result: %s '%s'\n\n" , resultBool ? "true":"false", result);
 
  resultBool = replaceFile(objID, "test.txt", strlen("test.txt"), "testout.txt", strlen("testout.txt"),  "___", strlen("___"), SCRIPAL_ENC_UTF8);

  deleteObj(objID);

  source = "match find(any (' ', ',', '.')); ifMatch end; loop;";
  text = "Find the words apple, ape plus award.";
  objID = newCodeObj(source, strlen(source));
  if (split(objID, text, strlen(text))) {
    printf("split result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  if (splitFile(objID, "test.txt", strlen("test.txt"), "testout.txt", strlen("testout.txt"), SCRIPAL_ENC_UTF8)) {
    printf("split file result: \n" );
    FILE *fptr = fopen("testout.txt", "r");  
  
    // Check if the file was opened successfully
    if (fptr != NULL) {  
        // Read and print each character from the file
        char ch;
        while ((ch = fgetc(fptr)) != EOF) {
            putchar(ch);
        }
    
        // Close the file after reading
        fclose(fptr);  
    }
  }

  printf("\n");
  deleteObj(objID);

  source = "test";
  objID = newNearestObj(source, strlen(source), 0.5);
  if (searchFiles(objID, "./", strlen("./"), "*", strlen("*"), false)) {
    printf("file search result: 1, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultFileName(objID, i, &result2, &resultLen2);
      getResultPos(objID, i, &posStart, &posEnd);
      double rating; 
      getResultRating(objID, i, &rating);
      printf("'%s', '%s', [%d, %d], %f\n", result2, result, posStart, posEnd, rating);
    }
  }
  printf("\n");
  deleteObj(objID);

  // config
  configToJSON(&result, &resultLen);
  printf("config as JSON:\n%s\n\n", result);
  
  const char* names = "negSign,encoding";
  configToJSONVal(&result, &resultLen, names, strlen(names));
  printf("config settings 'negSign' and 'encoding' as JSON:\n%s\n\n", result);

  json = "{ \"negSign\": \"+\" }";
  configFromJSON(json, strlen(json));
  names = "negSign";
  configToJSONVal(&result, &resultLen, names, strlen(names));
  printf("config settings 'negSign' after changing to '+':\n%s\n\n", result);
 
  names = "conf.c";
  resultBool = configStore(names, strlen(names));
  printf("store config in conf.c, result:%s\n\n", resultBool ? "true":"false");

  resultBool = configRestore(names, strlen(names));
  printf("restore config from conf.c, result: %s\n\n", resultBool ? "true":"false");

  // templates
  templateReset();

  names = "findtest";
  source = "match find('test')";
  templateSet(names, strlen(names), source, strlen(source));

  names = "templates.c";
  resultBool = templateStore(names, strlen(names));
  printf("store templates in templates.c, result: %s\n\n", resultBool ? "true":"false");

  names = "findtest";
  templateGet(names, strlen(names), &result, &resultLen);
  printf("template findtest: %s\n\n", result);

  templateReset();
  names = "templates.c";
  resultBool = templateRestore(names, strlen(names));
  printf("restore templates from templates.c, result: %s\n\n", resultBool ? "true":"false");
 
  resultBool = templatesToJSON(&result, &resultLen);
  printf("templates to JSON: \n%s\n\n", result);

  names = "findtest";
  resultBool = templatesToJSONVal(&result, &resultLen, names, strlen(names));
  printf("template findtest to JSONVal: %s\n\n", result);

  deletePtr(result2);
  deletePtr(result);

  libFinalize();
}

// ****************************************************************************
// test file decocde operation
// ****************************************************************************
void unitTestDecodeFile() {
  std::string filePath;
  std::string appPath = trimText(config.appPath) + pathSep + "test";
  
  #ifdef LINUX
  if (appPath.substr(0,2) == "~/") 
    appPath.replace(0, 1, homeDir());
  #endif

  output("test file decoding, should show 'this is case x, this is table x'\n\n");
  
  filePath = decodeFile(appPath + pathSep + "test1.docx");
  if (filePath == "NULL") return;
  output(appPath + pathSep + "test1.docx: \n\n"); 
  std::string buffer;
  if (readFromFile(filePath, buffer)) 
    output(buffer + "\n");

  filePath = decodeFile(appPath + pathSep + "test2.docx");
  if (filePath == "NULL") return;
  output(appPath + pathSep + "test2.docx: \n\n"); 
  if (readFromFile(filePath, buffer)) 
    output(buffer + "\n");

  filePath = decodeFile(appPath + pathSep + "test1.pdf");
  if (filePath == "NULL") return;
  output(appPath + pathSep + "test1.pdf: \n\n");   
  if (readFromFile(filePath, buffer)) 
    output(buffer + "\n");

  filePath = decodeFile(appPath + pathSep + "test2.pdf");
  if (filePath == "NULL") return;
  output(appPath + pathSep + "test2.pdf: \n\n");   
  if (readFromFile(filePath, buffer)) 
    output(buffer + "\n");
}
  
// ****************************************************************************
// test file search operation
// ****************************************************************************
void unitTestFileSearch() {
  std::string source = "match find(' signature' any(' ' '.' )); end; loop;";
  TFScripal* obj;
  std::string appPath = trimText(config.appPath) + pathSep + "test";
  
  #ifdef LINUX
  if (appPath.substr(0,2) == "~/") {
    appPath.replace(0, 1, homeDir());
  }
  #endif

  bool dirRecursive = true;
  std::string dirExtension;

  // source search
  dirExtension = "txt";
  output("source search for word 'signature' in .txt: \n\n");
  obj = new TFScripal(source);
  obj->fileSearch(appPath, dirExtension, dirRecursive);
  output(obj->resultsToJSON(encoding) + "\n");
  delete obj; obj = nullptr;

  // source search
  dirExtension = "*";
  output("\nsource search for 'signature' in .*: \n\n");
  obj = new TFScripal(source);
  obj->fileSearch(appPath, dirExtension, dirRecursive);
  output(obj->resultsToJSON(encoding) + "\n");
  delete obj; obj = nullptr;

  // nearest search
  output("\nnearest search: look for words similar to 'test'\n\n");
  obj = new TFScripal("test", 0.9, MATCH_NEAREST);
  obj->fileSearch(appPath, dirExtension, dirRecursive);
  output(obj->resultsToJSON(encoding) + "\n");
  delete obj; obj = nullptr;

  // block search
  output("\nblock match: block*.txt should match\n\n");
  obj = new TFScripal("test", 0.8, MATCH_BLOCK);
  obj->fileSearch(appPath, dirExtension, dirRecursive);
  output(obj->resultsToJSON(encoding) + "\n");
  delete obj; obj = nullptr;
}
// ****************************************************************************
// test file replace operation
// ****************************************************************************
void unitTestFileReplace() {
  std::string appPath = trimText(config.appPath) + pathSep + "test";

  #ifdef LINUX
  if (appPath.substr(0,2) == "~/") {
    appPath.replace(0, 1, homeDir());
  }
  #endif
  std::string fileIn = appPath + pathSep + "replace.txt";
  std::string fileOut = appPath + pathSep + "replace.txt_";

  std::filesystem::copy(fileIn, fileOut, std::filesystem::copy_options::update_existing);

  std::string source = "match find('testSrc'); end; loop;";
  TFScripal* obj = new TFScripal(source);
  int curEncoding = ENC_DEFAULT;
  obj->replaceFile(fileIn, fileOut, "testDst", curEncoding);
  if (obj->hasMatched) {
    output("new file: \n");
    std::string fileContent;
    readFromFile(fileOut, fileContent);
    output(fileContent + "\n\n");
  } else {
    output("no match\n\n");
  }
  delete obj;
};

} // end of namespace
