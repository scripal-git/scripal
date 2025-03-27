// ***************************************************************************
// Scripal main functionality tests
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef MAINTEST_HPP_INCLUDED
#define MAINTEST_HPP_INCLUDED

namespace Scripal {
  
static std::vector<TFExmpl> testExamples = {
{ "task: test *all* operand with mutiple sub operands",
  "match all ('test' space 'all')",
  "test all",
  {"test all"}
},

{ "task: test *all* operand with single sub operand",
  "match all ('test')",
  "test",
  {"test"}
},

{ "task: test *all* operand with mutiple sub operands with fail",
  "match all ('tet' space 'all')",
  "test all",
  {}
},

{ "task: test *any* operand with mutiple sub operands",
  "match any ('test' space 'all')",
  "test",
  {"test"}
},

{ "task: test *any* operand with single sub operand",
  "match any ('test')",
  "test",
  {"test"}
},

{ "task: test *any* operand with mutiple sub operands with fail",
  "match any ('test' space 'all')",
  "word",
  {}
},

{ "task: test *each* operand with mutiple sub operands",
  "match each ( ('test' blank) ('all' blank) ('each') )",
  "test all each cases",
  {"test all each"}
},

{ "task: test *each* operand with single sub operand",
  "match each ('test')",
  "test case",
  {"test"}
},

{ "task: test *each* operand with mutiple sub operands with fail",
  "match each ( 'test' )",
  "find all each cases",
  {}
},

{ "task: test *every* operand with mutiple sub operands",
  "match every ( 'each' 'test' blank 'all' )",
  "test all each cases",
  {"test all each "}
},

{ "task: test *every* operand with single sub operand",
  "match every ('test')",
  "test case",
  {"test"}
},

{ "task: test *every* operand with mutiple sub operands with fail",
  "match every ( 'each' 'test' blank 'all' )",
  "no more cases",
  {}
},

{ "task: test *repeat* operand with mutiple sub operands",
  "match find (space repeat[3,3](letter digit) 'test' )",
  "find a1n1y1test with three letters previous",
  {" a1n1y1test"}
},

{ "task: test *repeat* operand with single sub operand",
  "match find (space repeat[3,3](letter) 'test' )",
  "find anytest with three letters previous",
  {" anytest"}
},

{ "task: test *repeat* operand with mutiple sub operands with fail",
  "match find (space repeat[3,3](letter digit) 'test' )",
  "find a1n1y1t with three letters previous",
  {}
},

{ "task: test *find* operand with mutiple sub operands",
  "match find ( 'test ' 'all')",
  "look for test all",
  {"test all"}
},

{ "task: test *find* operand with single sub operand",
  "match find ('test')",
  "look for test all",
  {"test"}
},

{ "task: test *find* operand with single sub operand with fail",
  "match find ('test')",
  "look for word",
  {}
},

{ "task: test *findat* operand with mutiple sub operands",
  "match findat[8,30] ( 'test ' 'all')",
  "test all look for test all , but there's only on test all in match",
  {"test all"}
},

{ "task: test *findat* operand with single sub operand",
  "match findat[10,25] ( 'test' )",
  "test and more test , but there's only one test in match",
  {"test"}
},

{ "task: test *findat* operand with single sub operand with fail",
  "match findat[10,25] ( 'try' )",
  "test and more test , but there's only one test in match",
  {}
},

{ "task: test *break* operand with mutiple sub operands",
  "match find ( break('before ' 'all ') 'test' )",
  "if before all is ahead of test, break",
  {}
},

{ "task: test *break* operand with single sub operand",
  "match find ( break('before') 'test' )",
  "if before is ahead of test, break",
  {}
},

{ "task: test *isNumber* operand with no range",
  "match isNumber(repeat[1,@](char))",
  "-1.234",
  {"-1.234"}
},

{ "task: test *isNumber* operand with no range with fail",
  "match isNumber(repeat[1,@](char))",
  "-A1.234",
  {}
},

{ "task: test *isNumber* operand with range",
  "match isNumber[1,200](repeat[1,@](char))",
  "199.234",
  {"199.234"}
},

{ "task: test *isNumber* operand with range with fail",
  "match isNumber[1,200](repeat[1,@](char))",
  "299.234",
  {}
},

{ "task: test *isWord* operand",
  "match isWord( repeat[1,@](letter) )",
  "Alexander  ",
  {"Alexander"}
},

{ "task: test *isWord* operand with fail",
  "match isWord( repeat[1,@](letter) )",
  "Alexander1",
  {}
},

{ "task: test *isUpper* operand",
  "match isUpper( repeat[1,@](char) )",
  "TEST",
  {"TEST"}
},

{ "task: test *isUpper* operand with fail",
  "match isUpper( repeat[1,@](char) )",
  "TeST",
  {}
},

{ "task: test *isLower* operand",
  "match isLower( repeat[1,@](char) )",
  "test",
  {"test"}
},

{ "task: test *isLower* operand with fail",
  "match isLower( repeat[1,@](char) )",
  "tEst",
  {}
},

{ "task: test *space* operand",
  "match space",
  " ",
  {" "}
},

{ "task: test *space* operand with fail",
  "match space",
  "t",
  {}
},

{ "task: test *blank* operand",
  "match (blank)",
  "     a",
  {"     "}
},

{ "task: test *blank* operand with fail",
  "match (blank)",
  "abc",
  {}
},

{ "task: test *char* operand with no range",
  "match (char)",
  "a",
  {"a"}
},

{ "task: test *char* operand with character value range",
  "match (char[97,97])",
  "a",
  {"a"}
},

{ "task: test *char* operand with character value range with fail",
  "match (char[10,60])",
  "a",
  {}
},

{ "task: test *letter* operand",
  "match letter",
  "a",
  {"a"}
},

{ "task: test *letter* operand with fail",
  "match letter",
  "2",
  {}
},

{ "task: test *digit* operand",
  "match digit",
  "0",
  {"0"}
},

{ "task: test *digit* operand with fail",
  "match digit",
  "a",
  {}
},

{ "task: test *hex digit* operand",
  "match hex digit",
  "a",
  {"a"}
},

{ "task: test *hex digit* operand with fail",
  "match hex digit",
  "k",
  {}
},

{ "task: test *oct digit* operand",
  "match bin digit",
  "0",
  {"0"}
},

{ "task: test *oct digit* operand with fail",
  "match bin digit",
  "9",
  {}
},

{ "task: test *bin digit* operand",
  "match bin digit",
  "0",
  {"0"}
},

{ "task: test *bin digit* operand with fail",
  "match bin digit",
  "2",
  {}
},

{ "task: test *word* operand",
  "match word",
  "word and more",
  {"word"}
},

{ "task: test *word* operand",
  "match word",
  "word. and more",
  {"word"}
},

{ "task: test *word* operand with fail",
  "match word",
  "  word and more",
  {}
},

{ "task: test *word* operand with fail",
  "match word",
  "1word and more",
  {}
},

{ "task: test *word* operand with fail",
  "match word",
  "word% and more",
  {}
},

{ "task: test *bos* operand",
  "moveon 1; match bos",
  " start of sentence.",
  {""}
},

{ "task: test *bos* operand",
  "moveon 20; match bos",
  " start of sentence. Next sentence.",
  {""}
},

{ "task: test *bos* operand with fail",
  "moveon 21; match bos",
  " start of sentence. Next sentence.",
  {}
},

{ "task: test *eos* operand",
  "moveon 24; match eos",
  "Is this the sentence end. ",
  {"."}
},

{ "task: test *eos* operand",
  "moveon 24; match eos",
  "Is this the sentence end? ",
  {"?"}
},

{ "task: test *eos* operand with fail",
  "moveon 25; match eos",
  "Is this the sentence end? ",
  {}
},

{ "task: test *bol* operand",
  "match bol",
  " begin of line",
  {""}
},

{ "task: test *bol* operand with fail",
  "moveon 1; match bol",
  " begin of line",
  {}
},

{ "task: test *eol* operand",
  "moveon 25; match eol",
  "Is this the sentence end.\n Start a new line. ",
  {"\n"}
},

{ "task: test *eol* operand",
  "moveon 25; match eol",
  "Is this the sentence end.\r\n Start a new sentence. ",
  {"\r\n"}
},

{ "task: test *eol* operand with fail",
  "moveon 27; match eol",
  "Is this the sentence end.\r\n Start a new sentence. ",
  {}
},

{ "task: test *bot* operand",
  "match bot",
  " begin of line",
  {""}
},

{ "task: test *bot* operand with fail",
  "moveon 1; match bot",
  " begin of line",
  {}
},

{ "task: test *bot* operand with fail",
  "moveon 1; match bot",
  " begin of line",
  {}
},

{ "task: test *eot* operand",
  "moveon 19; match eot",
  "End of text. ",
  {""}
},

{ "task: test *eot* operand with fail",
  "moveon 12; match eot",
  "End of text. ",
  {}
},

{ "task: test *bow* operand",
  "moveon 4; match bow",
  "End of text. ",
  {""}
},

{ "task: test *bow* operand with fail",
  "moveon 3; match bow",
  "End of text. ",
  {}
},

{ "task: test *eow* operand",
  "moveon 3; match eow",
  "End of text. ",
  {""}
},

{ "task: test *eow* operand with fail",
  "moveon 2; match eow",
  "End of text. ",
  {}
},

{ "task: test *bomark* operand",
  "match bomark",
  "\xef\xbb\xbf",
  {"\xef\xbb\xbf"}
},

{ "task: test *move* operand",
  "match (move 5 'test')",
  "     test",
  {"test"}
},

{ "task: test *move* operand with fail",
  "match (move 3 'test')",
  "     test",
  {}
},

{ "task: test *~* attribute",
  "match ~'test'",
  "Test",
  {"Test"}
},

{ "task: test *!* attribute",
  "match !'test'",
  "apple",
  {""}
},

{ "task: test number",
  "match (123 find('test'))",
  "123test",
  {"123test"}
},

{ "task: test attribute for hex value",
  "match hex isNumber(repeat[1,@](char))",
  "a0056",
  {"a0056"}
},

{ "task: test attribute for hex value with fail",
  "match hex isNumber(repeat[1,@](char))",
  "g0056",
  {}
},

{ "task: test attribute for oct value",
  "match oct isNumber(repeat[1,@](char))",
  "50056",
  {"50056"}
},

{ "task: test attribute for oct value with fail",
  "match oct isNumber(repeat[1,@](char))",
  "90056",
  {}
},

{ "task: test attribute for binary value",
  "match bin isNumber(repeat[1,@](char))",
  "-0111011",
  {"-0111011"}
},

{ "task: test attribute for binary value with fail",
  "match bin isNumber(repeat[1,@](char))",
  "-A0111011",
  {}
},

{ "task: test *at* attribute",
  "match (find (at 'test') char)",
  "   test2",
  {"t"}
},

{ "task: test *skip* attribute",
  "match (find (skip 'test') char)",
  "   test2",
  {"2"}
},

{ "task: test *skip* attribute for number with fail",
  "match (skip 123 find('test'))",
  "124test",
  {}
},

{ "task: test *test* attribute",
  "match test find ('test'); ifMatch match (char);",
  "a      test",
  {"a"}
},

{ "task: test *test* attribute with fail",
  "match test find ('test'); ifMatch match (char);",
  "a      tes",
  {}
},

{ "task: test *try* attribute and match",
  "match try 'test'",
  "test",
  {"test"}
},

{ "task: test *try* attribute with fail",
  "match try 'test'",
  "no test",
  {""}
},

{ "task: test *last* attribute",
  "match repeat[2,@]( last(~'S') letter )",
  "tests",
  {"tests"}
},

{ "task: test *last* attribute with fail",
  "match repeat[2,@]( last(~'S') letter )",
  "test",
  {}
},

{ "task: test *notlast* attribute",
  "match repeat[2,@]( notlast ~'S' letter )",
  "test",
  {"test"}
},

{ "task: test *notlast* attribute with fail",
  "match repeat[2,@]( notlast ~'S' letter )",
  "tests",
  {}
},

{ "task: test *únless* attribute",
  "match find( all (unless 'honey ' 'bee'))",
  "i can see a bee",
  {"bee"}
},

{ "task: test *únless* attribute with fail",
  "match find( all (unless 'honey ' 'bee'))",
  "i can see a honey bee",
  {}
},

{ "task: test config",
  "% = { \"showCode\" : true, \"debugRun\" : true } \n\
  match find( all ('bee'))",
  "i can see a honey bee",
  {"bee"}
},

{ "task: test template",
  "< roadMarker = { 'test' } >  \n\
  match <roadMarker>",
  "test",
  {"test"}
},


};

}; // end of namespace

#endif