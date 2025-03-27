// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

// Scripal in C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// compiler/OS dependencies 
#ifdef __linux__
#include "../../inc/interface.hpp"
#endif
#ifdef _WIN32
#include "../../inc/interface.hpp"
#endif

int main(int argc, char **argv) {
  libInitC("", 0, SCRIPAL_ENC_UTF8);
  const char* logChannel = "buffer";
  logInit(logChannel, strlen(logChannel));

  const char* result = NULL;
  const char* result2 = NULL;
  const char* source = NULL;
  const char* text = NULL;
  int resultLen, resultLen2;
  bool resultBool;
  int intRes, objID, posStart, posEnd;

  //provoke error
  deletePtr((const char*)0x1);

  getErrMsg(&result, &resultLen);
  printf("getErr: %s\n", result);

  getErrExp(&result, &resultLen);
  printf("getErrExp: %s\n", result);

  intRes = getErrPos();
  printf("getErrPos: %d\n\n", intRes);

  // basic methods
  clearErr();
  setEncoding(SCRIPAL_ENC_UTF8);
  getVersion(&result, &resultLen);
  printf("Version: %s\n\n", result);

  // matches
  source = "match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')";
  text = "Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.";
  objID = newCodeObj(source, strlen(source));

  if (match(objID, text, strlen(text))) {
    printf("source match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      printf("'%s', [%d, %d]", result, posStart, posEnd);
    }
  }
  printf("\n\n");
  deleteObj(objID);

  clearLogBuf();
  const char* json = "{ \"debugRun\": \"true\" }";
  configFromJSON(json, strlen(json));
  source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  text = "Find the words apple, ape and award.";
  objID = newCodeObj(source, strlen(source));
  
  if (match(objID, text, strlen(text))) {
    getLogBuf(&result, &resultLen);
    printf("log buffer:\n%s", result);
  
    printf("source match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      printf("'%s', [%d, %d]\n", result, posStart, posEnd);
    }
  }
  printf("\n");
  getResultCSV(objID, &result, &resultLen);
  printf("results as CSV:\n%s\n", result);
  getResultJSON(objID, &result, &resultLen);
  printf("results as JSON:\n%s\n\n", result);
  getResultHRF(objID, &result, &resultLen);
  printf("results as HRF:\n%s\n", result);
  deleteObj(objID);

  source = "test";
  text = "Find the words like test, tests and TEST.";
  objID = newNearestObj(source, strlen(source), 0.5);
  if (match(objID, text, strlen(text))) {
    printf("nearest match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  if (matchFile(objID, "test.txt", strlen("test.txt"), SCRIPAL_ENC_UTF8)) {
    printf("match file result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  deleteObj(objID);

  source = "testing";
  objID = newBlockObj(source, strlen(source), 0.5);
  text = "resting";
  double rating;
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'resting'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  text = "testings";
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'testings'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  text = "nesting";
  resultBool = match(objID, text, strlen(text));
  printf("block match result: %s 'nesting'\n", resultBool ? "true":"false");
  getResultRating(objID, 0, &rating);
  printf("%f\n", rating);
  printf("\n");
  deleteObj(objID);

  source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  text = "Find the words apple, ape plus award.";
  objID = newCodeObj(source, strlen(source));
  resultBool = replace(objID, text, strlen(text), "___", strlen("___"));
  getResultStr(objID, 0, &result, &resultLen);
  printf("source replace result: %s '%s'\n\n" , resultBool ? "true":"false", result);
 
  resultBool = replaceFile(objID, "test.txt", strlen("test.txt"), "testout.txt", strlen("testout.txt"),  "___", strlen("___"), SCRIPAL_ENC_UTF8);

  deleteObj(objID);

  source = "match find(any (' ', ',', '.')); ifMatch end; loop;";
  text = "Find the words apple, ape plus award.";
  objID = newCodeObj(source, strlen(source));
  if (split(objID, text, strlen(text))) {
    printf("split result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      printf("'%s'\n", result);
    }
  }
  printf("\n");
  if (splitFile(objID, "test.txt", strlen("test.txt"), "testout.txt", strlen("testout.txt"), SCRIPAL_ENC_UTF8)) {
    printf("split file result: \n" );
    FILE *fptr = fopen("testout.txt", "r");  
  
    // Check if the file was opened successfully
    if (fptr != NULL) {  
        // Read and print each character from the file
        char ch;
        while ((ch = fgetc(fptr)) != EOF) {
            putchar(ch);
        }
    
        // Close the file after reading
        fclose(fptr);  
    }
  }

  printf("\n");
  deleteObj(objID);

  source = "test";
  objID = newNearestObj(source, strlen(source), 0.5);
  if (searchFiles(objID, "./", strlen("./"), "*", strlen("*"), false)) {
    printf("file search result: 1, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultFileName(objID, i, &result2, &resultLen2);
      getResultPos(objID, i, &posStart, &posEnd);
      double rating; 
      getResultRating(objID, i, &rating);
      printf("'%s', '%s', [%d, %d], %f\n", result2, result, posStart, posEnd, rating);
    }
  }
  printf("\n");
  deleteObj(objID);

  // config
  configToJSON(&result, &resultLen);
  printf("config as JSON:\n%s\n\n", result);
  
  const char* names = "negSign,encoding";
  configToJSONVal(&result, &resultLen, names, strlen(names));
  printf("config settings 'negSign' and 'encoding' as JSON:\n%s\n\n", result);

  json = "{ \"negSign\": \"+\" }";
  configFromJSON(json, strlen(json));
  names = "negSign";
  configToJSONVal(&result, &resultLen, names, strlen(names));
  printf("config settings 'negSign' after changing to '+':\n%s\n\n", result);
 
  names = "conf.c";
  resultBool = configStore(names, strlen(names));
  printf("store config in conf.c, result:%s\n\n", resultBool ? "true":"false");

  resultBool = configRestore(names, strlen(names));
  printf("restore config from conf.c, result: %s\n\n", resultBool ? "true":"false");

  // templates
  templateReset();

  names = "findtest";
  source = "match find('test')";
  templateSet(names, strlen(names), source, strlen(source));

  names = "templates.c";
  resultBool = templateStore(names, strlen(names));
  printf("store templates in templates.c, result: %s\n\n", resultBool ? "true":"false");

  names = "findtest";
  templateGet(names, strlen(names), &result, &resultLen);
  printf("template findtest: %s\n\n", result);

  templateReset();
  names = "templates.c";
  resultBool = templateRestore(names, strlen(names));
  printf("restore templates from templates.c, result: %s\n\n", resultBool ? "true":"false");
 
  resultBool = templatesToJSON(&result, &resultLen);
  printf("templates to JSON: \n%s\n\n", result);

  names = "findtest";
  resultBool = templatesToJSONVal(&result, &resultLen, names, strlen(names));
  printf("template findtest to JSONVal: %s\n\n", result);

  deletePtr(result2);
  deletePtr(result);

  libFinalize();
};

