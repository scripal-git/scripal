// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

// Scripal in JavaScript

"use strict";
const ref = require("ref-napi");
if (process.platform === "linux") {
  global.ffi = require("@makeomatic/ffi-napi");
}
if (process.platform === "win32") {
  global.ffi = require("@makeomatic/ffi-napi");
}
const path = require("path");
const fs = require('fs');

// Define basic types using ref
const voidPtr = ref.refType(ref.types.void);
const intPtr = ref.refType(ref.types.int);
const doublePtr = ref.refType(ref.types.double);

// Global variables
let scripalLib = null;
let initialized = false;
let scripalInit = false;

// encodings
const ENC_DEFAULT  = 1;
const ENC_UTF8     = 2;
const ENC_UTF16L   = 3;
const ENC_UTF16B   = 4;
const ENC_UTF32L   = 5;
const ENC_UTF32B   = 6;
const ENC_ASCII    = 10;  
const ENC_CP932    = 11;  
const ENC_CP936    = 12;  
const ENC_CP949    = 13;  
const ENC_CP950    = 14;  
const ENC_LATIN1   = 30;  
const ENC_LATIN2   = 31;  
const ENC_LATIN9   = 32;  
const ENC_WIN874   = 50;  
const ENC_WIN1250  = 51;  
const ENC_WIN1251  = 52;  
const ENC_WIN1252  = 53;  
const ENC_WIN1253  = 54;  
const ENC_WIN1254  = 55;  
const ENC_WIN1255  = 56;  
const ENC_WIN1256  = 57;  
const ENC_WIN1257  = 58;  
const ENC_WIN1258  = 59;  

// matching algorithms (block and nearest)
const PATTERN_LEVEN_WORD = 1;
const PATTERN_LEVENPLUS_WORD = 2;
const PATTERN_LEVEN = 3;
const PATTERN_JARO = 100;
const PATTERN_JAROWINKLER = 101;
const PATTERN_JAROWINKLER_WORD = 102;

// match models
const MATCH_SOURCE      = 1;
const MATCH_NEAREST     = 2;
const MATCH_BLOCK       = 3;

// result position types
const POS_UTF8          = 1;
const POS_OFFSET        = 2;
const POS_COUNT         = 3;

// constants
const NO_POS = -1;
const NO_VALUE = -2;

function findNodeModulesPath(startPath = __dirname) {
    let currentPath = path.resolve(startPath);

    while (true) {
        const potentialPath = path.join(currentPath, 'node_modules');
        
        if (fs.existsSync(potentialPath) && fs.lstatSync(potentialPath).isDirectory()) {
            return potentialPath;
        }

        const parentPath = path.dirname(currentPath);
        
        if (parentPath === currentPath) {
            break;
        }
        
        currentPath = parentPath;
    }

    return null;
}

