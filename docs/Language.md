# Scripal language

## terminology 

* **source:** Scripal instructions, the pattern to use

* **code:** compiled Scripal instructions

* **text:** text to parse

* **match:** some condition is fulfilled, an operand matches portions of the text<br> 
a match doesn't always produce a result, it's a logical condition  

* **result:** result string and/or result positions, results are stored in an array, as CSV or JSON and returned at the end of the match process<br>
a result may only occur in case of a match

* **result region:** a result string, part of the text, with a begin and end

* **result positions:** relating to results in the original text, a byte position corresponding to the result region or a virtual position (one side may be nPos)

* **nPos:** nPos denotes an invalid position or no position  

* **tags:** relating to the result, a tag describes the result 

* **names:** result strings may be given a name for further use

* **template:** a template holds part of the source to be used repeatedly



**All operations are of the form** 
<pre>
  |condition| operator |attributes| |operand| |(...)|  
</pre>

where |element| denotes, the element is optional or element1|element2 means, one of many can be chosen  

* conditions are keywords controlling the program flow

* operators are executed one by one, controlling the type of operation (examples: match, loop etc.)

* an operand may be logical, a block operand, value(s), text or a name

* attributes can be used to further specify the behavior of operands

* controls may be found anywhere and separate operators or operands, denote operand types etc. and are often
  special characters like ';' '"' '[' and others 


## operators

- <b>match</b> opd                      : match operand(s) with text, leaving result open for more matches
  <pre>example: match 'dog'; </pre>
  will match 'dog' at the current position in text

- <b>matchEnd</b> opd                      : match operand(s) with text and store result
  <pre>example: matchEnd 'dog'; </pre>
  will match 'dog' at the current position in text and finalize result

- <b>reset</b>                          : reset (throw away) result, next match will start new result 

- <b>end</b>                            : end result region at end of last match, store result text and/or result position, if region is open at end of text, it is stored as well, no end necessary 

- <b>endlast</b>                        : end result region before start of last match, if region is open at end of text, it is stored as well, no end necessary 

- <b>replace</b>                        : end result region at end of last match, replace last result/position with new one

- <b>name</b> text                      : result is only used as a name specified after operand, not stored. The text denoted by name may be used for further matches.
  <pre>example: name #code#; match find ( #code# );</pre> 
  will store the current result as label 'code' (much like a variable) and can be used for further matches

- <b>nameAdd</b> text                   : result is stored and also used as a name specified after operand

- <b>tag</b> text                       : tag result, results have tags with the same index to explain the result in detail
  <pre>example: matchEnd 'test'; tag 'test tag' ;</pre> 
  will store the current result under tag 'test tag' to denote the result text and position 

- <b>setMatch</b>                       : set last operation as matched without true result for condition logic

- <b>setNomatch</b>                     : set last operation as not matched for condition logic

- <b>loop</b>                           : repeat search in block if combined with block end {} or from the beginning of the code if not preceded by curly bracket<br>if end of text is reached, loop will break 
  <pre>example: match find('test'); loop;</pre> 
  or
  <pre>match 'pre'; ifMatch {match find('test'); loop;}</pre> 

- <b>total</b> number                   : try to match the preceding block {} number of times, if number is not given, 1 is default<br>
if no or less matches occurred -> reset, else the whole block is considered as a match
  <pre>example: { match find( all(letter char)); end; } total 2</pre> 
  must match at least 2 times
- <b>moveon</b> number                  : move position pointer in text by number of code points, if number is omitted -> move ahead one code point<br> does not influence match logic 
                                          
- <b>exit</b>                           : quit entire process

## operator conditions

- <b>ifMatch</b>                        : if last operation was a match, perform the following operation(s), either single operation or entire block in {} 

- <b>ifNomatch</b>                      : if last operation wasn't a match, perform the following operation(s), either single operation or entire block in {}

- <b>ifMatched</b>                      : if any match has occurred so far, perform the following operation(s), either single operation or entire block in {}

- <b>ifNoMatched</b>                    : if no match has occurred so far, perform the following operation(s), either single operation or entire block in {}

- <b>else</b>                           : alternative branch either of block or previous operator

