// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

// Scripal in C++
#include <stdio.h>
#include <iostream>
#include <fstream>

// compiler/OS dependencies 
#ifdef __linux__
#include "scripal/scripalLib.hpp"
#endif
#ifdef _WIN32
#include "../../inc/scripalLib.hpp"
#endif

int main(int argc, char **argv) {
  using namespace Scripal;
  initLibrary("", ENC_UTF8); // put path to your own config file here, "" = use default
  initLog("buffer");
  setError("system error, address(0x1) not found in buffer", nPos, "");

  std::cout << "getErr: " << errorMsg << "\n";
  std::cout << "getErrExp: " << errorExp;
  std::cout << "getErrPos: " << posToInt(errorPos) << "\n";
  std::cout << "\n";

  std::cout << "version: " << Scripal::VERSION << "\n\n";

  // matches
  auto obj = new TFScripal("match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')");
  bool result = obj->match("Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.");
  std::cout << "source match result:" << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.text[i] << "', [" << \
    obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "]\n";
  }
  std::cout << "\n";
  delete obj;

  // matches
  config.fromJSON("{ \"debugRun\": \"true\" }");
  logBuffer.clear();
  obj = new TFScripal("match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
  result = obj->match("Find the words apple, ape and award.");
  std::cout << "log buffer:\n" << logBuffer;
  std::cout << "source match result:" << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.text[i] << "', [" << obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "]\n";
  }
  std::cout << "\n";
  std::cout << "results as CSV:\n";
  std::cout << obj->resultsToCSV() << "\n";
  std::cout << "results as JSON:\n";
  std::cout << obj->resultsToJSON() << "\n\n";
  std::cout << "results as HRF:\n";
  std::cout << obj->resultsToHRF() << "\n";
  delete obj;

  obj = new TFScripal("test",  0.5, MATCH_NEAREST);
  result = obj->match("Find the words like test, tests and TEST.");
  std::cout << "nearest match result: " << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.text[i] << "', [" << obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "]\n";
  }
  std::cout << "\n";
  
  result = obj->matchFile("test.txt");
  std::cout << "match file results: '" << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.text[i] << "', [" << obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "]\n";
  }
  std::cout << "\n";
  delete obj;

  obj = new TFScripal("testing", 0.5, MATCH_BLOCK);
  result = obj->match("resting");
  std::cout << "block match result: " << std::to_string(result) <<  ", 'resting'\n"  << std::to_string(obj->results.ratings[0]) << "\n";
  result = obj->match("testings");
  std::cout << "block match result: " << std::to_string(result) <<  ", 'testings'\n"  << std::to_string(obj->results.ratings[0]) << "\n";
  result = obj->match("nesting");
  std::cout << "block match result: " << std::to_string(result) <<  ", 'nesting'\n"  << std::to_string(obj->results.ratings[0]) << "\n";
  std::cout << "\n";
  delete obj;

  obj = new TFScripal("match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
  result = obj->replace("Find the words apple, ape plus award.", "___");
  std::cout << "source replace result: " << std::to_string(result) << ", " << obj->results.text[0] << "\n\n";
  delete obj;

  obj = new TFScripal("test",  0.5, MATCH_NEAREST);
  obj->replaceFile("test.txt", "testout.txt", "___");
  delete obj;

  obj = new TFScripal("match find(any (' ', ',', '.')); ifMatch end; loop;");
  result = obj->split("Find the words apple, ape plus award.");
  std::cout << "split result: " << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << "'" << obj->results.text[i] << "'\n";
  }
  std::cout << "\n";
  delete obj;

  obj = new TFScripal( "match find(any (' ', ',','.')); ifMatch end; loop;");
  result = obj->splitFile("test.txt", "testout.txt");
  std::cout << "split file result: \n";
  std::cout << std::ifstream("testout.txt").rdbuf() << "\n"; 
  delete(obj);

  obj = new TFScripal("test",  0.5, MATCH_NEAREST);
  obj->fileSearch("./", "*", false);
  std::cout << "file search result: " << std::to_string(result) <<  " , no. of results: "  << std::to_string(obj->results.size()) << "\n";
  for (int i = 0; i < obj->results.size(); ++i) {
    std::cout << obj->results.fileNames[i] << "', '" << obj->results.text[i] << "'" << \
    "  [" << obj->results.positions[i][0] << ", " << obj->results.positions[i][1] << "] , " << obj->results.ratings[i] << "\n";
  }
  std::cout << "\n"; 
  delete obj;

  // config
  std::cout << "config as JSON:\n";
  std::cout << config.toJSON({"all"}) << "\n\n";

  std::cout << "config settings 'negSign' and 'encoding' as JSON:\n";
  std::cout << config.toJSON({"negSign", "encoding"}) << "\n\n";

  std::cout << "config settings 'negSign' after changing to '+':\n";
  config.fromJSON("{ \"negSign\": \"+\" }");
  std::cout << config.toJSON({"negSign"}) << "\n\n";

  std::cout << "store config in conf.cpp, result: " << std::to_string(config.store("conf.cpp")) << "\n\n";

  std::cout << "restore config from conf.cpp, result: " << std::to_string(config.restore("conf.cpp")) << "\n\n";

 // templates
  templates.reset();
  
  templates.set("findtest", "match find('test')");
  
  std::cout << "store templates in templates.cpp, result: " << std::to_string(templates.store("templates.cpp")) << "\n\n";
  
  std::cout << "template findtest: " << templates.get("findtest") << "\n\n";
  
  templates.clear("findtest");
  
  std::cout << "restore templates from templates.cpp, result: " << std::to_string(templates.restore("templates.cpp")) << "\n\n";
  
  std::cout << "all templates to JSON: \n" << templates.toJSON({"all"}) << "\n\n";
  
  std::cout << "template findtest to JSON: " << templates.toJSON({"findtest"}) << "\n\n";

  finalizeLibrary();
 
};

