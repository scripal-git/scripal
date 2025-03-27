#!/bin/bash
echo scripal unit test



echo
echo ____ test file match with source.txt and text.txt, find four sentences ____
(set -x; ../scripal -a source.txt -b text.txt)

echo ____ test file match with source.txt and text.txt, write result to resultOut.txt ____
(set -x; ../scripal -a source.txt -b text.txt -d resultOut.txt)
echo -e "result:"
cat resultOut.txt

echo
echo ____ find the word \'bee\' at [18,20] in string ____
(set -x; ../scripal "match find( all ('bee'))"   "i can see a honey bee")

echo
echo ____ find the word \'bee\' at [18,20], show comp, code, execution ____
(set -x; ../scripal --debugrun --showcode "match find( all ('bee'))"   "i can see a honey bee")

echo
echo ____ nearest match for the word \'test\' in nearestTxt.txt, matches 3 times ____
(set -x; ../scripal "test" -b nearestTxt.txt -f 1) 

echo
echo ____ nearest match for the word held in nearestSrc.txt in nearestTxt.txt, matches 3 times ____
(set -x; ../scripal -a nearestSrc.txt -b nearestTxt.txt -f 1)

echo
echo ____ block match for the word \'test\' in 'test1', distance '>=' 0.9, should match once ____
(set -x; ../scripal "test" "test1" -g 0.9)

echo
echo ____ block match for the word held in blockSrc.txt in blockTxt.txt, should match once ____
(set -x; ../scripal -a blockSrc.txt -b blockTxt.txt -g 0.9)

echo
echo ____ file search for the word \'case\' in all files, 12 matches ____
(set -x; ../scripal "match find('case'); end; loop;" -p "../test/" -o)

echo
echo ____ file search for the word \'case\' in all .txt files, 2 matches ____
(set -x; ../scripal "match find('case'); end; loop;" -p "../test/" -q "txt" -o)

echo
echo ____ file search for the word \'case\', nearest match, 2 matches ____
(set -x; ../scripal "case" -p "../test/" -q "txt" -o -f 2)

echo
echo ____ file search for the file similar to \'test\', block match, 2 matches ____
(set -x; ../scripal "test" -p "../test/" -q "txt" -o -g 0.9)

echo
echo ____ replace the word \'dog\' in text with \'cat\' ____
text="look for dog, my dog should be replaced with another animal, it's a dog's life"
echo -e "text is:\n$text"
echo -e "result is:"
(set -x; ../scripal "match find('dog'); end; loop;" "$text" -r "cat")

echo
echo ____ replace the word \'dog\' in file dog.txt with \'cat\' ____
text="look for dog, my dog should be replaced with another animal, it's a dog's life"
printf "$text" > dog.txt
(set -x; ../scripal "match find('dog'); end; loop;" -b dog.txt -r "cat")
echo -e "result:"
cat dog.txt_
echo

echo
echo ____ split  string at spaces ____
text="    test1 test2    test3   "
echo -e "text is:\n$text\n"
echo -e "result is:"
(set -x; ../scripal "match find( blank ); ifMatch end; loop" "$text" -s)

echo
echo ____ split file testSplit.txt at spaces and store in testSplitRes.txt ____
(set -x; ../scripal "match find( blank ); ifMatch end; loop" -b testSplit.txt -s --outfile=testSplitRes.txt)
echo -e "result:"
cat testSplitRes.txt
echo

echo
echo ____ convert files to utf16l,latin1, win1250 ____
(set -x; ../scripal -k -a utf8//text.txt -b utf16l//text_utf16l.txt)
echo generating ../test/text_utf16l.txt
(set -x; ../scripal -k -a utf8//text.txt -b latin1//text_latin1.txt)
echo generating ../test/text_latin1.txt
(set -x; ../scripal -k -a utf8//text.txt -b win1250//text_winb1250.txt)
echo generating ../test/text_win1250.txt