function baseInit(libPath = "") {
  // global variables
  // Scripal wrapper for JavaScript
  if (process.platform === "linux") {
    if (libPath === "") {
      libPath = findNodeModulesPath() +"/scripal/libscripal.so";
      if (!fs.existsSync(libPath)) {
        libPath = "/usr/lib/scripal/libscripal.so";
        if (!fs.existsSync(libPath))
          libPath = "/usr/lib64/scripal/libscripal.so";
      }  
    }
    scripalLib = ffi.Library(libPath, {
      "printText": ["void", ["string", "int", "int"]],
      "getVersion": ["void", [voidPtr, voidPtr]],
      "baseInit": ["void", []],
      "baseFinalize": ["void", []],
      "libInit": ["void", ["string", "int", "int"]],
      "libFinalize": ["void", []],
      "logInit": ["void", ["string", "int"]],
      "getLogBuf": ["void", [voidPtr, voidPtr]],
      "clearLogBuf": ["void", []],
      "deletePtr": ["void", [voidPtr]],
      "getErrMsg": ["void", [voidPtr, voidPtr]],
      "getErrExp": ["void", [voidPtr, voidPtr]],
      "getErrPos": ["int", []],
      "clearErr": ["void", []],
      "setEncoding": ["void", ["int"]],
      "newCodeObj": ["int", ["string", "int"]],
      "newNearestObj": ["int", ["string", "int", "double"]],
      "newBlockObj": ["int", ["string", "int", "double"]],
      "deleteObj": ["void", ["int"]],
      "match": ["bool", ["int", "string", "int"]],
      "matchFile": ["bool", ["int", "string", "int", "int"]],
      "replace": ["bool", ["int", "string", "int", "string", "int"]],
      "replaceFile": ["bool", ["int", "string", "int", "string", "int", "string", "int", "int"]],
      "split": ["bool", ["int", "string", "int"]],
      "splitFile": ["bool", ["int", "string", "int", "string", "int", "int"]],
      "searchFiles": ["bool", ["int", "string", "int", "string", "int", "bool"]],
      "getResultSize": ["int", ["int"]],
      "getResultSpecs": ["bool", ["int", "int", intPtr, intPtr, intPtr]],
      "getResultStr": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultFileName": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultTag": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultPos": ["bool", ["int", "int", intPtr, intPtr]],
      "getResultRating": ["bool", ["int", "int", doublePtr]],
      "getResultJSON": ["bool", ["int", voidPtr, intPtr]],
      "getResultCSV": ["bool", ["int", voidPtr, intPtr]],
      "getResultHRF": ["bool", ["int", voidPtr, intPtr]],
      "configSetLocale": ["bool", ["string", "int"]],
      "configToJSON": ["bool", [voidPtr, intPtr]],
      "configToJSONVal": ["bool", [voidPtr, intPtr, "string", "int"]],
      "configFromJSON": ["bool", ["string", "int"]],
      "configStore": ["bool", ["string", "int"]],
      "configRestore": ["bool", ["string", "int"]],
      "templateSet": ["bool", ["string", "int", "string", "int"]],
      "templateClear": ["bool", ["string", "int"]],
      "templateReset": ["void", []],
      "templateGet": ["bool", ["string", "int", voidPtr, intPtr]],
      "templateStore": ["bool", ["string", "int"]],
      "templateRestore": ["bool", ["string", "int"]],
      "templatesToJSON": ["bool", [voidPtr, intPtr]],
      "templatesToJSONVal": ["bool", [voidPtr, intPtr, "string", "int"]],
      "templatesFromJSON": ["bool", ["string", "int"]]
    });
    config = new TFConfig();
  }

  if (process.platform === "win32") {
    if (libPath === "") {
      libPath = "..\\..\\win\\x64\\Release\\dll";  
      if (!fs.existsSync(libPath))       
        libPath = process.cwd();
    }
    // os.add_dll_directory equivalent: modify PATH environment variable
    process.env.PATH = libPath + ";" + process.env.PATH;
    scripalLib = ffi.Library(path.join(libPath, "scripal.dll"), {
      "printText": ["void", ["string", "int", "int"]],
      "getVersion": ["void", [voidPtr, voidPtr]],
      "baseInit": ["void", []],
      "baseFinalize": ["void", []],
      "libInit": ["void", ["string", "int", "int"]],
      "libFinalize": ["void", []],
      "logInit": ["void", ["string", "int"]],
      "getLogBuf": ["void", [voidPtr, voidPtr]],
      "clearLogBuf": ["void", []],
      "deletePtr": ["void", [voidPtr]],
      "getErrMsg": ["void", [voidPtr, voidPtr]],
      "getErrExp": ["void", [voidPtr, voidPtr]],
      "getErrPos": ["int", []],
      "clearErr": ["void", []],
      "setEncoding": ["void", ["int"]],
      "newCodeObj": ["int", ["string", "int"]],
      "newNearestObj": ["int", ["string", "int", "double"]],
      "newBlockObj": ["int", ["string", "int", "double"]],
      "deleteObj": ["void", ["int"]],
      "match": ["bool", ["int", "string", "int"]],
      "matchFile": ["bool", ["int", "string", "int", "int"]],
      "replace": ["bool", ["int", "string", "int", "string", "int"]],
      "replaceFile": ["bool", ["int", "string", "int", "string", "int", "string", "int", "int"]],
      "split": ["bool", ["int", "string", "int"]],
      "splitFile": ["bool", ["int", "string", "int", "string", "int", "int"]],
      "searchFiles": ["bool", ["int", "string", "int", "string", "int", "bool"]],
      "getResultSize": ["int", ["int"]],
      "getResultSpecs": ["bool", ["int", "int", intPtr, intPtr, intPtr]],
      "getResultStr": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultFileName": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultTag": ["bool", ["int", "int", voidPtr, intPtr]],
      "getResultPos": ["bool", ["int", "int", intPtr, intPtr]],
      "getResultRating": ["bool", ["int", "int", doublePtr]],
      "getResultJSON": ["bool", ["int", voidPtr, intPtr]],
      "getResultCSV": ["bool", ["int", voidPtr, intPtr]],
      "getResultHRF": ["bool", ["int", voidPtr, intPtr]],
      "configSetLocale": ["bool", ["string", "int"]],
      "configToJSON": ["bool", [voidPtr, intPtr]],
      "configToJSONVal": ["bool", [voidPtr, intPtr, "string", "int"]],
      "configFromJSON": ["bool", ["string", "int"]],
      "configStore": ["bool", ["string", "int"]],
      "configRestore": ["bool", ["string", "int"]],
      "templateSet": ["bool", ["string", "int", "string", "int"]],
      "templateClear": ["bool", ["string", "int"]],
      "templateReset": ["void", []],
      "templateGet": ["bool", ["string", "int", voidPtr, intPtr]],
      "templateStore": ["bool", ["string", "int"]],
      "templateRestore": ["bool", ["string", "int"]],
      "templatesToJSON": ["bool", [voidPtr, intPtr]],
      "templatesToJSONVal": ["bool", [voidPtr, intPtr, "string", "int"]],
      "templatesFromJSON": ["bool", ["string", "int"]]
    });
  }

  scripalLib.baseInit();
  initialized = true;
  return;
}

