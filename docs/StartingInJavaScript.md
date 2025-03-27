# Scripal: Starting in JavaScript (NodeJS)

Currently Scripal has been tested only under a single JavaScript platform: Node.js. 
Scripal is most efficient when using UTF-8 as base encoding in JavaScript (default in Node.js).
Using other encodings causes some performance overhead.

## Installation

[prerequisites](/docs/InstallPrerequisites.md) <br>

### ready to use packages and module

[install packages](/docs/InstallPackage.md) <br>

### JavaScript module
Scripal relies on ffi-napi, which requires modules nan and ref-napi <br>

<pre>
npm install nan
npm install ref-napi
</pre>

Install FFI package:

https://www.npmjs.com/package/@makeomatic/ffi-napi

<pre>
npm install @makeomatic/ffi-napi
</pre>


These modules must be available.
The JavaScript module is [scripal.js](/wrapper/js/scripal.js). It consists of only one file. <br>
See how to use Scripal in JavaScript [test.js](/wrapper/js/test.js) <br>
Make sure to specify the correct path to the Scripal shared library when calling **scripal.baseInit(path)**. This must be a path to the folder holding libscripal.so oder scripal.dll.
The function must be called once to initialize the library. <br>
Every single thread must call **scripal.libInit(config, encoding)** to specify the entire path to the config file used (leave empty for default) and the encodig of the OS environment. **scripal.ENC_DEFAULT** will denote the standard.  

## run Node.js test program 
<pre>
"use strict";

const os = require('os');
const fs = require('fs');

// init functions
let lpath = "";
if (process.platform === "linux") {
  lpath = "";
  global.scripal = require('./scripal');
}

if (process.platform === "win32") {  
  lpath = "..\\..\\win\\x64\\Debug";
  global.scripal = require('.\\scripal');
}
scripal.baseInit(lpath);
scripal.libInit("", scripal.ENC_UTF8);
scripal.logInit("stdout");

let obj = new scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");

let result = obj.match("Find the words apple, ape and award.");
console.log("source match result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i], obj.results.positions[i]);
}
</pre>

for more information see [matching with Scripal](/docs/IntroMatching.md) and [library](/docs/LibraryBase.md) 





