# scripal

Scripal is a universal tool to find, match or manipulate text and numbers in strings and files. 
Find numbers and number ranges easily in strings or files. Valid and extract numbers. 

It's written in C++ for optimal performance, and is available for many languages, including Node.js. 
Scripal also comes as a stand-alone tool to be used in a terminal.
The tool is meant to be an alternative to regular expressions, but offers more functionality, like finding similar words based on edit distances and file operations. 
It's a multi-purpose text processor.

You can find numbers and number ranges easily, also embedded in IDs, dates etc.
No more hassling with non-intuitive RegEx Phrases.
Find embedded numbers, as in ID45ctx or pure, isolated numbers using the keyword "pure".
run **node testnum.js** for examples. 
'testnum.js' is stored in your **node_modules/scripal** folder.

For a full documentation, please look at <https://github.com/scripal-git/scripal> <br>
and <https://github.com/scripal-git/scripal/blob/main/docs/StartingInJavaScript.md> <br>

To write comments and requests, please use <scripal@ksum.de>

## Usage

### match hex numbers in the range 0001-ff00
```JS
let lpath = "";

global.scripal = require('scripal');

scripal.baseInit(lpath);

scripal.clearErr();
scripal.setEncoding(scripal.ENC_UTF8);

let obj = new scripal.TFScripal("source", "match find( pure hex [1,ff00] )");

let result = obj.match("Look for number 0x01e6 but not 0x12ffe.");
showResult();
```

## Tryout

Run **node test.js** and **node testnum.js** in the scripal folder.

## Examples:

match hex number in the range 0001-ff00

match find( pure hex [1,ff00] )

-----------------------------------------------------

match binary number in the range 0001-1110

match find( pure bin [1,1110] )

-----------------------------------------------------

find all numbers (float, int) between 1 and 200

matchEnd find ( [1,200] ); loop

-----------------------------------------------------

find all integer numbers between -200 and 200

matchEnd find( int[-200,200] ); loop

-----------------------------------------------------

find all IDs from ATX-4-103 to ATX-4-206

matchEnd find( 'ATX-4-' int[103,206] ); loop

-----------------------------------------------------

find specific number formats, here all IPv4 addresses

matchEnd find( pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255]); loop

-----------------------------------------------------

match time range from 2020.07.14 8:10 to 8:14
2020.7.14 , 2020.07.14, 08:10, 8:11 etc. 

match (pure 2020 '.' pure 7 '.'  pure 14 blank pure 8 ':' int [10,14]) 

-----------------------------------------------------

match all dates from 1900 on with a certain! accuracy

matchEnd find( pure [1900,@] '.' pure [1,12] '.' pure [1,31] ); loop

-----------------------------------------------------

find telephone numbers in NANP format

matchEnd find( '(' repeat[3]( digit ) ') ' repeat[3]( digit ) '-' repeat[4]( digit ));loop

-----------------------------------------------------

find all numbers between 100 and 200  

matchEnd find( int[100,200] ); loop

-----------------------------------------------------

JS example, search in file testnum.js:

```JS
let obj = new scripal.TFScripal("source", "matchEnd find( int[100,200] ); loop");
result = obj.matchFile("testnum.js");
showResult();

get results in many formats, from CSV to simple human readable format

console.log(obj.getResultCSV());
console.log(obj.getResultJSON()
console.log(obj.getResultHRF())
```


Get started immediately, don't ruin your nerves with RegEx phrases. Have fun. 

## Installation

```bash
npm install scripal
```

During install, the postinstall script (`install.js`) automatically:

1. **Checks** whether `ffi-napi` is installed – installs it if not.
2. **Detects** the operating system.
3. **Downloads** the matching pre-built native library from GitHub Releases:
   | Platform | Archive | Extracted as |
   |---|---|---|
   | Linux x64 | `scripal-so-linux-x64.tar.xz` | `libscripal.so` |
   | Windows x64 | `scripal-dll-win32-x64.tar.xz` | `scripal.dll` |
4. **Extracts** the library into the package directory (next to `scripal.js`).

## Running tests

Files test.js and testnum.js are in scripal module folder.

```
node test.js
```
or
```
node testnum.js
```

## Supported platforms

- Linux x64
- Windows x64

## License

**BSL-1.0**
