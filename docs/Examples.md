# Scripal examples

<pre>
-------------------------------------------------------------------------------
example 1

task: match characters from several UNICODE pages, compare characters from position 0 on
      to find text fragments in source text use match( find...  how:  simple text match with letters from several languages

----- source -----

match ( 'aËಭݣԓԖ𑜔ࢡÄz' )

----- text -----

aËಭݣԓԖ𑜔ࢡÄz

----- results -----

match: 'aËಭݣԓԖ𑜔ࢡÄz', pos[0, 21]

total results: 1

-------------------------------------------------------------------------------
example 2

task: find first character after end of sentence sign
how:  find end of sentence and space (skip the result, don't store), if found -> match next letter

----- source -----

match skip find( eos space ) 
ifMatch matchEnd( letter )

----- text -----

End of sentence. Now start the next sentence.

----- results -----

match: 'N', pos[17, 17]

total results: 1

-------------------------------------------------------------------------------
example 3

task: find single words that are followed by one or many spaces and 'more'
how:  find word, only test (don't store result) if followed by spaces and 'more'
      if match->store result, reset in case a match cannot be completed and loop

----- source -----

match find( word ) 
match test( blank 'more' )  
ifMatch end
reset
loop

----- text -----

Find the word that is followed by more and see if there is more in the text.

----- results -----

match: 'by', pos[31, 32]
match: 'is', pos[56, 57]

total results: 2

-------------------------------------------------------------------------------
example 4

task: find all words that begin with an 'a'
how:  find begin of word and 'a', if match -> find the end of the word and store result
      loop for more words

----- source -----

match find(bow 'a');
ifMatch matchEnd find(eow); loop;

----- text -----

Find the words apple, ape and award.

----- results -----

match: 'apple', pos[15, 19]
match: 'ape', pos[22, 24]
match: 'and', pos[26, 28]
match: 'award', pos[30, 34]

total results: 4

-------------------------------------------------------------------------------
example 5

task: match hex number in the range 0001-ff00
how:  specify hex attribute and number range

----- source -----

match ( hex [1,ff00] )

----- text -----

fe78

----- results -----

match: 'fe78', pos[0, 3]

total results: 1

-------------------------------------------------------------------------------
example 6

task: match serial number in the range 678[900-999]-[0,9]
how:  match '678', see if followed by 3 digits which form number in the range 900-999
      followed by '-' and a digit, if true -> end result, else reset entire result

----- source -----

match ( '678' )
ifMatch { match ( isNumber[900,999]( digit digit digit ) '-' digit );}

----- text -----

678901-8

----- results -----

match: '678901-8', pos[0, 7]

total results: 1

-------------------------------------------------------------------------------
example 7

task: match time range from 2020.07.14 8:10 to 8:14
how:  match pure number 2020 followed by . 7 . 14 spaces 8 : and numbers in the range 10-14
      use pure numbers instead of int otherwise 2020.xx would be seen as a float number

----- source -----

match ( pure 2020 '.' pure 7 '.'  pure 14 blank pure 8 ':' int [10,14] )

----- text -----

2020.7.14 8:10

----- results -----

match: '2020.7.14 8:10', pos[0, 13]

total results: 1

-------------------------------------------------------------------------------
example 8

task: match all dates from 1900 on with a certain accuracy
how:  find pure number 1900..infinity followed by '.' [1..12] '.' [1.31]
      use pure numbers instead of int otherwise 1900.1 would be seen as a float number!

----- source -----

matchEnd find( pure [1900,@] '.' pure [1,12] '.' pure [1,31] )
loop

----- text -----

Find dates like 2020.12.30 or 2021.01.14 but not numbers like 12344.23 or 12344.23.12

----- results -----

match: '2020.12.30', pos[16, 25]
match: '2021.01.14', pos[30, 39]

total results: 2

-------------------------------------------------------------------------------
example 9

task: find dates with higher accuracy between 1900-01-01 and 1999-12-31 in this format
      a leap year is not taken into account
how:  find at (without storing) pure number 1900..1999 and digits in the form dd-dd
      if matched reset result, store year and test for month's ranges

----- source -----

match at find( int[1900,1999] '-' digit digit '-' digit digit ) 
ifMatch { 
  reset 
  match ( repeat[4]( digit ) '-') 
  match ( 1  '-' [1,31] ) 
  ifNoMatch match ( 2  '-' [1,29] ) 
  ifNoMatch match ( 3  '-' [1,31] ) 
  ifNoMatch match ( 4  '-' [1,30] ) 
  ifNoMatch match ( 5  '-' [1,31] ) 
  ifNoMatch match ( 6  '-' [1,30] ) 
  ifNoMatch match ( 7  '-' [1,31] ) 
  ifNoMatch match ( 8  '-' [1,31] ) 
  ifNoMatch match ( 9  '-' [1,30] ) 
  ifNoMatch match ( 10 '-' [1,31] ) 
  ifNoMatch match ( 11 '-' [1,30] ) 
  ifNoMatch match ( 12 '-' [1,31] ) 
  ifMatch end 
} 
reset
loop

----- text -----

Find 1924-01-31, 1900-07-14, 1900-01-01 and 1999-02-28 but not 1999-2-28, 1999-2-31, 1180-07-14, 2000-01-31 OR 1999.2.28

----- results -----

match: '1924-01-31', pos[5, 14]
match: '1900-07-14', pos[17, 26]
match: '1900-01-01', pos[29, 38]
match: '1999-02-28', pos[44, 53]

total results: 4

-------------------------------------------------------------------------------
example 10

task: todo: find entire sentence holding a date + time
how:  find begin of sentence, find pure numbers according to date format used in your country
      if found -> find end of sentence


----- source -----

match find (bos letter) 
ifMatch match find( break( eos ) pure [1900,@] '.' pure [1,12] '.' pure [1,31] blank pure [0,23] ':' pure [0,59] ':' pure [0,59] )
ifMatch { 
  match find( eos ) 
  end 
} 
reset 
loop

----- text -----

This is a test. Find the sentence written at 2020.01.01 11:03:02. Are there other sentences holding a date? Yes, we started the test on 2021.11.3 09:03:07.

----- results -----

match: 'Find the sentence written at 2020.01.01 11:03:02.', pos[16, 64]
match: 'Yes, we started the test on 2021.11.3 09:03:07.', pos[108, 154]

total results: 2

-------------------------------------------------------------------------------
example 11

task: test if at least two characters are in password, which are not digits or letters
how:  repeat 2 times and find characters where all (like logical and) conditions are met
      matches will enforce at least 2 results

----- source -----

{ matchEnd find( all(!digit !letter char)); } total 2;

----- text -----

R188Tl_pohd_

----- results -----

match: '_', pos[6, 6]
match: '_', pos[11, 11]

total results: 2

-------------------------------------------------------------------------------
example 12

task: test password strength, password must hold at least two letters lowercase, 
      two letters uppercase, one digit, must be a min. of 10 characters long and 
      hold one non-digit and non-letter
how:  scan through text by using 'match test' which will allow to scan over and over again
      position pointer is not moved due to test() operand

----- source -----

match test (repeat[10,@] (char))
ifMatch match test( repeat[2,@]( find( isLower(letter ))))
ifMatch match test( repeat[2,@]( find( isUpper(letter ))))
ifMatch match test( repeat[1,@]( find( digit )))
ifMatch match test( repeat[1,@]( find( all( !digit !letter ))))
ifMatch match all( repeat[1,@]( char ))

----- text -----

R188Tl_pohd

----- results -----

match: 'R188Tl_pohd', pos[0, 10]

total results: 1

-------------------------------------------------------------------------------
example 13

task: find last folder in a pure folder path
how:  find '\' but skip (no result), one or more characters not equal '\'  
      and match '\' but skip (no result), end-of-text must follow

----- source -----

match find( skip '\\' repeat[1,@]( !( '\\' ) char ) skip ('\\' eot) )

----- text -----

C:\Users\Dean-Office\Contracts\Myer 34443\

----- results -----

match: 'Myer 34443', pos[31, 40]

total results: 1

-------------------------------------------------------------------------------
example 14

task: find telephone numbers in NANP format
how:  find '(' 3 digits and enclosing ')' a space, again 3 digits '-' and 4 digits
      end stores result, otherwise several matches would be stored as a single result


----- source -----

matchEnd find( '(' repeat[3]( digit ) ') ' repeat[3]( digit ) '-' repeat[4]( digit )) 
loop

----- text -----

The company may be reached globally under (234) 235-5678, (278) 678-2933, (113) 114-3682 or (227) 145-1993

----- results -----

match: '(234) 235-5678', pos[42, 55]
match: '(278) 678-2933', pos[58, 71]
match: '(113) 114-3682', pos[74, 87]
match: '(227) 145-1993', pos[92, 105]

total results: 4

-------------------------------------------------------------------------------
example 15

task: find last word in last sentence in a text
how:  find begin of word, followed by at least one letter, followed by at least one 
      non-letter (for the sentence end, which is not stored due to skip attribute) 
      and following the end of text.

----- source -----

matchEnd find( bow repeat[1,@]( letter ) skip (repeat[1,@]( !( letter ) char  ) eot ))

----- text -----

Look for last word, which is the target. 

----- results -----

match: 'target', pos[33, 38]

total results: 1

-------------------------------------------------------------------------------
example 16

task: find all words in a text
how:  match begin of word, if found -> find end of word and store result, loop for more matches

----- source -----

match find( bow ) 
ifMatch { 
  match find( eow ) 
  end } 
loop

----- text -----

Look for all words.

----- results -----

match: 'Look', pos[0, 3]
match: 'for', pos[5, 7]
match: 'all', pos[9, 11]
match: 'words', pos[13, 17]

total results: 4

-------------------------------------------------------------------------------
example 17

task: find last word in a text
how:  find word, if a match occured -> replace the last result with new one, loop for more

----- source -----

match find (word); ifMatch replace; loop;

----- text -----

Look for last word, which is target.

----- results -----

match: 'target', pos[29, 34]

total results: 1

-------------------------------------------------------------------------------
example 18

task: find all sentences in a text
how:  match begin of sentence, if found -> match for end of sentence, store result, loop for more

----- source -----

match find (bos) 
ifMatch { 
  match find( eos ) 
  end } 
loop

----- text -----

The task ist to find all sentences. There are 4 of them in this text. This ist the third one, a longer sentence. Here we have the final sentence. 

----- results -----

match: 'The task ist to find all sentences.', pos[0, 34]
match: 'There are 4 of them in this text.', pos[36, 68]
match: 'This ist the third one, a longer sentence.', pos[70, 111]
match: 'Here we have the final sentence.', pos[113, 144]

total results: 4

-------------------------------------------------------------------------------
example 19

task: find all words that start with 'A' or 'a' and end with 'S' or 's', holding at least one more letter
how:  match begin of word, then case insensitive 'A', 2 to infinite letters, 
      'S' must be last matched letter in last repeat loop'


----- source -----

matchEnd find( bow ~'A' repeat[2,@]( last(~'S') letter )  )
loop

----- text -----

Find words apples, apes, ants, Amazonas but not as, Arizona, apply

----- results -----

match: 'apples', pos[11, 16]
match: 'apes', pos[19, 22]
match: 'ants', pos[25, 28]
match: 'Amazonas', pos[31, 38]

total results: 4

-------------------------------------------------------------------------------
example 20

task: find all words that start with 'A' or 'a' and do not end with 'S' or 's', holding at least one more letter
how:  match begin of word, then case insensitive 'A', 2 to infinite letters, 
      'S' must not be last matched letter in last repeat loop'


----- source -----

matchEnd find( bow ~'A' repeat[2,@]( notlast(~'S') letter )  )
loop

----- text -----

Find words Arizona, apply, agent but not apples, apes, ants, Amazonas

----- results -----

match: 'Arizona', pos[11, 17]
match: 'apply', pos[20, 24]
match: 'agent', pos[27, 31]

total results: 3

-------------------------------------------------------------------------------
example 21

task: find first word starting with letter 'A' case insensitive 
how:  find case insensitive 'A' then find end of word

----- source -----

match find(~'A' find(eow));

----- text -----

find the word also and match

----- results -----

match: 'also', pos[14, 17]

total results: 1

-------------------------------------------------------------------------------
example 22

task: find all words not ending with 'ed'
how:  match begin of word, repeat 2 to infinite letters but break operand as soon as 'ed' at word end found

----- source -----

matchEnd find( bow repeat[1,@]( letter notlast 'ed')  )
loop

----- text -----

Find words but not ended and blended.

----- results -----

match: 'Find', pos[0, 3]
match: 'words', pos[5, 9]
match: 'but', pos[11, 13]
match: 'not', pos[15, 17]
match: 'and', pos[25, 27]

total results: 5

-------------------------------------------------------------------------------
example 23

task: find all words not ending with 'e*d' where * = letter
how:  match begin of word, test for 'e' letter 'd' , repeat 1 to infinite letters
      put attribute last before the letter, otherwise in a short word like 'dad'
      the pattern is grabbed

----- source -----

matchEnd find( bow repeat[1,@]( last ('d' letter 'd') letter )  )
loop

----- text -----

Find words ended, dad and blended.

----- results -----

match: 'ended', pos[11, 15]
match: 'dad', pos[18, 20]
match: 'blended', pos[26, 32]

total results: 3

-------------------------------------------------------------------------------
example 24

task: match first word if it starts with A..M or a..m 
how:  match first letter (A..M,a..m) with any (logical or) and look for all 
      following letters, (no single letter word allowed)

----- source -----

match ( any( char['A','M'] char['a','m'] ) repeat[1,@]( letter ))

----- text -----

Leakey Hofstadter

----- results -----

match: 'Leakey', pos[0, 5]

total results: 1

-------------------------------------------------------------------------------
example 25

task: find all URLs in text and name results 
how:  match 'http://' or 'https://', if found -> look for ',' space or end of line or
      end of sentence and store result to start last match or to end if past text (endlast)

----- source -----

match find( any( ~'http://' ~'https://' )) 
ifMatch { 
  match find( any( ',' space eol eos )) 
  tag 'try any tag' 
  endlast 
} 
loop

----- text -----

click on http://myownsite.com/, HTTP://anyknownsite.com/,  HTTPS://sales.mysite.com/ or HTTP://myknownsite.com/

----- results -----

match: 'http://myownsite.com/', pos[9, 29]  tag: 'try any tag'
match: 'HTTP://anyknownsite.com/', pos[32, 55]  tag: 'try any tag'
match: 'HTTPS://sales.mysite.com/', pos[59, 83]  tag: 'try any tag'
match: 'HTTP://myknownsite.com/', pos[88, 110]  tag: 'try any tag'

total results: 4

-------------------------------------------------------------------------------
example 26

task: find all URLs in text 'http://*.anysite.de/*'  or 'http://anysite.de/' 
how:  find 'http://' or 'https://'; if found, match 'anysite.de',  
      if no match ->  find 'anysite.de' further on but break if delimiter is found
      if match -> find any of space, eol, eot or comma at the end of URL

----- source -----

match find( any( ~'http://' ~'https://' )) 
ifMatch { 
  match 'anysite.de' 
  ifNoMatch match find( break( any(space eol eot ',' )) ~'.anysite.de/' ) 
    ifMatch {
    match find( any(space eol eot ',' ))  
    endlast 
  } 
}
reset 
loop

----- text -----

find http://anysite.de/, https://user.wait.anysite.de/, https://anysite.de/ or http://test.anysite.de/ but not http://myanysite.de/

----- results -----

match: 'http://anysite.de/', pos[5, 22]
match: 'https://user.wait.anysite.de/', pos[25, 53]
match: 'https://anysite.de/', pos[56, 74]
match: 'http://test.anysite.de/', pos[79, 101]

total results: 4

-------------------------------------------------------------------------------
example 27

task: find  all expressions like TE(3 letters)ST, ignore case
how:  find 'TE' exactly three letters and 'ST', if match -> store result

----- source -----

matchEnd find( ~'TE' repeat[3]( letter ) ~'ST' eow )  
loop

----- text -----

search for TEAAAST, TEBBBST, TEaaaST, TEXXXST but not TE123ST, TEAAST, TEST, TEXXXXST

----- results -----

match: 'TEAAAST', pos[11, 17]
match: 'TEBBBST', pos[20, 26]
match: 'TEaaaST', pos[29, 35]
match: 'TEXXXST', pos[38, 44]

total results: 4

-------------------------------------------------------------------------------
example 28

task: find all 'test..s' where .. = any characters not equal to space 
how:  find 'test' from there on loop for 's' but break on space and loop afterwards

----- source -----

matchEnd ( find( ~'test' findAt[0,@]( break( space ) ~'s' ))) 
loop

----- text -----

find all tests, testalls, test~s, test123s but not test, tets

----- results -----

match: 'tests', pos[9, 13]
match: 'testalls', pos[16, 23]
match: 'test~s', pos[26, 31]
match: 'test123s', pos[34, 41]

total results: 4

-------------------------------------------------------------------------------
example 29

task: find all IDs  IDaxxxx-SG  where xxxx = 4 digits
how:  find 'IDa' exactly 4 digits and '-SG'

----- source -----

matchEnd find( 'IDa' repeat[4]( digit ) '-SG' ) 
loop

----- text -----

IDa9999-SG, IDa1101-SG, IDa0000-SG, IDa9999-SG but not IDa9977-ST, IDa997-SG or IDA9997-SG

----- results -----

match: 'IDa9999-SG', pos[0, 9]
match: 'IDa1101-SG', pos[12, 21]
match: 'IDa0000-SG', pos[24, 33]
match: 'IDa9999-SG', pos[36, 45]

total results: 4

-------------------------------------------------------------------------------
example 30

task find all IDs  IDax-SG  where x between 1 and 4 digits
how:  find 'IDa' 1 up to 4 digits and '-SG'

----- source -----

matchEnd find( 'IDa' repeat[1,4]( digit ) '-SG' ) 
loop

----- text -----

IDa1-SG, IDa11-SG, IDa0000-SG, IDa9999-SG but not IDa99777-ST, IDa-SG or IDA9997

----- results -----

match: 'IDa1-SG', pos[0, 6]
match: 'IDa11-SG', pos[9, 16]
match: 'IDa0000-SG', pos[19, 28]
match: 'IDa9999-SG', pos[31, 40]

total results: 4

-------------------------------------------------------------------------------
example 31

task: find all words case insensitive 'test' or 'tests'
how:  look for 'test(s)' and afterwards 'test' if the match is a true word

----- source -----

matchEnd find( any( isWord( ~'test' ) isWord( ~'tests' )))  
loop

----- text -----

find all words like test, TEST or tests in tests but not tes, TESTSS

----- results -----

match: 'test', pos[20, 23]
match: 'TEST', pos[26, 29]
match: 'tests', pos[34, 38]
match: 'tests', pos[43, 47]

total results: 4

-------------------------------------------------------------------------------
example 32

task: find float number  +/- digit(s).digit(s)
how: look for '+', '-' one digit, possibly more , '.' and at least one more digit

----- source -----

matchEnd find( any( '+' '-' digit ) repeat[0,@]( digit ) '.' repeat[1,@]( digit ))  
loop

----- text -----

find a float number like 1.4664 , -56.787 and -.3784 also +.343, not .8798, 56, 123 and not -98

----- results -----

match: '1.4664', pos[25, 30]
match: '-56.787', pos[34, 40]
match: '-.3784', pos[46, 51]
match: '+.343', pos[58, 62]

total results: 4

-------------------------------------------------------------------------------
example 33

* task: find float number +/-ddd.ddd and also .ddd
how: look for '+', '-' one digit, possibly more , '.' and at least one more digit
     if no match -> try to find '.' and at least one digit

----- source -----

match all( any( '+' '-' digit ) repeat[0,@]( digit ) '.' repeat[1,@]( digit ))  
ifMatch { end 
} else { 
  match all( '.' repeat[1,@]( digit )) 
  ifMatch end 
}
moveon 
loop

----- text -----

find a float number -56.787,-.374 and .8798, also +7.343 and not -98

----- results -----

match: '-56.787', pos[20, 26]
match: '-.374', pos[28, 32]
match: '.8798', pos[38, 42]
match: '+7.343', pos[50, 55]

total results: 4

-------------------------------------------------------------------------------
example 34

task: find first code of format AAxxx , where xxx are 3 digits, set name 'code'
      then all other same occurrences
how:  look for 'AA' and 3 digits, if found -> give name #code'
      then repeat looking for the content of #code#

----- source -----

match find( 'AA' repeat[3]( digit )) 
name #code# 
ifMatch { 
  end 
  match find ( #code# ) 
} loop

----- text -----

find code AA456 and other occurences in: AA454, AA455, AA456, AA457 , AA456, AA458, AA456, AA454

----- results -----

match: 'AA456', pos[10, 14]
match: 'AA456', pos[55, 59]
match: 'AA456', pos[70, 74]
match: 'AA456', pos[84, 88]

total results: 4

-------------------------------------------------------------------------------
example 35

task: find entire sentence holding phrase 'John Doe'
how:  look for begin of sentence and a letter, if match->find 'John Doe' but break on
      end of sentence, if found -> find character + end of sentence and store result
      reset result for non completed match and loop

----- source -----

match find( bos letter ) 
ifMatch match find( break( eos ) 'John Doe' ) 
ifMatch {
  match find( char eos ) 
  end 
} 
reset 
  loop

----- text -----

This is a test. Find Mr John Doe in this text and extract the sentence. That's all.

----- results -----

match: 'Find Mr John Doe in this text and extract the sentence.', pos[16, 70]

total results: 1

-------------------------------------------------------------------------------
example 36

task: find all words with xat, where x = h, c, s, b, example 'cat' but not 'cats'
how:  look for any of the letters followed by 'at' and see if it is a true word

----- source -----

matchEnd find( isWord( any( 'h' 'c' 's' 'b' ) 'at' )) 
loop 

----- text -----

A cat sat on my hat and talked with a bat but not with other cats.

----- results -----

match: 'cat', pos[2, 4]
match: 'sat', pos[6, 8]
match: 'hat', pos[16, 18]
match: 'bat', pos[38, 40]

total results: 4

-------------------------------------------------------------------------------
example 37

task: todo: find all numbers between 1 and 200
how:  specify range

----- source -----

matchEnd find ( [1,200] ) 
loop

----- text -----

Find numbers +3, 199, 60, 80.9 but not 0 , -70 or 201

----- results -----

match: '+3', pos[13, 14]
match: '199', pos[17, 19]
match: '60', pos[22, 23]
match: '80.9', pos[26, 29]

total results: 4

-------------------------------------------------------------------------------
example 38

task: find all integer numbers between -200 and 200
how:  specify range

----- source -----

matchEnd find( int[-200,200] ) 
loop

----- text -----

Find integer numbers 0, 3, 199, -60 but not 3.5, .70, 500 or +199.67

----- results -----

match: '0', pos[21, 21]
match: '3', pos[24, 24]
match: '199', pos[27, 29]
match: '-60', pos[32, 34]

total results: 4

-------------------------------------------------------------------------------
example 39

task: find all IPv4 addresses
how:  find bundles of pure numbers in range 0.255 followed by '.'

----- source -----

matchEnd find( pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255] ) 
loop

----- text -----

, 128.0.1.1, 45.6.7.9, 255.255.255.0 and 0.0.0.0, but not 1234.5.7.4, 66.89.56.2222 or 257.45.232.4

----- results -----

match: '128.0.1.1', pos[2, 10]
match: '45.6.7.9', pos[13, 20]
match: '255.255.255.0', pos[23, 35]
match: '0.0.0.0', pos[41, 47]

total results: 4

-------------------------------------------------------------------------------
example 40

task: find all IPv6 addresses
how:  look for start (at) pure hex number or ':' preceded by space, ',' or bos for start of IP
      look for pure hex number or ':' , match only hex number but break on ':::', loop for more
      'match at' only sets position pointer to begin of match but no result

----- source -----

match at find( any( space ',' bos ) any( pure hex[0,65535] ':' ))
ifMatch { 
  match at find( any( pure hex[0,65535] ':'))
  match every( break( ':::' ) pure hex[0,65535] break( ':::' ) ':' ) 
  ifMatch end 
}
loop

----- text -----

, a00:b100:0:128:0:66:bc:0, a00:b100::66::bc:0, ::a:b10:bc:0, ffff:b1:d:fe:0:66:bc:0 but not :::b100:0:128:0:66:bc:0 or b100:0:::128:0:66:bc:0

----- results -----

match: 'a00:b100:0:128:0:66:bc:0', pos[2, 25]
match: 'a00:b100::66::bc:0', pos[28, 45]
match: '::a:b10:bc:0', pos[48, 59]
match: 'ffff:b1:d:fe:0:66:bc:0', pos[62, 83]

total results: 4

-------------------------------------------------------------------------------
example 41

task: find all Email addresses
how:  look for any of space eos '<' ',', match at least 3 letters or digits + '@' plus least one letter
      if match -> find any of delimiters given including '.' + end of text -> store up to delimiter (endlast)

----- source -----

match skip find( any(space eos '<' ',') )
ifMatch { match (repeat[3,@] (any(letter digit)) '@' letter) 
ifMatch { match find( any( '>', ',' space eol eos all('.' eot)) )
endlast }}
loop;

----- text -----

 Test if we can find homer@domain.com or fred1@my.domain.com in a text but not 
 in next sentence tt@domain.com 1@THDG or zsh6@.
 In last sentence find fred2@mail.domain.com. 

----- results -----

match: 'homer@domain.com', pos[21, 36]
match: 'fred1@my.domain.com', pos[41, 59]
match: 'fred2@mail.domain.com', pos[152, 172]

total results: 3

-------------------------------------------------------------------------------
example 42

task: find all christian names with surname Smith
how:  look for word, space and 'smith' but only set position, if match->store first word

----- source -----

match at find( word blank ~'smith' !( letter )) 
ifMatch { 
  matchEnd ( word ) 
}
loop

----- text -----

 Mr.Owen Smith, Clive Smith, Jack Smith, Olson Smith but not Carla Smiths, Buddy Jacksmith or Michael Smithsonian

----- results -----

match: 'Owen', pos[4, 7]
match: 'Clive', pos[16, 20]
match: 'Jack', pos[29, 32]
match: 'Olson', pos[41, 45]

total results: 4

-------------------------------------------------------------------------------
example 43

task: find word 'test' + letter after word 'unit '
how:  shows how to use skip attribute

----- source -----

matchEnd find( all (skip 'unit ' 'test' letter))
ifMatch end
reset
loop

----- text -----

tests, main tests, unit testb, example tests 

----- results -----

match: 'testb', pos[24, 28]

total results: 1

-------------------------------------------------------------------------------
example 44

task: find word 'test' + letter after any other word but 'unit'
how:  shows how to user 'unless' attribute as a lookbehind

----- source -----

matchEnd find( all (unless('unit ') 'test' letter))
reset
loop

----- text -----

testa, main testb, unit tests

----- results -----

match: 'testa', pos[0, 4]
match: 'testb', pos[12, 16]

total results: 2

-------------------------------------------------------------------------------
example 45

task: find all words that are not followed by the word 'test' excluding the word 'test' itself
how:  match begin of word, then check that it is not 'test', repeat 2 to infinite letters, 
      at the end make sure that ist is not followed by one/many spaces and 'test'

----- source -----

matchEnd find( bow !isWord('test') repeat[1,@]( letter ) !(blank 'test')  )
loop

----- text -----

Find words module, unit test, main  test, object, program.

----- results -----

match: 'Find', pos[0, 3]
match: 'words', pos[5, 9]
match: 'module', pos[11, 16]
match: 'object', pos[42, 47]
match: 'program', pos[50, 56]

total results: 5

-------------------------------------------------------------------------------
example 46

task: find all words not ending with 'ed'
how:  match begin of word, repeat 1 to infinite letters but break operand as soon as 'ed' at word end found

----- source -----

matchEnd find( bow repeat[1,@]( letter break('ed' eow))  )
loop

----- text -----

Find words but not ended and blended.

----- results -----

match: 'Find', pos[0, 3]
match: 'words', pos[5, 9]
match: 'but', pos[11, 13]
match: 'not', pos[15, 17]
match: 'and', pos[25, 27]

total results: 5

-------------------------------------------------------------------------------
example 47

task: test long string (100 MB), find text at the end and measure time in ms
how:  find case insensitive 't' and T'

----- source -----

match find( ~'t' 'T' )

----- results -----

match: 'TT', pos[100000000, 100000001]

total results: 1

-------------------------------------------------------------------------------
example 48

task: find address in the form street name, postal code city in single line, using templates
how:  make template with common words for roads, find house number, road, postal code and city

----- source -----

< roadMarker = { any( ~'avenue' ~'ave.' ~'road' ~'street' ~'boulevard'  ~'drive'  ~'lane' ) } > 

match find( int[1,10000] blank repeat[1,3]( !(<roadMarker>) word ) blank <roadMarker> ) 
ifMatch { 
  matchEnd ( ',' blank int[1,@] repeat[1,3]( blank word ) at any(',' eol eot ))
} 
loop 

----- text -----

He lives in 1007 Mountain Drive, 63527 Gotham City, and is a superhero with questionable reputation. His buddy lives in 1938 Sullivan Lane, 55555 Metropolis,

----- results -----

match: '1007 Mountain Drive, 63527 Gotham City', pos[12, 49]
match: '1938 Sullivan Lane, 55555 Metropolis', pos[120, 155]

total results: 2

-------------------------------------------------------------------------------
example 49

task: find parameters of function configGetChar()
how:  find string 'configgetchar' case insensitive ~.
      If match -> include all characters not equal to ')'
      also possible in second clause:  ifMatch match every( all(!')' char) );
      here we match as long as character and not ')' is fullfilled, all() is logical and 
      since all() is the default operator, this is even shorter: ifMatch match every( (!')' char) );  

----- source -----

match skip find( ~'configgetchar(' );  ifMatch match ( repeat[1,@]( !')' char) );

----- text -----

void configSet(const char* aKey, int aKeylen);..... bool configGetChar(const char* aKey, int aKeylen, char32_t*aValue);....

----- results -----

match: 'const char* aKey, int aKeylen, char32_t*aValue', pos[71, 116]

total results: 1

-------------------------------------------------------------------------------
example 50

task: match JSON element where sub element cat is '127.0.4.1', *.bites is 0, status: 0 and url is an IPv4
how:  match chain, if branch fails skip rest and reset result, if last condition matches, end result
      break on every '{' once in an element to avoid skipping over JSON element

----- source -----

match find ('element' all(digit) ': {')
ifMatch match find( break('{') '"cat": "127.0.4.1"' ) 
ifMatch match find( break('{') '"bites": 0') 
ifMatch match find( break('{') '"status": ' 0) 
ifMatch match find( break('{') '"url": ''"' pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255] ) 
ifMatch match find( break('{') '}' ) 
ifMatch end
else reset
loop

----- text -----

{
element1: { 
"cat": "127.0.5.2",
"bites": 2,
"status": 1,
"url": "http://test.de/" 
},
element2: { 
"cat": "127.0.4.1",
"bites": 0,
"status": 0,
"url": "192.168.5.6" 
},
element3: { 
"cat": "127.0.4.2",
"bites": 0,
status: 0,
"url": "192.168.5.6" 
}
}

----- results -----

match: 'element2: { 
"cat": "127.0.4.1",
"bites": 0,
"status": 0,
"url": "192.168.5.6" 
}', pos[89, 169]

total results: 1

-------------------------------------------------------------------------------
example 51

task: match JSON element where  sub element cat is '127.0.4.1', *.bites is 0, status: 0 and url is an IPv4
how:  all in single line: match chain with single find
      break on '{' once in an element to avoid skipping over JSON element
      advance chars as long as not condition looked for with repeat[@] (!condition char)

----- source -----

match find ('element' all(digit) ': {')
ifMatch match find( break('{') '"cat": "127.0.4.1"' repeat[@](!'"bites' char) '"bites": ' 0 repeat[@](!'"status"' char) '"status": ' 0 repeat[1,@](!'"url"' char) '"url": ' '"' pure[0,255] '.' repeat[@](!'}' char) '}') 
ifMatch end; else reset
loop

----- text -----

{
element1: { 
"cat": "127.0.5.2",
"bites": 2,
"status": 1,
"url": "http://test.de/" 
},
element2: { 
"cat": "127.0.4.1",
"bites": 0,
"status": 0,
"url": "192.168.5.6" 
},
element3: { 
"cat": "127.0.4.2",
"bites": 0,
status: 0,
"url": "192.168.5.6" 
}
}

----- results -----

match: 'element2: { 
"cat": "127.0.4.1",
"bites": 0,
"status": 0,
"url": "192.168.5.6" 
}', pos[89, 169]

total results: 1

-------------------------------------------------------------------------------
</pre>
