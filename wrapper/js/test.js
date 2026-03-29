// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

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
scripal.libInit("", scripal.ENC_UTF8);
scripal.logInit("buffer");

// provoke error and output messages
scripal.testError();
console.log("getErr: ", scripal.getErrMsg());
console.log("getErrExp: ", scripal.getErrExp());
console.log("getErrPos: ", scripal.getErrPos());
console.log(" ");

// basic methods
scripal.clearErr();
scripal.setEncoding(scripal.ENC_UTF8);

console.log("Version: ", scripal.getVersion(), "\n");

// matches

let obj = new scripal.TFScripal("source", "match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')");

let result = obj.match("Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.");
console.log("source match result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i], obj.results.positions[i]);
}
console.log(" ");
obj = null;

scripal.clearLogBuf();
scripal.config.fromJSON('{ "debugRun": "true" }');
obj = new scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");

result = obj.match("Find the words apple, ape and award.");
console.log("log buffer: \n" + scripal.getLogBuf());
console.log("source match result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i], obj.results.positions[i]);
}
console.log("results as CSV:"); 
console.log(obj.getResultCSV());
console.log("results as JSON:");
console.log(obj.getResultJSON(), "\n");
console.log("results as HRF:");
console.log(obj.getResultHRF());
obj = null;

obj = new scripal.TFScripal("nearest", "test", 0.5);
result = obj.match("Find the words like test, tests and TEST.");
console.log("nearest match result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i]);
}
console.log(" ");

result = obj.matchFile("test.txt");
console.log("match file result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i]);
}
obj = null;

obj = new scripal.TFScripal("block", "testing", 0.5);
result = obj.match("resting");
console.log("block match result: ", result , " 'resting'");
console.log(obj.results.ratings[0]);
result = obj.match("testings");
console.log("block match result: ", result , " 'testings'");
console.log(obj.results.ratings[0]);
result = obj.match("nesting");
console.log("block match result: ", result , " 'nesting'");
console.log(obj.results.ratings[0]);
console.log(" ");
obj = null;

obj = new scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
result = obj.replace("Find the words apple, ape plus award.", "___");
console.log("source replace result: ", result , obj.results.text[0]);
console.log(" ");
obj = null;

obj = new scripal.TFScripal("nearest", "test", 0.5);
result = obj.replaceFile("test.txt", "testout.txt", "___");
obj = null;

obj = new scripal.TFScripal("source", "match find(any (' ', ',', '.')); ifMatch end; loop;");
result = obj.split("Find the words apple, ape plus award.");
console.log("split result: ", result , " , no. of results: " , obj.getResultSize());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.text[i]);
}
console.log(" ");
obj = null;

obj = new scripal.TFScripal("source", "match find(any (' ', ',','.')); ifMatch end; loop;");
result = obj.splitFile("test.txt", "testout.txt");
let f = fs.readFileSync("testout.txt", "utf8");
console.log("split file result:\n", f);
obj = null;

// file search
obj = new scripal.TFScripal("nearest", "test", 0.5);
result = obj.searchFiles("./", "*");
console.log("file search result: ", result , " , no. of results: " , obj.results.size());
for (let i = 0; i < obj.results.size(); i++) {
  console.log(obj.results.fileNames[i], ",'", obj.results.text[i], "',", obj.results.positions[i],",", obj.results.ratings[i].toFixed(6));
}
console.log(" ");
obj = null;

// config
console.log("config as JSON:");
console.log(scripal.config.toJSON());
console.log(" ");

console.log("config settings 'negSign' and 'encoding' as JSON:");
console.log(scripal.config.toJSONVal("negSign,encoding"));
console.log(" ");

console.log("config settings 'negSign' after changing to '+':");
scripal.config.fromJSON('{ "negSign": "+" }');
console.log(scripal.config.toJSONVal("negSign"));
console.log(" ");

console.log("store config in conf.js, result: ", scripal.config.store("conf.js"));
console.log(" ");

console.log("restore config from conf.js, result: ", scripal.config.restore("conf.js"));
console.log(" ");

// templates
scripal.templates.reset();

scripal.templates.set("findtest", "match find('test')");

console.log("store templates in templates.js, result: ", scripal.templates.store("templates.js"));
console.log(" ");

console.log("template findtest: ", scripal.templates.get("findtest"));
console.log(" ");

scripal.templates.clear("findtest");

console.log("restore templates from templates.js, result: ", scripal.templates.restore("templates.js"));
console.log(" ");

console.log("templates to JSON: ", scripal.templates.toJSON());
console.log(" ");

console.log("template findtest to JSONVal: ", scripal.templates.toJSONVal("findtest"));
console.log(" ");

scripal.libFinalize();