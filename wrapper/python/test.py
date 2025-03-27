# Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
# License:  BSL 1.0

import os
import sys
import scripal

# test python wrapper

# init functions
if sys.platform == "linux":
  lpath = ""
if sys.platform == "win32":  
  cwd = os.getcwd()
  lpath = cwd + "\\..\\..\\win\\x64\\Release\\dll"

scripal.baseInit(lpath)
scripal.libInit("", scripal.ENC_UTF8)
scripal.logInit("buffer")

# provoke error and output messages
scripal.testError()
print ("getErr: ", scripal.getErrMsg())
print ("getErrExp: ", scripal.getErrExp())
print ("getErrPos: ", scripal.getErrPos())
print(" ")

# basic methods
scripal.clearErr()
scripal.setEncoding(scripal.ENC_UTF8)

print("Version: ", scripal.getVersion(), "\n")

# matches
obj = scripal.TFScripal("source", "match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')")

result = obj.match("Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.")
print ("source match result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i], obj.results.positions[i])
print(" ")
del obj

scripal.config.fromJSON('{ "debugRun": "true" }')
scripal.clearLogBuf()
obj = scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;")
result = obj.match("Find the words apple, ape and award.")

print("log buffer:")
print(scripal.getLogBuf())

print ("source match result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i], obj.results.positions[i])
print ("results as CSV:") 
print (obj.getResultCSV())
print ("results as JSON:")
print (obj.getResultJSON(), "\n")
print ("results as HRF:")
print (obj.getResultHRF())
del obj

obj = scripal.TFScripal("nearest", "test", 0.5)
result = obj.match("Find the words like test, tests and TEST.")
print ("nearest match result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i])
print(" ")

result = obj.matchFile("test.txt")
print ("match file result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i])
print(" ")  
del obj

obj = scripal.TFScripal("block", "testing", 0.5)
result = obj.match("resting")
print ("block match result: ", result , " 'resting'")
print(obj.results.ratings[0])
result = obj.match("testings")
print ("block match result: ", result , " 'testings'")
print(obj.results.ratings[0])
result = obj.match("nesting")
print ("block match result: ", result , " 'nesting'")
print(obj.results.ratings[0])
print(" ")
del obj

obj = scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;")
result = obj.replace("Find the words apple, ape plus award.", "___")
print ("source replace result: ", result , obj.results.text[0])
print(" ")
del obj

obj = scripal.TFScripal("nearest", "test", 0.5)
result = obj.replaceFile("test.txt", "testout.txt", "___")
del obj

obj = scripal.TFScripal("source", "match find(any (' ', ',', '.')); ifMatch end; loop;")
result = obj.split("Find the words apple, ape plus award.")
print ("split result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i])
print(" ")
del obj

obj = scripal.TFScripal("source", "match find(any (' ', ',','.')); ifMatch end; loop;")
result = obj.splitFile("test.txt", "testout.txt")
f = open("testout.txt", "r")
print ("split file result:\n", f.read())
f.close()
del obj

# file search
obj = scripal.TFScripal("nearest", "test", 0.5)
result = obj.searchFiles("./", "*")
print ("file search result: ", result , " , no. of results: " , obj.results.size())
for i in range(0, obj.results.size()):
  print(obj.results.fileNames[i], ",'", obj.results.text[i], "',", obj.results.positions[i],",", obj.results.ratings[i])
print (" ")
del obj

# config
print ("config as JSON:")
print (scripal.config.toJSON())
print (" ")

print ("config settings 'negSign' and 'encoding' as JSON:")
print (scripal.config.toJSONVal("negSign,encoding"))
print (" ")

# print out changed neg sign +
print ("config settings 'negSign' after changing to '+':")
scripal.config.fromJSON('{ "negSign": "+" }')
print (scripal.config.toJSONVal("negSign"))
print (" ")

print ("store config in conf.py, result: ", scripal.config.store("conf.py"))
print (" ")

print ("restore config from conf.py, result: ", scripal.config.restore("conf.py"))
print (" ")

# templates
scripal.templates.reset()

scripal.templates.set("findtest", "match find('test')")

print ("store templates in templates.py, result: ", scripal.templates.store("templates.py"))
print (" ")

print("template findtest: ", scripal.templates.get("findtest"))
print (" ")

scripal.templates.clear("findtest")

print ("restore templates from templates.py, result: ", scripal.templates.restore("templates.py"))
print (" ")

print ("templates to JSON: ", scripal.templates.toJSON())
print (" ")

print ("template findtest to JSONVal: ", scripal.templates.toJSONVal("findtest"))
print (" ")

scripal.libFinalize()