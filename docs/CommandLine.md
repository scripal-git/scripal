# Using Scripal from the command line

## Installation

[prerequisites](/docs/InstallPrerequisites.md) <br>

## clean build -> else skip to ready to use packages

[build Scripal under Linux ](/docs/BuildLinux.md) <br>
[build Scripal under Windows ](/docs/BuildWindows.md) <br>

## ready to use packages  

[install packages](/docs/InstallPackage.md) <br>

## running Scripal

call Scripal with

<pre>
scripal [options] [source] [text]
</pre>

where options are:
<pre>
-?                           : show usage and end
-a path (--sourcefile=path)  : path to file holding source, specify encoding with utf8//path 
-b path (--textfile=path)    : path to file holding text, specify encoding with utf8//path
-c path (--config=path)      : use alternative config file, create if missing
                               'reset' to create/reset default config file
-d path (--outfile=path)     : path to output file
-e (--example)               : run built-in examples
-f dist (--nearest=dist)     : no source-> use nearest algorithm to find text
                               store results with distance >= dist, dist = 0..1, 1 = equal
-g dist (--block=dist)       : no source-> block comparison
                               match if distance >= dist, 0 means not at all, 1 fully equal
-k (--convert)               : convert file -a (specify encoding in path enc//..) to
                               file -b enc//..
-m (--main)                  : run main test (for developers)
-n (--single)                : run single test (for developers)
-o (--recursive)             : scan all files in subdirectories, use with -p
-p path (--path=path)        : scan all files in path
-q ext (--extension=ext)     : scan all files with given extension, use with -p
-r string (--replace=string) : replace occurrences with given string
                               if in file, '_' is appended to output filename, filename_
-s (--split)                 : split file
-t (--time)                  : show execution time
-u (--csv)                   : results as CSV, else human readable format
-v (--json)                  : result as JSON, else human readable format
-w (--verbose)               : results in verbose readable format
                               ([file], result, tag, pos start, pos end)
-x (--debugcomp)             : debug compilation
-y (--debugrun)              : debug run
-z (--showcode)              : show compiled code
</pre>

## escape arguments

depending on your operating system, remember to properly escape arguments

escape ' in linux (bash)
<pre>
./scripal 'match find( '\''file'\''); end; loop;' -p '../test/' 
./scripal 'match find( '\!''\'' 'file'); end; loop;' -p '../test/' 
</pre>

## examples

find the term 'bee' in given string
<pre>
scripal "match find( all ('bee'))"   "i can see a honey bee"
</pre>

find the term 'bee' in given string, show compilation result, compiled code and run parser in debug  mode
<pre>
scripal --debugcomp --debugrun --showcode "match find( all ('bee'))"   "i can see a honey bee"
</pre>

file match where Scripal source is in source.txt and text to match in text.txt
<pre>
scripal -a source.txt -b text.txt
</pre>

file match where Scripal source is in source.txt and text to match in text.txt, output in resultOut.txt
<pre>
scripal -a source.txt -b text.txt -d resultOut.txt
</pre>

nearest match, compare term "test" with text in nearestTxt.txt, distance >= 0.9
<pre>
scripal "test" -b nearestTxt.txt -f 0.9 
</pre>

nearest match, compare text in nearestSrc with text in nearestTxt.txt, distance >= 0.9
<pre>
scripal -a nearestSrc.txt -b nearestTxt.txt -f 0.9
</pre>

block match for the word "test" and "test1" where distance >= 0.9
<pre>
scripal "test" "test1" -g 0.9
</pre>

block match for text held in blockSrc.txt with blockTxt.txt, where distance >= 0.9
<pre>
scripal -a blockSrc.txt -b blockTxt.txt -g 0.9
</pre>

file search for the word "case" in all files in directory test (directory in UNIX notation)
<pre>
scripal "match find('case'); end; loop;" -p "./test/" -o
</pre>

file search for the word "case" in all files in directory test (directory in UNIX notation) . file ending = ."txt"
<pre>
scripal "match find('case'); end; loop;" -p "./test/" -q "txt" -o
</pre>

search for files holding term "case" using nearest match
<pre>
scripal "case" -p "./test/" -q "txt" -o -f 0.9
</pre>

file search for the file most similar to term "test" using block match
<pre>
../scripal "test" -p "./test/" -q "txt" -o -g 0.9
</pre>

replace the word "dog" (all occurences) in text with "cat"
<pre>
scripal "match find('dog'); end; loop;" "look for dog, my dog should be replaced with another animal, it's a dog's life" -r "cat"
</pre>

replace the word "dog" (all occurences) in file dog.txt with "cat"
<pre>
../scripal "match find('dog'); end; loop;" -b dog.txt -r "cat"
</pre>

split  string at spaces
<pre>
scripal "match find( blank ); ifMatch end; loop" "    test1 test2    test3   " -s
</pre>

split file testSplit.txt at spaces and store in testSplitRes.txt 
<pre>
scripal "match find( blank ); ifMatch end; loop" -b testSplit.txt -s --outfile=testSplitRes.txt
</pre>

convert file to utf16l, latin1, win1250 , set encoding before file name with /
<pre>
scripal -k -a utf8//text.txt -b utf16l/text_utf16l.txt
scripal -k -a utf8//text.txt -b latin1/text_latin1.txt
scripal -k -a utf8//text.txt -b win1250/text_winb1250.txt
</pre>

## encodings

default<br>
utf8<br>
utf16l<br>
utf16b<br>
utf32l<br>
utf32b<br>
ascii<br>
cp932<br>
cp936<br>
cp949<br>
cp950<br>
latin1<br>
latin2<br>
latin9<br>
win847<br>
win1250<br>
win1251<br>
win1252<br>
win1253<br>
win1254<br>
win1255<br>
win1256<br>
win1257<br>
win1258<br>
