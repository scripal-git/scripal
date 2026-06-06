# Scripal: Starting in JavaScript (NodeJS)

Currently Scripal has been tested only under a single JavaScript platform: Node.js. 
Scripal is most efficient when using UTF-8 as base encoding in JavaScript (default in Node.js).
Using other encodings causes some performance overhead.

## Installation

<pre>
npm install scripal
</pre>

Scripal relies on **@makeomatic/ffi-napi**, which requires modules **nan** and **ref-napi** <br>
Now start to write your first code.

## Additional installation of full scripal package with command line tool 

[prerequisites](/docs/InstallPrerequisites.md) <br>

[install packages](/docs/InstallPackage.md) <br>

## getting started

The JavaScript module is [scripal.js](/wrapper/js/scripal.js). It consists of only one file. <br>
See how to use Scripal in JavaScript [test.js](/wrapper/js/test.js) <br>

The function  **scripal.baseInit(path)** must be called once to initialize the library. <br>
Every single thread must call **scripal.libInit(config, encoding)** to specify the entire path to the config file used (leave empty for default) and the encodig of the OS environment. **scripal.ENC_DEFAULT** will denote the standard.  

## run Node.js test program 

<pre>
const os = require('os');
const fs = require('fs');

// ffi-napi must be installed

// init functions
let lpath = "";
if (process.platform === "linux") {
  global.scripal = require('./scripal');
}

if (process.platform === "win32") {  
  global.scripal = require('.\\scripal');
}

// set path to library or leave as "" for default 
scripal.baseInit(lpath);
scripal.libInit("", scripal.ENC_UTF8);
scripal.logInit("buffer");

let obj = new scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");

let result = obj.match("Find the words apple, ape and award.");
console.log("source match result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i], obj.results.positions[i]);
}
</pre>

for more information see [matching with Scripal](/docs/IntroMatching.md) and [library](/docs/LibraryBase.md) 





