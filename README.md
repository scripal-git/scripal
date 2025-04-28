
<img align="right" width="150" src="./docs/internal/Logo.png">
<br><br>

# $${\color{Teal}Scripal \space text \space processor}$$ 

Scripal is a multipurpose tool which can be used for matching and manipulating text, testing for similarity, or finding patterns in strings and files as well as finding, comparing or converting files.
It is written in **C++** for best performance and may be used as a console application or library. It is also availabe for **C**, **Python**, **C#**, **Java** and **NodeJs**.

Scripal is meant to be an alternative to regular expressions but offers more functionality, like finding similar words based on edit distances and file operations, a multi-purpose text processor.<br> 
It offers a syntax which is easily readable and resembles more the notation known from programming languages, including constructs like if conditions, operator blocks and loops.<br>
With it's built-in debugging functionality the internals are easy to understand and flaws in your own code may be found quickly.<br>
Scripal is for those, who struggle with RegEx's compressed syntax and prefer more readability and intermediate control.<br>
It gives the user hands-on control over linear execution speed ( O(n) ). Simple text searches are comparable to fast string searches in programming languages.  

**cases:**
 
match number ranges:<br>**match [1,200]**  

match characters classes:<br>**word, letter, blank, begin-of-sentence, begin-of-word etc.**

logical operands:<br>**any ('city' 'town')** or **every ('dog' 'cat')** 
<br>and combine logic:<br> **all ( any('street' 'avenue')   any(city' 'town') )** 

control flow by:<br>**skipping text, repeating searches in loops, moving to a definite position etc.**

use templates for repeating patterns:<br>
**roadMarker = { any( ~'avenue' ~'ave.' ~'road' ~'street' ~'boulevard'  ~'drive'  ~'lane' ) }**<br>
**match &roadMarker&**  

### Scripal's advantages are: 

* **readable:** the pattern (source code) is intuitive, more like a programming language (if.., loops) and allows comments.<br><br>
* **fast:** Scripal is very fast for simple searches, which may then be followed by more complex match operations. Strictly O(n), no bitter surprises with large texts.<br><br>
* **versatile:** works on strings and files in combination with many programming languages, also works as a standalone tool.<br><br>
* **smart:** Scripal can interpret numbers (hex, octal...), number ranges and other lexical elements, no hassling with single digits as in RegEx.<br><br>
* **functional:** match results may be held in variables (called names) which can be used in further matches and templates work like functions.<br><br> 
* **similarity:** finds similar words and text (example: nearest distance comparison) and evaluates how different two text blocks are.<br><br> 
* **comprehendible:** produces debug information during compile- and run time.<br><br>
* **internationalization:** Scripal takes internationalization seriously and supports most natural languages. (digits in other languages)<br><br>
* **deterministic:** Will produce the same replicable result in any programming language on any OS and fully supports UTF-8, -16, -32, Windows code pages and most other popular character encodings.<br><br>
* **more intuitive and precise than RegEx** ex.: match number in the range 380-1400<br>
 RegEx: &nbsp; **\b(3[8-9][0-9]|[4-9][0-9]{2}|1[0-3][0-9]{2}|1400)\b**<br>
 Scripal: &nbsp; **match find (int[380,1400]); ifMatch end; loop**

###  how it works  

Scripal is a library which offers objects that will compile Scripal source instructions to byte code. 
As a standalone program it can be fed with various parameters and it's output is CSV style or JSON.<br> 
The pattern, here called source code, is compiled and in case the syntax isn't plausible, the compilation  quits with an error.<br> 
For nearest (distance) matches, the source is pure text to be matched. 
A text or file may be compared wth text or files, files can be found according to patterns.

###  mechanism

source match
<pre>
Scripal source (pattern) -> compile -> code -> match text -> result strings, positions and tags  
</pre>

nearest and block match
<pre>
source and method -> match text -> result strings and positions  
</pre>


**example:**

**find 'pass' followed by any two characters and a further letter, example: 'pass12x'**
<pre>
match find( 'pass' )
ifMatch { 
  move 2
  match (letter)
  end
}
</pre>

**match combinations of 'apple' and words**
<pre>
match ( 'apple ' any ('pie' 'tree' 'cake') )
</pre>

**match IPv4**
<pre>
match ( pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255] )
</pre>

##  how matching works

[intro](/docs/IntroMatching.md)

##  further examples

[examples](/docs/Examples.md)

##  getting started with the tool

[get started on the command line](/docs/CommandLine.md) <br>

##  getting started with C++

[get started in C++](/docs/StartingInCPP.md) <br>

##  getting started with C

[get started in C](/docs/StartingInC.md) <br>

##  getting started with Python3

[get started in Python3](/docs/StartingInPython.md) <br>

##  getting started with Java

[get started in Java](/docs/StartingInJava.md) <br>

##  getting started with JavaScript

[get started in JavaScript](/docs/StartingInJavaScript.md) <br>

##  getting started with C#

[get started in C#](/docs/StartingInC-Sharp.md) <br>

## License

### The Boost Software License 1.0

Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:

The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 

## Notices

Scripal uses Poco C++ libraries, special thanks to a great team!
see: [https://pocoproject.org/](https://pocoproject.org/) or [https://github.com/pocoproject/poco](https://pocoproject.org/)

## Contact

For error reports or any feature requests, please send notices to scripal@ksum.de.
Only Emails written in English will be considered!  
