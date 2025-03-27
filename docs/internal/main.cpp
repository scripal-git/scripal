# test program for scripal library

// #define MATCH 
// #define NEAREST
#define THREADS

#ifdef MATCH
#include <stdio.h>
#include "scripal/scripalLib.hpp"

int main(int argc, char **argv) {
  using namespace Scripal;
  initLibrary(""); // put path to your own config file here, "" = use default

  std::cout << "version: " << Scripal::VERSION << "\n\n";

  // set time measurement on
  getConfig()->set("measureTime", true);

  // scripal object
  auto obj = new TFScripal("match '(234) 235-5678'");;
  if (obj->match("(234) 235-5678")) {
    std::cout << "results: " << obj->results.size() << " , '" << obj->results.text[0] << "'" << \
    "  position: " << obj->results.positions[0][0] << ", " << obj->results.positions[0][1] << "\n\n";
  }
  exit(0);

};
#endif

#ifdef NEAREST
#include <stdio.h>
#include "scripal/scripalLib.hpp"
int main() {
  using namespace Scripal;
  initLibrary(""); // put path to your own config file here, "" = use default

  std::string text = "find any tests where a test might be testing and tested stuff is the best of the rest";
  std::string source = "Test";
  TFScripal* obj;

  getConfig()->set("patternNearest" , PATTERN_LEVENPLUS_WORD);
  obj = new TFScripal(source, 0.6, MATCH_NEAREST);
  obj->match(text);

  std::cout << "pattern levensthein plus word\n";
  std::cout << "source :'" + source + "'\n";
  std::cout << "text   :'" + text + "'\n\n";
  for (size_t i = 0; i < obj->results.positions.size(); ++i) {
    std::cout << "rating : " + std::to_string(obj->results.ratings[i]) + " , '" + obj->results.text[i] + "' [" + std::to_string(obj->results.positions[i][0]) + "," + std::to_string(obj->results.positions[i][1]) + "]\n";
  }
}
#endif

#ifdef THREADS
#include <stdio.h>
#include "scripal/scripalLib.hpp"
#include <thread>
#include <iostream>
#include <chrono>

void tprocess(int x)
{
  std::string confVal;
  getConfig()->set("appPath", "\\test\\" + std::to_string(x));
  getConfig()->get("appPath", confVal);
  std::cout << "start with " << std::to_string(x) + ": " + confVal + "\n"; 

  std::this_thread::sleep_for (std::chrono::seconds(1));

  getConfig()->get("appPath", confVal);
  std::cout << "end with "  << std::to_string(x) + ": " + confVal + "\n"; 
}

int main() {
  using namespace Scripal;
  initLibrary(""); // put path to your own config file here, "" = use default

  std::thread thread1(tprocess, 1);     
  std::thread thread2(tprocess, 2); 
  std::thread thread3(tprocess, 3); 
  std::thread thread4(tprocess, 4); 
  std::thread thread5(tprocess, 5); 

  thread1.join();
  thread2.join();
  thread3.join();
  thread4.join();
  thread5.join();
}
#endif