## operands 

  - a logical operand has sub operands and is of the form 
   |attribute| logic |value|[value range]| ( operand1 operand2 ...)

  - a block operand is of the form
   |attribute| block |value|[value range]|

  - a number is of the form
   |attribute| number or |attribute| [number range]
   
  - text is of the form<br>
   |attribute| 'text'  
   |attribute| "text"  
   |attribute| \`text\`

  - a name is of the form
   |attribute| #name#  

value or number ranges are embedded in [x, x] where @ = infinity, example:  [0, @]
 
many operands push the position pointer further

 
## text operand 

'<b>text</b>', "<b>text</b>", <b>\`text\`</b>: match text<br>
<pre>example: match ('test')</pre>
match the word 'test'<br><br>
So any of the three delimiters '', "", `` may be used.
If you have text with double quotes, embed in quotes.<br>
example: ' it is no "surprise" '<br>

special attributes: ~

##  name operand 

\#<b>name</b>\# : match text held in given name

<pre>example: match (#ph#)</pre>
match the word held in placeholder called 'ph'<br><br>
special attributes: ~

## number or number range 

attribute <b>number</b> or

attribute <b>[number min,number max]</b>

<pre>example: match [3,5]</pre>
match all numbers >= 3 and <= 5   
The number should stand isolated like a word, as in '100 times' or 'I was born in 1989.'
To find numbers in phrases use the pure attribute. Example '0xff6a' or 'hgf-300-GHT' etc.
Here the embedded numbers may be found by using the pure attribute.
Decimal point in Scripal is always '.', example: 0.355. A decimal point in text to scan is set in the config, may be locale specific.    
special attributes: hex, oct, bin, pure

## logical operand

logical operands don't set results themselves but combine or test other operands 

- <b>all</b>(opd1 opd2..)                : match all operands in a consecutive order (row), from current position on (logical 'and') 
default operand!, if no operand is given only a term (xx ), the 'all' operand is assumed as default 
  <pre>example: all('my' blank 'friend' blank 'Greg')</pre>
  will match only if all terms in this order are given 'my friend Greg' 

- <b>any</b>(opd1 opd2..)                : must match any one of the operands (logical 'or')  
  <pre>example: any('cat' 'dog' 'hamster' 'tyrannosaurus rex')</pre> 
  will match if any one of the terms occur

- <b>each</b>(opd1 opd2..)               : match (as long as operand matches in consecutive order -> continue...) 
  <pre>example: each('my' blank 'cat' blank 'is' blank 'hungry')</pre> 
  will match if 'my', 'my cat', also 'my cat is' etc. is found.
  'each' matches as long as possible in the order given

- <b>every</b>(opd1 opd2..)              : find all text/logical in any order , set position to last found (if found)
  <pre>example: every('dog' 'cat' 'hamster')</pre> 
  will match 'dog', also 'cat' and 'dog', also 'dog' and 'cat', the order is not relevant, the 'every' operand matches as long as possible in any order

- <b>find</b>(opd1 opd2..)               : continue to parse until all operands match, move position pointer to first match 
  <pre>example: find('dog')</pre>
  will move the position pointer until 'dog' is found

- <b>findAt</b>[min|,max|] (opd1 opd2..) : continue to parse until all operands match starting min characters from current position on, up to max characters from current position, move position pointer to first match 
  <pre>example: findAt[1,5] ('dog')</pre> 
  will move the position pointer until 'dog' is found, only if it the term is found 1 up to 5 characters from the current position on
                                 
- <b>break</b> (opd1 opd2..)            : break parent operand if all suboperands match
  <pre>example: find( break('cat') 'dog')</pre> 
  will look for the term 'dog' , but if 'cat' is found, the find operation will be aborted

- <b>repeat</b>[min,max] (opd1 opd2..) : operation must match all sub operands minimum min, maximum max times, use '@' to denote infinite , if no range is given -> repeat as long as all sub operands match       
  <pre>example: repeat[1,3] (space)</pre>
  will succeed if at least one and up to three spaces haven been found

- pure|int|bin|oct|hex <b>isNumber</b>|[min,max]|(opd1)    : does opd1 correspond to a  number?<br>if [min,max] is specified in given range? 
  <pre>example: int isNumber()</pre> 
  will succeed if the entire match is an integer number<br>
  see attribute description further down 

- <b>isWord</b>(opd1)                    : does opd1 correspond to word
  <pre>example: isWord('grape' any(eow 's' 'fruit'))</pre> 
  will succeed if the sole words 'grape' (eow), 'grapes' or 'grapefruit' have been found<br>
  without eow the word 'grape' would not be found

- <b>isUpper</b>(opd1)                   : is result in opd1 all upper case?
  <pre>example: isUpper('GRAPE')</pre> 
  will succeed if the sole word 'GRAPE' is all in upper case

- <b>isLower</b>(opd1)                   : is result in opd1 all lower case?
  <pre>example: isLower('grape')</pre> 
  will succeed if the sole word 'grape' is all in lower case

## block operand

block operands set matches and results 

- <b>space</b> : match if text is single space

- <b>blank</b> : match all contiguous white space in text 

- <b>char</b>\[begin[, end]] : match any code point<br> 
                                           if single single value or range is specified -> match Unicode value or range   
  <pre>example: match char</pre>
  match any single character 
  <pre>example: match (char[65])</pre>
  match character 'A'
  <pre>example: match (char[65,67])</pre>
  match character 'A', 'B' or 'C'

- <b>letter</b> : match if text is a letter, that is a character which is part of an alphabet

- <b>digit</b> : match if text is a digit, recognizes digits in most languages, not just 0,1,2..., see **config.translateDigits**  

- <b>word</b> : match if text is any word

- <b>bos</b> : match beginning of sentence, first non-space character in a sentence<br>
BOM does not count as sentence begin<br>
sentences start at the beginning of text and after EOS markers<br>                                       (only returns position [pos, nPos)]
                                          
- <b>eos</b> : match end of sentence sequence, that is EOF markers like '.'
                                          
- <b>bol</b> : match beginning of line (includes beginning of text, if first character)<br>                                       might be BOM if present, the BOM marks the beginning of the first line<br>
(only returns position [pos,nPos])                                    

- <b>eol</b> : match all consecutive end-of-line control characters like linefeed

- <b>bot</b> : match begin of text, first byte in text (might be start of BOM if present)<br>
(only returns position [pos,nPos])

- <b>eot</b> : match end of text, position after last byte in text<br>
(only returns position [nPos,pos])

- <b>bow</b> : match beginning of word<br>
(only returns position [pos,nPos]) 

- <b>eow</b> : match end of word (that is character after word)
(only returns position [nPos,pos]) 

- <b>bomark</b> : match byte order mark  

- <b>move</b>[count] : move position pointer count characters ahead<br> 
expand result region 
                                           
## operand attributes

attributes further specify the behaviour of operands 

- <b>~</b>                               : ignore case<br>(use for text matching only)
  <pre>example: match (~'dog')</pre> 
  will match 'dog' case insensitive, 'Dog' will match as well  

- <b>!</b>                               : logical not, match for exclusion (mismatch)
  <pre>example: match (!'dog')</pre> 
  will match any text but 'dog'

- <b>int</b>                             : match integer number, clearly distinguishable as an integer in the text. That is not embedded in true letters, or followed by decimal point unless end of sentence.<br>
(use for number matching only)
  <pre>example: match (int 145)</pre> 
  will match the number 145, not 145.13 or -145'

- <b>pure</b>                            : match pure natural number anywhere embedded in phrases or any characters<br>
  <pre>example: match (pure 145)</pre> 
  will match 'a145b' or '.145.' at the end of a sentence

- <b>hex</b>                             : use hexadecimal format<br>
(use for number matching only)
  <pre>example: match (hex a015f)</pre> 

- <b>oct</b>                             : use octal format<br>
(use for number matching only)
  <pre>example: match (oct 0157)</pre> 

- <b>bin</b>                             : use binary format<br>
(use for number matching only)
  <pre>example: match (bin 01001)</pre> 

- <b>at</b>                              : conditional match, set position pointer to begin of match but produce no result  
  <pre>example: match (find (at 'test'))</pre>
  will find the word 'test' and set match condition to true, but no result<br> 
  the position pointer is at the first 't' of 'test'

- <b>skip</b>                            : conditional match, set position pointer to end of match but produce no result
  <pre>example: match (find (skip 'test'))</pre> 
  will find the word 'test' and set match condition to true, but no result<br> 
  the position pointer is after the last 't' of 'test'

- <b>test</b>                            : conditional match, don't set position pointer and produce no result
  <pre>example: match (find (test 'is')); ifMatch match find ('contained')</pre>
  find the word 'contained' only if anywhere in the text the word 'is' was found, only match condition is set to true

- <b>try</b>                             : if match then include, but operand matches in any case, that is the match condition is true
  <pre>example: match (try 'dog')</pre> 
  will match 'dog', but will also match if 'dog' ist not found  
  try will include text but only if possible, in any case the condition is fulfilled

- <b>last</b>                            : set the last condition in a repetitive operand, the last loop must fulfill the condition
  <pre>example: match repeat[2,@]( last ~'S' letter )</pre> 
  will match all contiguous letters, the last letter must be 's' or 'S'

- <b>notlast</b>                         : set the last condition in a repetitive operand, the last loop must not fulfill the condition
  <pre>example: match repeat[2,@]( notlast ~'S' letter )</pre> 
  will match all contiguous letters, the last letter cannot be 's' or 'S'

- <b>unless</b>                          : set condition previous to the current match
  <pre>example: match ( unless 'honey ' 'bee' )</pre> 
  will match the word 'bee' if not preceded by 'honey '
 
## controls 

controls have various functions, grouping operator blocks, ending lines etc.

- <b>{}</b>                              : operator block

- <b>\n \r \n\r</b>                      : line end and operator end

- <b>;</b>                               : operator end
  <pre>example: match ( 'test' );</pre>

- <b>//</b>                              : comment up to end of line
  <pre>example: match ( 'test' ) // remark here<br>// final remark</pre>

## results  

Scripal is UTF-8 based, all internals use this encoding. Results are byte positions! not character positions, since they are quite useless for further processing in many programming languages.
Results regions may be open!, that is only one side is a byte position, denotig every character from here [byte pos, nPos] or up to here [nPos, byte pos]. nPos represents the NULL position.
**Result positions are related to the position in the UTF-8 representation of the text.** <br>
If the character encoding is different, positions cost time to be calculated, they are set to NaN (not a number). Set **config.positionType** to  POS_OFFSET for byte positions in the native encoding or POS_COUNT for character count (0...).
These last two settings produce extra proccesing time, use if positions are needed.
nPos and is denoted as -1 in JSON results, NaN as -2. In other result types , the words are written out.

Example: position [2,7] means the result expands over bytes 2 up to 7, so 7 would by the last byte of the last code point of the result region.<br>
Virtual positions are open ranges: 
For example: In case of operand 'bow' (begin of word), only the byte position is returned with an end, no region is involved:  [8,nPos] would indicate, the word starts at byte position 8.<br>
Results are held in arrays or may be obtained in JSON or CSV format. 

## templates 

<b>\<name = template\></b>                       : denote template

Templates are used for repetitive expressions, like macros. By specifying the name of the  template you use a certain code fragment several times.
A change in the template causes a change in all instances.

<pre>< roadMarker = { any( ~'avenue' ~'ave.' ~'road' ~'street' ~'boulevard'  ~'drive'  ~'lane' ) } >  
match find( int[1,10000] blank repeat[1,3]( !( < roadMarker > ) word ) blank < roadMarker > ) 
ifMatch { 
  matchEnd ( ',' blank int[1,@] repeat[1,3]( blank word ) at any(',' eol eot ))
} 
</pre>
defines **roadMarker** as a template to match any road type<br>
the entire expression will match an address like 
**1007 Mountain Drive, 63527 Gotham City**  
  
define a template in the source by using  
<pre>< name = { code } ></pre>

templates may have arguments 
<pre> <1> <2> ..</pre>
<pre>example: < person  = { match find ( <1> space <2>) } > </pre>
which will be substituted by caller arguments in: <br>
<pre>< person {'mike'}{'myers'} ></pre>

<1> and <2> in template person will be substitued with **'mike'** and **'myers'**
so template **person** will match for first name and surname 

## configuration

The configuraton data can be set by special methods. The configuration is thread bound, all Scripal objects in a thread share the same configuration.
Configuration data may be specified at the beginning of source in %xxxxx%, and this configuration is only used for the given object.<br> 
<pre>example: % = { "showCode" : true, "posSign" : "+"  } </pre>

If a config file exists at the default location, it is used implicitly. Create by calling 
<pre>scripal -c reset</pre>

default path is:<br>
**Linux** : ~/.config/scripal/scripal.cnf<br>
**MS Windows** : .\scripal.cnf<br>

config values:

- debugCompile                           : boolean, compile source with debug option (default: false)

- debugRun                               : boolean, run code with debug option (default: false)

- showCode                               : boolean, show compiled code (default: false)

- measureTime                            : boolean, if true measure process time in milliseconds (default: false)

- useEmpty                               : boolean, also store empty results in replace/split operations (default: false)

- translateDigits                        : boolean, set true to convert digits in current language to 0,1 .. slight performance penalty (default: 
false)

- verboseResult                          : boolean, set true for very explicit results (default: false)

- decimalPoint                           : character, decimal point in current language (default: '.')

- thousandsSep                           : character, thousands separator in current language (default: ',')

- posSign                                : character, + sign in current language (default: '+')

- negSig                                 : character, - sign in current language (default: '-')

- encoding                               : integer, best fit encoding used in environment (default: ENC_DEFAULT to guess best encoding), see paragraph **encodings** further down 

- logEncoding                            : integer, encoding used in logging (file or console) (default: ENC_DEFAULT to guess best encoding), see paragraph **encodings** further down 

- maxFileSize                            : integer, maximum size of file to load in MB used in file matching, prevent out-of-memory errors (default: 1000) 

- patternNearest                         : integer, pattern used for nearest match (default: PATTERN_LEVENPLUS_WORD)<br>
PATTERN_LEVEN_WORD = 1 : Levenshtein distance, find similar word<br>
PATTERN_LEVENPLUS_WORD = 2: Levenshtein distance, optimized for word match<br>
PATTERN_LEVEN = 3 : Levenshtein distance, find similar phrase<br>
PATTERN_JARO = 100 : Jaro distance, find similar phrase<br>
PATTERN_JARO_WINKLER = 101 : Jaro Winkler distance, find similar phrase<br>
PATTERN_JARO_WINKLER_WORD = 102 : Jaro Winkler distance, find similar word<br>

- patternBlock                           : integer, pattern used for block match (default: PATTERN_JARO)<br>
PATTERN_JARO = 100 : Jaro distance, compare block using Jaro distance<br>
PATTERN_JARO_WINKLER = 101 : Jaro Winkler distance,  compare block using Jaro-Winkler distance<br>
PATTERN_JARO_WINKLER_WORD = 102 : Jaro Winkler distance, compare block using Jaro-Winkler word based distance<br>

- positionType = type of result positions, (default : POS_UTF8)<br>
POS_UTF8 = 1: result positions relate to UTF-8 or are NaN if other encoding is used (fast)<br>
POS_OFFSET = 2: result positions relate to native encoding of text and is byte offset<br>
POS_COUNT = 3:  result position relates to character count (0,1..)<br>

- logChannel                             : string, log channel to use, 'default', 'stdout', 'buffer' or a filename (default : 'default')
                                           default setting: log to stdout in Scripal binary, don't log in Scripal library   

- pdfReader                              : string, path to binary pdftotext (see install document)
(default : 'pdftotext')

- frmReader                              : string, path to binary pandoc (see install document)(default : 'pandoc')

- appPath                                : string, path to application data (templates)(default : './' under Windows, '~/.config/scripal under Linux)

- sentenceEnd                            : array, sentence end signs (default: '.', '!', '?')

- separators                             : array, word separators (default: ' ', '.', '!', '?', ',', ';', ':', '/', '(', ')', '[', ']', '{', '}')

- abbreviations;                          : array, abbreviations to distinguish word and end of sentence (default: empty set)

##  encodings

Scripal uses UTF-8 internally, so matching a UTF-8 string or file is the fastest operation. 
It does support various other encodings: UTF-16BE, UTF-16LE, UTF-32BE, UTF-32LE, Latin and Windows codepages. 

The internal encodings are:

* ENC_DEFAULT  = 1   , use encoding specified in config.encoding, be careful: the encoding of a console is sometimes not specified, we recommend using Windows Terminal and PowerShell with UTF-8 encoding  
* ENC_UTF8     = 2   , UTF-8
* ENC_UTF16L   = 3   , UTF-16LE
* ENC_UTF16B   = 4   , UTF-16BE
* ENC_UTF32L   = 5   , UTF-32LE
* ENC_UTF32B   = 6   , UTF-32BE
* ENC_ASCII    = 10  , ASCII 
* ENC_CP932    = 11  , CP932 DBCS, Japanese characters
* ENC_CP936    = 12  , CP93 6DBCS, simple Chinese characters
* ENC_CP949    = 13  , CP949 DBCS, Korean characters
* ENC_CP950    = 14  , CP505 DBCS, Chinese Big5 characters
* ENC_LATIN1   = 30  , West Europe Latin-1, ISO 8859-15
* ENC_LATIN2   = 31  , Middle Europe Latin-2, ISO 8859-15
* ENC_LATIN9   = 32  , West Europe Latin-9, ISO 8859-15
* ENC_WIN874   = 50  , Windows Codepage 874, Thai characters
* ENC_WIN1250  = 51  , Windows Codepage 1251, Middle Europe
* ENC_WIN1251  = 52  , Windows Codepage 1251, Cyrillic
* ENC_WIN1252  = 53  , Windows Codepage 1252, West Europe
* ENC_WIN1253  = 54  , Windows Codepage 1253, Greek
* ENC_WIN1254  = 55  , Windows Codepage 1254, Turkish
* ENC_WIN1255  = 56  , Windows Codepage 1255, Hebrew
* ENC_WIN1256  = 57  , Windows Codepage 1256, Arabic
* ENC_WIN1257  = 58  , Windows Codepage 1257, Baltic
* ENC_WIN1258  = 59  , Windows Codepage 1258, Vietnamese

