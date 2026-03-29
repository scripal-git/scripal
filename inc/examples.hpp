// ***************************************************************************
// Scripal examples
// ***************************************************************************
// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

#ifndef EXAMPLES_HPP_INCLUDED
#define EXAMPLES_HPP_INCLUDED

// all interface classes and data available in namespace "Scripal"  
namespace Scripal {
  
static std::vector<TFExmpl> userExamples = {
  { "task: match characters from several UNICODE pages, compare characters from position 0 on\n"
    "      to find text fragments in source text use match( find...  "
    "how:  simple text match with letters from several languages",
  
  u8"match ( 'aËಭݣԓԖ汉ᎧᎾ' )",

  u8"aËಭݣԓԖ汉ᎧᎾ",

  {"aËಭݣԓԖ汉ᎧᎾ"}
},

// ***********************************************************************************\n

{ "task: find first character after end of sentence sign\n"
  "how:  find end of sentence and space (skip the result, don't store), if found -> match next letter",
  
  "match skip find( eos space ) \n"
  "ifMatch matchEnd( letter )",
  
  "End of sentence. Now start the next sentence.",

  {"N"}
},

// ***********************************************************************************\n

{ "task: find single words that are followed by one or many spaces and 'more'\n"
  "how:  find word, only test (don't store result) if followed by spaces and 'more'\n"
  "      if match->store result, reset in case a match cannot be completed and loop",
  
  "match find( word ) \n"
  "match test( blank 'more' )  \n"
  "ifMatch end\n"
  "reset\n"
  "loop",
  
  "Find the word that is followed by more and see if there is more in the text.",

  {"by", "is"}
},

// ***********************************************************************************\n

{ "task: find all words that begin with an 'a'\n"
  "how:  find begin of word and 'a', if match -> find the end of the word and store result\n"
  "      loop for more words",
  
  "match find(bow 'a');\n"
  "ifMatch matchEnd find(eow); loop;",
  
  "Find the words apple, ape and award.",

  {"apple", "ape", "and", "award"}
},

// ***********************************************************************************\n

{ "task: match hex number in the range 0001-ff00\n"
  "how:  specify hex attribute and number range",
  
  "match ( pure hex [1,ff00] )",
  
  "fe78",

  {"fe78"}
},

// ***********************************************************************************\n

{ "task: match serial number in the range 678[900-999]-[0,9]\n"
  "how:  match '678', see if followed by 3 digits which form number in the range 900-999\n"
  "      followed by '-' and a digit, if true -> end result, else reset entire result",
  
  "match ( '678' )\n"
  "ifMatch { match ( isNumber[900,999]( digit digit digit ) '-' digit );}",
  
  "678901-8",

  {"678901-8"}
},

// ***********************************************************************************\n

{ "task: match time range from 2020.07.14 8:10 to 8:14\n"
  "how:  match pure number 2020 followed by . 7 . 14 spaces 8 : and numbers in the range 10-14\n"
  "      use pure numbers instead of int otherwise 2020.xx would be seen as a float number",
  
  "match ( pure 2020 '.' pure 7 '.'  pure 14 blank pure 8 ':' int [10,14] )",
  
  "2020.7.14 8:10",

  {"2020.7.14 8:10"}
},

// ***********************************************************************************\n

{ "task: match all dates from 1900 on with a certain accuracy\n"
  "how:  find pure number 1900..infinity followed by '.' [1..12] '.' [1.31]\n"
  "      use pure numbers instead of int otherwise 1900.1 would be seen as a float number!",
  
  "matchEnd find( pure [1900,@] '.' pure [1,12] '.' pure [1,31] )\n"
  "loop",
  
  "Find dates like 2020.12.30 or 2021.01.14 but not numbers like 12344.23 or 12344.23.12",

  {"2020.12.30","2021.01.14"}
},

// ***********************************************************************************\n

{ "task: find dates with higher accuracy between 1900-01-01 and 1999-12-31 in this format\n"
  "      a leap year is not taken into account\n"    
  "how:  find at (without storing) pure number 1900..1999 and digits in the form dd-dd\n"
  "      if matched reset result, store year and test for month's ranges",

  "match at find( int[1900,1999] '-' digit digit '-' digit digit ) \n"
  "ifMatch { \n"
  "  reset \n"
  "  match ( repeat[4]( digit ) '-') \n"
  "  match ( 1  '-' [1,31] ) \n"
  "  ifNoMatch match ( 2  '-' [1,29] ) \n"
  "  ifNoMatch match ( 3  '-' [1,31] ) \n"
  "  ifNoMatch match ( 4  '-' [1,30] ) \n"
  "  ifNoMatch match ( 5  '-' [1,31] ) \n"
  "  ifNoMatch match ( 6  '-' [1,30] ) \n"
  "  ifNoMatch match ( 7  '-' [1,31] ) \n"
  "  ifNoMatch match ( 8  '-' [1,31] ) \n"
  "  ifNoMatch match ( 9  '-' [1,30] ) \n"
  "  ifNoMatch match ( 10 '-' [1,31] ) \n"
  "  ifNoMatch match ( 11 '-' [1,30] ) \n"
  "  ifNoMatch match ( 12 '-' [1,31] ) \n"
  "  ifMatch end \n"
  "} \n"
  "reset\n"
  "loop" ,

  "Find 1924-01-31, 1900-07-14, 1900-01-01 and 1999-02-28 but not 1999-2-28, 1999-2-31, 1180-07-14, 2000-01-31 OR 1999.2.28",

  {"1924-01-31", "1900-07-14", "1900-01-01", "1999-02-28"}
},

// ***********************************************************************************\n

{ "task: todo: find entire sentence holding a date + time\n"
  "how:  find begin of sentence, find pure numbers according to date format used in your country\n"
  "      if found -> find end of sentence\n",

  "match find (bos letter) \n"
  "ifMatch match find( break( eos ) pure [1900,@] '.' pure [1,12] '.' pure [1,31] blank pure [0,23] ':' pure [0,59] ':' pure [0,59] )\n"
  "ifMatch { \n"
  "  match find( eos ) \n"
  "  end \n"
  "} \n"
  "reset \n"
  "loop",

  "This is a test. Find the sentence written at 2020.01.01 11:03:02. Are there other sentences holding a date? Yes, we started the test on 2021.11.3 09:03:07.",

  {"Find the sentence written at 2020.01.01 11:03:02.", "Yes, we started the test on 2021.11.3 09:03:07."}
},

// ***********************************************************************************\n

{ "task: test if at least two characters are in password, which are not digits or letters\n"
  "how:  repeat 2 times and find characters where all (like logical and) conditions are met\n"
  "      matches will enforce at least 2 results",
  
  "{ matchEnd find( all(!digit !letter char)); } total 2;",
  
  "R188Tl_pohd_",

  {"_", "_"}
},

// ***********************************************************************************\n

{ "task: test password strength, password must hold at least two letters lowercase, \n"
  "      two letters uppercase, one digit, must be a min. of 10 characters long and \n"
  "      hold one non-digit and non-letter\n"
  "how:  scan through text by using 'match test' which will allow to scan over and over again\n"
  "      position pointer is not moved due to test() operand",
  
  "match test (repeat[10,@] (char))\n"
  "ifMatch match test( repeat[2,@]( find( isLower(letter ))))\n"
  "ifMatch match test( repeat[2,@]( find( isUpper(letter ))))\n"
  "ifMatch match test( repeat[1,@]( find( digit )))\n"
  "ifMatch match test( repeat[1,@]( find( all( !digit !letter ))))\n"
  "ifMatch match all( repeat[1,@]( char ))",
  
  "R188Tl_pohd",

  {"R188Tl_pohd"}
},

// ***********************************************************************************\n

{ "task: find last folder in a pure folder path\n"
  "how:  find '\\' but skip (no result), one or more characters not equal '\\'  \n"
  "      and match '\\' but skip (no result), end-of-text must follow" ,
  
  "match find( skip '\\\\' repeat[1,@]( !( '\\\\' ) char ) skip ('\\\\' eot) )", 

  "C:\\Users\\Dean-Office\\Contracts\\Myer 34443\\",

  {"Myer 34443"}
},

// ***********************************************************************************\n

{ "task: find telephone numbers in NANP format\n"
  "how:  find '(' 3 digits and enclosing ')' a space, again 3 digits '-' and 4 digits\n"
  "      end stores result, otherwise several matches would be stored as a single result\n",
  
  "matchEnd find( '(' repeat[3]( digit ) ') ' repeat[3]( digit ) '-' repeat[4]( digit )) \n"
  "loop", 

  "The company may be reached globally under (234) 235-5678, (278) 678-2933, (113) 114-3682 or (227) 145-1993",

  {"(234) 235-5678", "(278) 678-2933", "(113) 114-3682", "(227) 145-1993"}
},

// ***********************************************************************************\n

{ "task: find last word in last sentence in a text\n"
  "how:  find begin of word, followed by at least one letter, followed by at least one \n"
  "      non-letter (for the sentence end, which is not stored due to skip attribute) \n"
  "      and following the end of text." ,
  
  "matchEnd find( bow repeat[1,@]( letter ) skip (repeat[1,@]( !( letter ) char  ) eot ))", 

  "Look for last word, which is the target. ",

  {"target"}
},

// ***********************************************************************************\n

{ "task: find all words in a text\n"
  "how:  match begin of word, if found -> find end of word and store result, loop for more matches",
  
  "match find( bow ) \n"
  "ifMatch { \n"
  "  match find( eow ) \n"
  "  end } \n"
  "loop", 

  "Look for all words.", 

  {"Look", "for", "all", "words"}
},

// ***********************************************************************************\n

{ "task: find last word in a text\n"
  "how:  find word, if a match occured -> replace the last result with new one, loop for more",
  
  "match find (word); ifMatch replace; loop;", 

  "Look for last word, which is target.",

  {"target"}
},

// ***********************************************************************************\n

{ "task: find all sentences in a text\n"
  "how:  match begin of sentence, if found -> match for end of sentence, store result, loop for more",
  
  "match find (bos) \n"
  "ifMatch { \n"
  "  match find( eos ) \n"
  "  end } \n"
  "loop", 

  "The task ist to find all sentences. There are 4 of them in this text. This ist the third one, a longer sentence. Here we have the final sentence. ",

  {"The task ist to find all sentences.", "There are 4 of them in this text.", "This ist the third one, a longer sentence.", "Here we have the final sentence."}
},

// ***********************************************************************************\n

{ "task: find all words that start with 'A' or 'a' and end with 'S' or 's', holding at least one more letter\n"
  "how:  match begin of word, then case insensitive 'A', 2 to infinite letters, \n"
  "      'S' must be last matched letter in last repeat loop'\n",
  
  "matchEnd find( bow ~'A' repeat[2,@]( last(~'S') letter )  )\n"
  "loop", 

  "Find words apples, apes, ants, Amazonas but not as, Arizona, apply",

  {"apples", "apes", "ants", "Amazonas"}
},

// ***********************************************************************************\n

{ "task: find all words that start with 'A' or 'a' and do not end with 'S' or 's', holding at least one more letter\n"
  "how:  match begin of word, then case insensitive 'A', 2 to infinite letters, \n"
  "      'S' must not be last matched letter in last repeat loop'\n",
  
  "matchEnd find( bow ~'A' repeat[2,@]( notlast(~'S') letter )  )\n"
  "loop", 

  "Find words Arizona, apply, agent but not apples, apes, ants, Amazonas",

  {"Arizona", "apply", "agent"}
},

// ***********************************************************************************\n

{ "task: find first word starting with letter 'A' case insensitive \n"
  "how:  find case insensitive 'A' then find end of word",
  
  "match find(~'A' find(eow));", 

  "find the word also and match",

  {"also"}
},

// ***********************************************************************************\n

{ "task: find all words not ending with 'ed'\n"
  "how:  match begin of word, repeat 2 to infinite letters but break operand as soon as 'ed' at word end found",
  
  "matchEnd find( bow repeat[1,@]( letter notlast 'ed')  )\n"
  "loop", 

  "Find words but not ended and blended.",

  {"Find", "words", "but", "not", "and"}
},

// ***********************************************************************************\n

{ "task: find all words not ending with 'e*d' where * = letter\n"
  "how:  match begin of word, test for 'e' letter 'd' , repeat 1 to infinite letters\n"
  "      put attribute last before the letter, otherwise in a short word like 'dad'\n"
  "      the pattern is grabbed",
  
  "matchEnd find( bow repeat[1,@]( last ('d' letter 'd') letter )  )\n"
  "loop", 

  "Find words ended, dad and blended.",

  {"ended", "dad", "blended"}
},

// ***********************************************************************************\n

{ "task: match first word if it starts with A..M or a..m \n"
  "how:  match first letter (A..M,a..m) with any (logical or) and look for all \n"
  "      following letters, (no single letter word allowed)",
  
  "match ( any( char['A','M'] char['a','m'] ) repeat[1,@]( letter ))",

  "Leakey Hofstadter",

  {"Leakey"}
},

// ***********************************************************************************\n

{ "task: find all URLs in text and name results \n"
  "how:  match 'http://' or 'https://', if found -> look for ',' space or end of line or\n" 
  "      end of sentence and store result to start last match or to end if past text (endlast)",

  "match find( any( ~'http://' ~'https://' )) \n"
  "ifMatch { \n"
  "  match find( any( ',' space eol eos )) \n"
  "  tag 'try any tag' \n"
  "  endlast \n"
  "} \n"
  "loop" ,

  "click on http://myownsite.com/, HTTP://anyknownsite.com/,  HTTPS://sales.mysite.com/ or HTTP://myknownsite.com/",

  {"http://myownsite.com/", "HTTP://anyknownsite.com/", "HTTPS://sales.mysite.com/", "HTTP://myknownsite.com/"}
},

// ***********************************************************************************\n

{ "task: find all URLs in text 'http://*.anysite.de/*'  or 'http://anysite.de/' \n"
  "how:  find 'http://' or 'https://'; if found, match 'anysite.de',  \n"
  "      if no match ->  find 'anysite.de' further on but break if delimiter is found\n"
  "      if match -> find any of space, eol, eot or comma at the end of URL",
  
  "match find( any( ~'http://' ~'https://' )) \n"
  "ifMatch { \n"
  "  match 'anysite.de' \n"
  "  ifNoMatch match find( break( any(space eol eot ',' )) ~'.anysite.de/' ) \n"
  "    ifMatch {\n"
  "    match find( any(space eol eot ',' ))  \n"
  "    endlast \n"
  "  } \n"
  "}\n"
  "reset \n"
  "loop",

  "find http://anysite.de/, https://user.wait.anysite.de/, https://anysite.de/ or http://test.anysite.de/ but not http://myanysite.de/",

  {"http://anysite.de/", "https://user.wait.anysite.de/", "https://anysite.de/", "http://test.anysite.de/"}
},
  
// ***********************************************************************************\n

{ "task: find  all expressions like TE(3 letters)ST, ignore case\n"
  "how:  find 'TE' exactly three letters and 'ST', if match -> store result",

  "matchEnd find( ~'TE' repeat[3]( letter ) ~'ST' eow )  \n"
  "loop",

  "search for TEAAAST, TEBBBST, TEaaaST, TEXXXST but not TE123ST, TEAAST, TEST, TEXXXXST",
  
  {"TEAAAST", "TEBBBST", "TEaaaST", "TEXXXST"}
},

// ***********************************************************************************\n

{ "task: find all 'test..s' where .. = any characters not equal to space \n"
  "how:  find 'test' from there on loop for 's' but break on space and loop afterwards",

  "matchEnd ( find( ~'test' findAt[0,@]( break( space ) ~'s' ))) \n"
  "loop",

  "find all tests, testalls, test~s, test123s but not test, tets",
  
  {"tests", "testalls", "test~s", "test123s"}
},  

// ***********************************************************************************\n

{ "task: find all IDs  IDaxxxx-SG  where xxxx = 4 digits\n"
  "how:  find 'IDa' exactly 4 digits and '-SG'",
 
  "matchEnd find( 'IDa' repeat[4]( digit ) '-SG' ) \n"
  "loop",

  "IDa9999-SG, IDa1101-SG, IDa0000-SG, IDa9999-SG but not IDa9977-ST, IDa997-SG or IDA9997-SG",
  
  {"IDa9999-SG", "IDa1101-SG", "IDa0000-SG", "IDa9999-SG"}
},

// ***********************************************************************************\n

{ "task find all IDs  IDax-SG  where x between 1 and 4 digits\n"
  "how:  find 'IDa' 1 up to 4 digits and '-SG'",
  
  "matchEnd find( 'IDa' repeat[1,4]( digit ) '-SG' ) \n"
  "loop", 

 "IDa1-SG, IDa11-SG, IDa0000-SG, IDa9999-SG but not IDa99777-ST, IDa-SG or IDA9997",

  {"IDa1-SG", "IDa11-SG", "IDa0000-SG", "IDa9999-SG"}
},

// ***********************************************************************************\n

{ "task: find all words case insensitive 'test' or 'tests'\n"
  "how:  look for 'test(s)' and afterwards 'test' if the match is a true word",

  "matchEnd find( any( isWord( ~'test' ) isWord( ~'tests' )))  \n"
  "loop",

  "find all words like test, TEST or tests in tests but not tes, TESTSS",

  {"test", "TEST", "tests", "tests"}
},

// ***********************************************************************************\n

{ "task: find float number  +/- digit(s).digit(s)\n"
  "how: look for '+', '-' one digit, possibly more , '.' and at least one more digit",

  "matchEnd find( any( '+' '-' digit ) repeat[0,@]( digit ) '.' repeat[1,@]( digit ))  \n"
  "loop",

  "find a float number like 1.4664 , -56.787 and -.3784 also +.343, not .8798, 56, 123 and not -98",

  {"1.4664", "-56.787", "-.3784", "+.343"}
},

// ***********************************************************************************\n

{ "* task: find float number +/-ddd.ddd and also .ddd\n"  
  "how: look for '+', '-' one digit, possibly more , '.' and at least one more digit\n"
  "     if no match -> try to find '.' and at least one digit",

  "match all( any( '+' '-' digit ) repeat[0,@]( digit ) '.' repeat[1,@]( digit ))  \n"
  "ifMatch { end \n"
  "} else { \n"
  "  match all( '.' repeat[1,@]( digit )) \n"
  "  ifMatch end \n"
  "}\n"
  "moveon \n"
  "loop",

  "find a float number -56.787,-.374 and .8798, also +7.343 and not -98",

  {"-56.787", "-.374", ".8798", "+7.343"}
},

// ***********************************************************************************\n
{ "task: find first code of format AAxxx , where xxx are 3 digits, set name 'code'\n"
  "      then all other same occurrences\n" 
  "how:  look for 'AA' and 3 digits, if found -> give name #code'\n"
  "      then repeat looking for the content of #code#",

  "match find( 'AA' repeat[3]( digit )) \n"
  "name #code# \n"
  "ifMatch { \n"
  "  end \n"
  "  match find ( #code# ) \n"
  "} loop",

  "find code AA456 and other occurences in: AA454, AA455, AA456, AA457 , AA456, AA458, AA456, AA454",

  {"AA456", "AA456", "AA456", "AA456"}
},

// ***********************************************************************************\n
  
{ "task: find entire sentence holding phrase 'John Doe'\n"
  "how:  look for begin of sentence and a letter, if match->find 'John Doe' but break on\n"
  "      end of sentence, if found -> find character + end of sentence and store result\n"
  "      reset result for non completed match and loop",

  "match find( bos letter ) \n"
  "ifMatch match find( break( eos ) 'John Doe' ) \n"
  "ifMatch {\n"
  "  match find( char eos ) \n"
  "  end \n"
  "} \n"
  "reset \n"
"  loop",

  "This is a test. Find Mr John Doe in this text and extract the sentence. That's all.",

  {"Find Mr John Doe in this text and extract the sentence."}
},

// ***********************************************************************************\n

{ "task: find all words with xat, where x = h, c, s, b, example 'cat' but not 'cats'\n"   
  "how:  look for any of the letters followed by 'at' and see if it is a true word",

  "matchEnd find( isWord( any( 'h' 'c' 's' 'b' ) 'at' )) \n"
  "loop ",

  "A cat sat on my hat and talked with a bat but not with other cats.",

  {"cat", "sat", "hat", "bat"}
},

// ***********************************************************************************\n

{ "task: todo: find all numbers between 1 and 200\n"  
  "how:  specify range",

  "matchEnd find ( [1,200] ) \n"
  "loop",

  "Find numbers +3, 199, 60, 80.9 but not 0 , -70 or 201",

  {"+3", "199", "60", "80.9"}
},

// ***********************************************************************************\n

{ "task: find all integer numbers between -200 and 200\n"
  "how:  specify range",
    
  "matchEnd find( int[-200,200] ) \n"
  "loop",

  "Find integer numbers 0, 3, 199, -60 but not 3.5, .70, 500 or +199.67",

  {"0", "3", "199", "-60"}
},

// ***********************************************************************************\n

{ "task: find all IPv4 addresses\n"
  "how:  find bundles of pure numbers in range 0.255 followed by '.'",

  "matchEnd find( pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255] ) \n"
  "loop",

  "128.0.1.1, 45.6.7.9, 255.255.255.0 and 0.0.0.0, but not 1234.5.7.4, 66.89.56.2222 or 257.45.232.4",

  {"128.0.1.1", "45.6.7.9", "255.255.255.0", "0.0.0.0"}
} ,

// ***********************************************************************************\n

{ "task: find all IPv6 addresses\n"
  "how:  look for start (at) pure hex number or ':' preceded by space, ',' or bos for start of IP\n"
  "      look for pure hex number or ':' , match only hex number but break on ':::', loop for more\n"
  "      'match at' only sets position pointer to begin of match but no result",

  "match at find( any( space ',' bos ) any( pure hex[0,65535] ':' ))\n"
  "ifMatch { \n"
  "  match at find( any( pure hex[0,65535] ':'))\n"
  "  match every( break( ':::' ) pure hex[0,65535] break( ':::' ) ':' ) \n"
  "  ifMatch end \n"
  "}\n"
  "loop" ,

  ", a00:b100:0:128:0:66:bc:0, a00:b100::66::bc:0, ::a:b10:bc:0, ffff:b1:d:fe:0:66:bc:0 but not :::b100:0:128:0:66:bc:0 or b100:0:::128:0:66:bc:0",

  {"a00:b100:0:128:0:66:bc:0", "a00:b100::66::bc:0", "::a:b10:bc:0", "ffff:b1:d:fe:0:66:bc:0"}
}, 

// ***********************************************************************************\n

{ "task: find all Email addresses\n"
  "how:  look for any of space eos '<' ',', match at least 3 letters or digits + '@' plus least one letter\n"
  "      if match -> find any of delimiters given including '.' + end of text -> store up to delimiter (endlast)",

  "match skip find( any(space eos '<' ',') )\n"
  "ifMatch { match (repeat[3,@] (any(letter digit)) '@' letter) \n"
  "ifMatch { match find( any( '>', ',' space eol eos all('.' eot)) )\n"
  "endlast }}\n"
  "loop;" ,

  " Test if we can find homer@domain.com or fred1@my.domain.com in a text but not \n in next sentence tt@domain.com 1@THDG or zsh6@.\n In last sentence find fred2@mail.domain.com. ",

  {"homer@domain.com", "fred1@my.domain.com", "fred2@mail.domain.com"}
}, 

// ***********************************************************************************\n

{ "task: find all christian names with surname Smith\n"
  "how:  look for word, space and 'smith' but only set position, if match->store first word",

  "match at find( word blank ~'smith' !( letter )) \n"
  "ifMatch { \n"
  "  matchEnd ( word ) \n"
  "}\n"
  "loop",

  " Mr.Owen Smith, Clive Smith, Jack Smith, Olson Smith but not Carla Smiths, Buddy Jacksmith or Michael Smithsonian", 

  {"Owen", "Clive", "Jack", "Olson"}
}, 

// ***********************************************************************************\n

{ "task: find word 'test' + letter after word 'unit '\n"
  "how:  shows how to use skip attribute",

  "matchEnd find( all (skip 'unit ' 'test' letter))\n"
  "ifMatch end\n"
  "reset\n"
  "loop",

  "tests, main tests, unit testb, example tests ",

  {"testb"}
},

// ***********************************************************************************\n

{ "task: find word 'test' + letter after any other word but 'unit'\n"
  "how:  shows how to user 'unless' attribute as a lookbehind",

  "matchEnd find( all (unless('unit ') 'test' letter))\n"
  "reset\n"
  "loop",

  "testa, main testb, unit tests",

  {"testa", "testb"}
},

// ***********************************************************************************\n

{ "task: find all words that are not followed by the word 'test' excluding the word 'test' itself\n"
  "how:  match begin of word, then check that it is not 'test', repeat 2 to infinite letters, \n"
  "      at the end make sure that ist is not followed by one/many spaces and 'test'",
  
  "matchEnd find( bow !isWord('test') repeat[1,@]( letter ) !(blank 'test')  )\n"
  "loop", 

  "Find words module, unit test, main  test, object, program.",

  {"Find", "words", "module", "object", "program"}
},

// ***********************************************************************************\n

{ "task: find all words not ending with 'ed'\n"
  "how:  match begin of word, repeat 1 to infinite letters but break operand as soon as 'ed' at word end found",
  
  "matchEnd find( bow repeat[1,@]( letter break('ed' eow))  )\n"
  "loop", 

  "Find words but not ended and blended.",

  {"Find", "words", "but", "not", "and"}
},

// ***********************************************************************************\n

{ "task: test long string (100 MB), find text at the end and measure time in ms\n"
  "how:  find case insensitive 't' and T'",

  "match find( ~'t' 'T' )",

  std::string(100000000,'X') + "TT",

  {"TT"}
}, 

// ***********************************************************************************\n

{ "task: find address in the form street name, postal code city in single line, using templates\n"
  "how:  make template with common words for roads, find house number, road, postal code and city",

  "< roadMarker = { any( ~'avenue' ~'ave.' ~'road' ~'street' ~'boulevard'  ~'drive'  ~'lane' ) } > \n"
  "\n"
  "match find( int[1,10000] blank repeat[1,3]( !(< roadMarker >) word ) blank < roadMarker > ) \n"
  "ifMatch { \n"
  "  matchEnd ( ',' blank int[1,@] repeat[1,3]( blank word ) at any(',' eol eot ))\n"
  "} \n"
  "loop ",

  "He lives in 1007 Mountain Drive, 63527 Gotham City, and is a superhero with questionable reputation. His buddy lives in 1938 Sullivan Lane, 55555 Metropolis,", 

  {"1007 Mountain Drive, 63527 Gotham City", "1938 Sullivan Lane, 55555 Metropolis"}
},
// ***********************************************************************************\n

{ "task: find parameters of function configGetChar()\n"
  "how:  find string 'configgetchar' case insensitive ~.\n\
      If match -> include all characters not equal to ')'\n\
      also possible in second clause:  ifMatch match every( all(!')' char) );\n\
      here we match as long as character and not ')' is fullfilled, all() is logical and \n\
      since all() is the default operator, this is even shorter: ifMatch match every( (!')' char) );  ", 

  "match skip find( ~'configgetchar(' );  ifMatch match ( repeat[1,@]( !')' char) );", 

  "void configSet(const char* aKey, int aKeylen);..... bool configGetChar(const char* aKey, int aKeylen, char32_t*aValue);....",

  {"const char* aKey, int aKeylen, char32_t*aValue"}
},
// ***********************************************************************************\n

{ "task: match JSON element where sub element cat is '127.0.4.1', *.bites is 0, status: 0 and url is an IPv4\n"
  "how:  match chain, if branch fails skip rest and reset result, if last condition matches, end result\n"
  "      break on every '{' once in an element to avoid skipping over JSON element",
  
  "match find ('element' all(digit) ': {')\n"
  "ifMatch match find( break('{') '\"cat\": \"127.0.4.1\"' ) \n"
  "ifMatch match find( break('{') '\"bites\": 0') \n"
  "ifMatch match find( break('{') '\"status\": ' 0) \n"
  "ifMatch match find( break('{') '\"url\": ''\"' pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255] ) \n"
  "ifMatch match find( break('{') '}' ) \n"
  "ifMatch end\n" 
  "else reset\n"
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



};

}; // end of namespace

#endif