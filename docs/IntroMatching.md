# Matching with  Scripal

## source match
The Scripal language is made up of operators and their associated operands which will be processed one by one, further controlled by logic flow. 
Internally a position pointer in the text to be scanned is moved, only forward, never back.  
Key element is the 'match' operator which performs a comparison of text and operands to determine whether a portion of it matches. It may produce a result (portion of the text scanned and/or a position) and always has a logical outcome:
'match' or 'no match'.<br>
Other operators specify, if the match actually produces a result string in case of a match or is only used to influence program flow (much like assertions in regular expressions).<br>
Results build up over matches and operators 'end' and 'endlast' then quit and store the current result.
Operator 'reset' quits a result sequence without storing it, in case not all conditions are fulfilled.<br>
Code may be processed many times when followed by a 'loop' statement to repeatedly search through a text.
Conditions like 'ifMatch' control whether an operator is processed at all or skipped, a method known from programming languages. 

## Scripal syntax 
  
The syntax is made up of basic operators and it's operands which are processed sequentially: left to right, top down. Operators are separated by newline or a semicolon:
They may be grouped in logical blocks by using {}, also a construct known from high-level languages.<br> 

Operators are succeeded by operands. There are logical, block, text and number operands.
Operators end with a **;** or at the end of a line.
  <pre>match find( any('t' 'T') ); ifNoMatch endlast; loop;</pre>
 
   is equivalent to

  <pre>match find( any('t' 'T') ) 
ifNoMatch endlast
loop</pre>

The key operator for matching text is **match**.
Matches produce either a result region with start and end, or just a virtual position. 

<pre>match all (bow);</pre>
produces a virtual result [pos, nPos] at the beginning of a word, but no result region, just a marker at the word beginning. **nPos** represents the NULL position. 
**bow** has no physical length, it is not the first letter but the position of this letter.
  
<pre> match all ('house'); </pre> 
produces a result region over the entire word 'house'.
 The result region may be expanded, it is left open for further matches. 
<pre> match all ('summer' blank 'house');  </pre>
produces a result region over the entire text 'summer house'.

The first match produces 'summer', it is expanded with blank and ends with 'house'. A result region is finalized by the operator **end**. Operator **endLast** also closes the region before the last match. **reset** will throw away a result region, no result is stored. 
The next match will then start a new result.<br> 
If a scripal process ends with a result region left open, this region is stored at the end 
of the entire process.<br>
The operator **moveon count** will move the position pointer count characters ahead. There are more operators, have a look further down in this document.

Logical operands have parameters enclosed in **(...)** and may be nested.

<pre>match all('summer' blank 'house');</pre>
will only match if all three sub operands match. **all()** is related to logical AND.
  
**all()** is the default operand if no operand is given, therefore
  
<pre>match all('summer' blank 'house'); <=> match ('summer' blank 'house');</pre> 
and  
<pre>match any('dog' 'cat' 'bird');</pre>  
will match if any one of the three sub operands match. **any()** is related to logical OR.
  
Operator **match** matches text against a string or file at position 0, the current position. To find a string within text use the logical **find**.

<pre>match find (any('dog' 'cat' 'bird'));</pre> 
will scan the text to a position where the match fits, ie. it will look for the first 'dog', 'cat' or 'bird' found.
  
To find all occurrences in the text and store them separately use **end** and **loop**.

<pre>match find (any('dog' 'cat' 'bird')); ifmatch end; loop;</pre>
will scan the text and find all occurences of any of the following: 'dog', 'cat' or 'bird'
  
Logical operands may be nested even further:  
<pre>match any( all('my' blank 'dog' )  all ('my' blank 'cat'))</pre>
will match 'my dog' or 'my cat'

block operators are for example:<br>
**bot**  - match at begin of text<br>
**bow**  - match at begin of word<br>
**eow**  - match at end of word<br>
**char** - match any character<br>
**char[65, 87]** - match an character in UNICODE range 65..87<br>
**letter** - match any letter<br>
**digit** - match any numerical digit<br>
...<br>
..

<pre>match find ( bow 'c' );</pre>  
will advance to the next word starting with the letter 'c'

<pre>match find ( bow 'c'); match find (eow); end;</pre>  
will match the entire next word starting with the letter 'c'<br><br>
or easier:
<pre>match find ( bow 'c' find (eow) ); end;</pre>  

and equivalent, find first word starting with 'c':

<pre>match find ( bow 'c' find (eow) )); end;</pre>  
 
and equivalent, find word starting with 'c' and all characters but not end of word , i.e. not last character in word. Then add last character of word:
     
<pre>match find ( bow 'c' repeat (char !eow) char); end;</pre>
    
text will match case sensitive
<pre>match ('dog')</pre> 
will only match 'dog', not 'DOG' 
  
<pre>match (~'dog')</pre> 
will match 'dog' or 'DOG' or 'Dog'

numbers may be matched easily

<pre>match (14)</pre>  
will match number 14 or 14.0

<pre>match ([0, 10])</pre>
will match any number between 0 and 10
  
<pre>match ([0, @])</pre>  
will match any number between 0 and infinty, ie. any number >= 0

for more information see [Scripal language](/docs/Language.md) 

## nearest match

The nearest match mechanism uses algorithms to find similar words or phrases in a text.
Example: 
<pre>
scripal "test" "Find words like tests in here." -f 0.8
</pre>
will find the word 'test' with a distance of at least 0.8 to 'test'. 0 represents no similarity at all, 1 equality.



## block match

The block match mechanism uses algorithms to compare, how similar two text blocks are.
A result is in the range [0,1], where 0 represents no similarity at all, 1 equality.

<pre>
scripal "This is block 1 to test." "This is block 2 to test here." -g 0.9
</pre>
will compare two blocks and show the result if at least as big as -g parameter.
'-g 0' will print result in any case.

## results
See section **results** in [Scripal language](/docs/Language.md#results) 

## configuration
Many functions as well as internationalization depend on configuraton settings.<br> See section **configuration** in [Scripal language](/docs/Language.md#configuration) 