function getVersion() {
  let verC = ref.alloc(voidPtr);
  let lenC = ref.alloc(ref.types.int, 0);
  scripalLib.getVersion(verC, lenC);
  let verPtr = verC.deref();
  let version = ref.readCString(verPtr, 0);
  scripalLib.deletePtr(verPtr);
  return version;
}

function baseFinalize() {
  scripalLib.baseFinalize();
  return;
}

function libInit(configPath, encoding = 1) {
  // global scripalInit, config, templates, initialized
  if (!initialized) {
    baseInit();
  }
  clearErr();
  let configPathC = Buffer.from(configPath, "utf8");
  scripalLib.libInit(configPathC.toString(), configPathC.length, encoding);
  config = new TFConfig(config);
  templates = new TFTemplate();
  if (getErrMsg() !== "") throw new Error(getErrExp());
  if (!scripalInit) {
    scripalInit = true;
    templates = new TFTemplate();
  }
  return;
}

function libFinalize() {
  scripalLib.libFinalize();
  return;
}

function logInit(logChannel) {
  clearErr();
  let logChannelC = Buffer.from(logChannel, "utf8");
  scripalLib.logInit(logChannelC.toString(), logChannelC.length);
  if (getErrMsg() !== "") throw new Error(getErrExp());
  return;
}

function getLogBuf() {
  let bufC = ref.alloc(voidPtr);
  let lenC = ref.alloc(ref.types.int, 0);
  scripalLib.getLogBuf(bufC, lenC);
  let bufPtr = bufC.deref();
  let logMsg = ref.readCString(bufPtr, 0);
  scripalLib.deletePtr(bufPtr);
  return logMsg;
}

function clearLogBuf() {
  scripalLib.clearLogBuf();
  return;
}

function getErrMsg() {
  let errC = ref.alloc(voidPtr);
  let lenC = ref.alloc(ref.types.int, 0);
  scripalLib.getErrMsg(errC, lenC);
  // print("getErrMsg: ", hex(c_void_p(errC.value).value))
  let errPtr = errC.deref();
  let errMsg = ref.readCString(errPtr, 0);
  scripalLib.deletePtr(errPtr);
  return errMsg;
}

