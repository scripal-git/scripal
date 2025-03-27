// ***************************************************************************
// Scripal quick single test
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef SINGLETEST_HPP_INCLUDED
#define SINGLETEST_HPP_INCLUDED

namespace Scripal {

static std::vector<TFExmpl> singleExample = {

{ "task: match JSON element where  sub element cat is '127.0.4.1', *.bites is 0, status: 0 and url is an IPv4\n"
  "how:  all in single line: match chain with single find\n"
  "      break on '{' once in an element to avoid skipping over JSON element\n"
  "      advance chars as long as not condition looked for with repeat[@] (!condition char)",
  
  "match find ('element' all(digit) ': {')\n"
  "ifMatch match find( break('{') '\"cat\": \"127.0.4.1\"' repeat[@](!'\"bites' char) '\"bites\": ' 0 repeat[@](!'\"status\"' char) '\"status\": ' 0 repeat[1,@](!'\"url\"' char) '\"url\": ' '\"' pure[0,255] '.' repeat[@](!'}' char) '}') \n"
  "ifMatch end; else reset\n"
  "loop",
  
  "{\n"
  "element1: { \n\"cat\": \"127.0.5.2\",\n" 
     "\"bites\": 2,\n"
     "\"status\": 1,\n"
     "\"url\": \"http://test.de/\" \n},\n"  
  "element2: { \n\"cat\": \"127.0.4.1\",\n" 
     "\"bites\": 0,\n"
     "\"status\": 0,\n"
     "\"url\": \"192.168.5.6\" \n},\n"  
  "element3: { \n\"cat\": \"127.0.4.2\",\n" 
     "\"bites\": 0,\n"
     "status: 0,\n"
     "\"url\": \"192.168.5.6\" \n}\n"  
  "}",
  
  {  "element2: { \n\"cat\": \"127.0.4.1\",\n" 
     "\"bites\": 0,\n"
     "\"status\": 0,\n"
     "\"url\": \"192.168.5.6\" \n}" }
},
/*
{ "task: test long string (100 MB), find text at the end and measure time in ms\n"
  "how:  find case insensitive 't' and T'",

  "match find( ~'t' 'T' )",

  std::string(100000000,'X') + "TT",

  {"TT"}
},
*/
};

};
//"match find(hex[0, ffffffffff]);" ,

#endif