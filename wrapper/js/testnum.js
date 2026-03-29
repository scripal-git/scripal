// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0
// find numbers in text and files

"use strict";

const os = require('os');
const fs = require('fs');

// npm install ffi-napi

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

// basic methods
scripal.clearErr();
scripal.setEncoding(scripal.ENC_UTF8);

// ********************************************************
// match hex number in the range 0001-ff00
// ********************************************************
let obj = new scripal.TFScripal("source", "match find( pure hex [1,ff00] )");

let result = obj.match("Look for number 0x01e6 but not 0x12ffe.");
showResult();

// ********************************************************
// match binary number in the range 0001-1110
// ********************************************************
obj = new scripal.TFScripal("source", "match find( pure bin [1,1110] )");

result = obj.match("Look for number 1100 but not 1111.");
showResult();

// ********************************************************
// find all numbers (float, int) between 1 and 200
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find ( [1,200] ); loop");

result = obj.match("Find numbers +3, 199, 60, 80.9 but not 0 , -70 or 201");
showResult();

// ********************************************************
// find all integer numbers between -200 and 200
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find( int[-200,200] ); loop");

result = obj.match("Find integer numbers 0, 3, 199, -60 but not 3.5, .70, 500 or +199.67");
showResult();

// ********************************************************
// find all IDs from ATX-4-103 to ATX-4-206
// ********************************************************
obj = new scripal.TFScripal( "source", "matchEnd find( 'ATX-4-' int[103,206] ); loop");

result = obj.match("Find ATX-4-170, ATX-4-189, ATX-4-206 but not ATX-4-102 and ATX-4-207");
showResult();

// ********************************************************
// find specific number formats, here all IPv4 addresses
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find( pure[0,255] '.' pure[0,255] '.' pure[0,255] '.' pure[0,255]); loop");

result = obj.match("128.0.1.1, 45.6.7.9, 255.255.255.0 and 0.0.0.0, but not 1234.5.7.4, 66.89.56.2222 or 257.45.232.4");
showResult();

// ********************************************************
// match time range from 2020.07.14 8:10 to 8:14
// 2020.7.14 , 2020.07.14, 08:10, 8:11 etc. 
// ********************************************************
obj = new scripal.TFScripal("source", "match (pure 2020 '.' pure 7 '.'  pure 14 blank pure 8 ':' int [10,14]) ");

result = obj.match("2020.07.14 8:11");
showResult();

// ********************************************************
// match all dates from 1900 on with a certain! accuracy
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find( pure [1900,@] '.' pure [1,12] '.' pure [1,31] ); loop");

result = obj.match("Find dates like 2020.12.30 or 2021.01.14 but not numbers like 12344.23 or 12344.23.12");
showResult();

// ********************************************************
// find telephone numbers in NANP format
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find( '(' repeat[3]( digit ) ') ' repeat[3]( digit ) '-' repeat[4]( digit ));loop");

result = obj.match("The company may be reached globally under (234) 235-5678, (278) 678-2933, (113) 114-3682 or (227) 145-1993");
showResult();

// ********************************************************
//  find all numbers between 100 and 200 in JS files 
// ********************************************************
obj = new scripal.TFScripal("source", "matchEnd find( int[100,200] ); loop");

result = obj.matchFile("testnum.js");
showResult();

console.log("results as CSV:"); 
console.log(obj.getResultCSV());
console.log("results as JSON:");
console.log(obj.getResultJSON(), "\n");
console.log("results as HRF:");
console.log(obj.getResultHRF());
obj = null;

scripal.libFinalize();

function showResult() {
  console.log("source match result: ", result , " , no. of results: " , obj.getResultSize());
  for (let i = 0; i < obj.results.size(); i++) {
    console.log(obj.results.text[i], obj.results.positions[i]);
  };
  console.log(" ");
};  