function getErrExp() {
  let errC = ref.alloc(voidPtr);
  let lenC = ref.alloc(ref.types.int, 0);
  scripalLib.getErrExp(errC, lenC);
  let errPtr = errC.deref();
  let errMsg = ref.readCString(errPtr, 0);
  scripalLib.deletePtr(errPtr);
  return errMsg;
}

function getErrPos() {
  return parseInt(scripalLib.getErrPos());
}

function clearErr() {
  scripalLib.clearErr();
  return;
}

function setEncoding(encoding) {
  clearErr();
  scripalLib.setEncoding(encoding);
  if (getErrMsg() !== "") throw new Error(getErrExp());
  return;
}

function testError() {
  // Attempt to delete an invalid pointer
  scripalLib.deletePtr(ref.alloc(ref.types.int, 1));
  return;
}

class TFResults {
  constructor() {
    this.id = -1; // c_int(-1)
    this.encoding = ENC_UTF8;
    this.matchType = MATCH_SOURCE;
    this.positionType = POS_UTF8;
    this.fileNames = [];
    this.text = [];
    this.positions = [];
    this.ratings = [];
    this.tags = [];
  }
  size() {
    return scripalLib.getResultSize(this.id);
  }
}

class TFScripal {
  constructor(matchtype, source, dist = 0, encoding = ENC_UTF8) {
    clearErr();
    let sourceC = Buffer.from(source, "utf8");
    this.results = new TFResults();
    // Initialize id based on matchtype
    if (matchtype.toLowerCase() === "source") {
      this.id = scripalLib.newCodeObj(sourceC.toString(), sourceC.length);
    } else if (matchtype.toLowerCase() === "nearest") {
      this.id = scripalLib.newNearestObj(sourceC.toString(), sourceC.length, dist);
    } else if (matchtype.toLowerCase() === "block") {
      this.id = scripalLib.newBlockObj(sourceC.toString(), sourceC.length, dist);
    }
    if (getErrMsg() !== "") throw new Error(getErrExp());
  }
  
  __del__() {
    if (this.id !== -1) {
      scripalLib.deleteObj(this.id);
    }
    return;
  }
  
  fillResults() {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    let posStart = ref.alloc(ref.types.int, 0);
    let posEnd = ref.alloc(ref.types.int, 0);
    let encodingVal = ref.alloc(ref.types.int, 0);
    let matchTypeVal = ref.alloc(ref.types.int, 0);
    let positionTypeVal = ref.alloc(ref.types.int, 0);
    let rating = ref.alloc(ref.types.double, 0);
  
    this.results.id = this.id;
    this.results.fileNames = [];
    this.results.text = [];
    this.results.positions = [];
    this.results.ratings = [];
    this.results.tags = [];
  
    for (let index = 0; index < this.getResultSize(); index++) {
      let resultBool = scripalLib.getResultSpecs(this.id, index, encodingVal, matchTypeVal, positionTypeVal);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      this.results.encoding = encodingVal.deref();
      this.results.matchType = matchTypeVal.deref();
      this.results.positionType = positionTypeVal.deref();
  
      resultBool = scripalLib.getResultFileName(this.id, index, resultC, lenC);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      let fileName = ref.readCString(resultC.deref(), 0);
      this.results.fileNames.push(fileName);
  
      resultBool = scripalLib.getResultStr(this.id, index, resultC, lenC);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      let textStr = ref.readCString(resultC.deref(), 0);
      this.results.text.push(textStr);
  
      scripalLib.getResultPos(this.id, index, posStart, posEnd);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      this.results.positions.push([posStart.deref(), posEnd.deref()]);
  
      scripalLib.getResultRating(this.id, index, rating);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      this.results.ratings.push(rating.deref());
  
      resultBool = scripalLib.getResultTag(this.id, index, resultC, lenC);
      if (getErrMsg() !== "") throw new Error(getErrExp());
      let tagStr = ref.readCString(resultC.deref(), 0);
      this.results.tags.push(tagStr);
    }
    return;
  }
  
  match(text) {
    clearErr();
    let textC = Buffer.from(text, "utf8");
    let result = scripalLib.match(this.id, textC.toString(), textC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  matchFile(file, fileEncoding = ENC_UTF8) {
    clearErr();
    let fileC = Buffer.from(file, "utf8");
    let result = scripalLib.matchFile(this.id, fileC.toString(), fileC.length, fileEncoding) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  replace(text, replaceStr) {
    clearErr();
    let textC = Buffer.from(text, "utf8");
    let replaceC = Buffer.from(replaceStr, "utf8");
    let result = scripalLib.replace(this.id, textC.toString(), textC.length, replaceC.toString(), replaceC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  replaceFile(fileIn, fileOut, replaceStr, fileEncoding = ENC_UTF8) {
    clearErr();
    let fileInC = Buffer.from(fileIn, "utf8");
    let fileOutC = Buffer.from(fileOut, "utf8");
    let replaceC = Buffer.from(replaceStr, "utf8");
    let result = scripalLib.replaceFile(this.id, fileInC.toString(), fileInC.length, fileOutC.toString(), fileOutC.length, 
                                        replaceC.toString(), replaceC.length, fileEncoding) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  split(text) {
    clearErr();
    let textC = Buffer.from(text, "utf8");
    let result = scripalLib.split(this.id, textC.toString(), textC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  splitFile(fileIn, fileOut, fileEncoding = ENC_UTF8) {
    clearErr();
    let fileInC = Buffer.from(fileIn, "utf8");
    let fileOutC = Buffer.from(fileOut, "utf8");
    let result = scripalLib.splitFile(this.id, fileInC.toString(), fileInC.length, fileOutC.toString(), fileOutC.length, fileEncoding) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  searchFiles(pathStr, dirExtension, recursive = false) {
    clearErr();
    let pathC = Buffer.from(pathStr, "utf8");
    let dirExtensionC = Buffer.from(dirExtension, "utf8");
    let result = scripalLib.searchFiles(this.id, pathC.toString(), pathC.length, dirExtensionC.toString(), dirExtensionC.length, recursive) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    this.fillResults();
    return result;
  }
  
  getResultSize() {
    return scripalLib.getResultSize(this.id);
  }
  
  getResultJSON() {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    scripalLib.getResultJSON(this.id, resultC, lenC);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let jsonStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return jsonStr;
  }
  
  getResultCSV() {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    scripalLib.getResultCSV(this.id, resultC, lenC);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let csvStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return csvStr;
  }
  
  getResultHRF() {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    scripalLib.getResultHRF(this.id, resultC, lenC);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let hrfStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return hrfStr;
  }
}

class TFConfig {
  constructor(config) {
    return;
  }
  
  setLocale(locale) {
    clearErr();
    let localeC = Buffer.from(locale, "utf8");
    let result = scripalLib.configSetLocale(localeC.toString(), localeC.length);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  toJSON() {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    scripalLib.configToJSON(resultC, lenC);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let jsonStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return jsonStr;
  }
  
  toJSONVal(names) {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    let namesC = Buffer.from(names, "utf8");
    scripalLib.configToJSONVal(resultC, lenC, namesC.toString(), namesC.length);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let jsonStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return jsonStr;
  }
  
  fromJSON(textJSON) {
    clearErr();
    let textJSONC = Buffer.from(textJSON, "utf8");
    let result = scripalLib.configFromJSON(textJSONC.toString(), textJSONC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  store(file) {
    clearErr();
    let fileC = Buffer.from(file, "utf8");
    let result = scripalLib.configStore(fileC.toString(), fileC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  restore(file) {
    clearErr();
    let fileC = Buffer.from(file, "utf8");
    let result = scripalLib.configRestore(fileC.toString(), fileC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
}

class TFTemplate {
  set(name, source) {
    clearErr();
    let nameC = Buffer.from(name, "utf8");
    let sourceC = Buffer.from(source, "utf8");
    let result = scripalLib.templateSet(nameC.toString(), nameC.length, sourceC.toString(), sourceC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  clear(name) {
    clearErr();
    let nameC = Buffer.from(name, "utf8");
    let result = scripalLib.templateClear(nameC.toString(), nameC.length) ? true : false;
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  reset() {
    scripalLib.templateReset();
    return;
  }
  
  get(name) {
    clearErr();
    let resultC = ref.alloc(voidPtr);
    let lenC = ref.alloc(ref.types.int, 0);
    let nameC = Buffer.from(name, "utf8");
    let resultBool = scripalLib.templateGet(nameC.toString(), nameC.length, resultC, lenC);
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let resultStr = ref.readCString(resultC.deref(), 0);
    scripalLib.deletePtr(resultC.deref());
    return resultStr;
  }

  toJSON() {
    clearErr();
    let resultC = Buffer.alloc(ref.sizeof.pointer);
    let lenC = Buffer.alloc(ref.sizeof.int);
    let resultBool = Boolean(scripalLib.templatesToJSON(resultC, lenC));
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let resultPtr = ref.readPointer(resultC, 0);
    let resultStr = ref.readCString(resultPtr, 0);
    scripalLib.deletePtr(resultPtr);
    return resultStr;
  }
  
  toJSONVal(names) {
    clearErr();
    let resultC = Buffer.alloc(ref.sizeof.pointer);
    let lenC = Buffer.alloc(ref.sizeof.int);
    let namesC = Buffer.from(names, 'utf8');
    let resultBool = Boolean(scripalLib.templatesToJSONVal(resultC, lenC, namesC.toString(), names.length));
    if (getErrMsg() !== "") throw new Error(getErrExp());
    let resultPtr = ref.readPointer(resultC, 0);
    let resultStr = ref.readCString(resultPtr, 0);
    scripalLib.deletePtr(resultPtr);
    return resultStr;
  }
  
  fromJSON(textJSON) {
    clearErr();
    let textJSONC = Buffer.from(textJSON, 'utf8');
    let result = Boolean(scripalLib.templatesFromJSON(textJSONC.toString(), textJSON.lenght));
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  store(file) {
    clearErr();
    let fileC = Buffer.from(file, 'utf8');
    let result = Boolean(scripalLib.templateStore(fileC.toString(), fileC.length));
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
  
  restore(file) {
    clearErr();
    let fileC = Buffer.from(file, 'utf8');
    let result = Boolean(scripalLib.templateRestore(fileC.toString(), fileC.length));
    if (getErrMsg() !== "") throw new Error(getErrExp());
    return result;
  }
}
let config = new TFConfig(); 
let templates = new TFTemplate();

module.exports = {
  config,
  templates,
  baseInit,
  getVersion,
  baseFinalize,
  libInit,
  libFinalize,
  logInit,
  getLogBuf,
  clearLogBuf,
  getErrMsg,
  getErrExp,
  getErrPos,
  clearErr,
  setEncoding,
  testError,
  TFResults,
  TFScripal,
  TFTemplate,
  TFConfig,
  // Also exporting constants
  ENC_DEFAULT,
  ENC_UTF8,
  ENC_UTF16L,
  ENC_UTF16B,
  ENC_UTF32L,
  ENC_UTF32B,
  ENC_ASCII,
  ENC_CP932,
  ENC_CP936,
  ENC_CP949,
  ENC_CP950,
  ENC_LATIN1,
  ENC_LATIN2,
  ENC_LATIN9,
  ENC_WIN874,
  ENC_WIN1250,
  ENC_WIN1251,
  ENC_WIN1252,
  ENC_WIN1253,
  ENC_WIN1254,
  ENC_WIN1255,
  ENC_WIN1256,
  ENC_WIN1257,
  ENC_WIN1258,
  PATTERN_LEVEN_WORD,
  PATTERN_LEVENPLUS_WORD,
  PATTERN_LEVEN,
  PATTERN_JARO,
  PATTERN_JAROWINKLER,
  PATTERN_JAROWINKLER_WORD,
  MATCH_SOURCE,
  MATCH_NEAREST,
  MATCH_BLOCK,
  POS_UTF8,
  POS_OFFSET,
  POS_COUNT,
  NO_POS,
  NO_VALUE
};