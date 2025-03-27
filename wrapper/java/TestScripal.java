// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

// Assuming that the scripal package and its classes are available in the classpath
// Linux: call with 
// java -cp "./:scripalwrapper.jar:/usr/share/maven-repo/net/java/dev/jna/jna/debian/jna-debian.jar" TestScripal.java
// Windows: call with 
// java -cp ".;.\scripalwrapper.jar;.\jna-jpms-5.16.0.jar" TestScripal.java

import scripal.*;

public class TestScripal {
  public static void main(String[] args) throws IOException {
    String os = System.getProperty("os.name");
    System.out.println("Using System Property: " + os);

    // init functions
    String lpath = "";
    String osName = System.getProperty("os.name").toLowerCase();
    if (osName.contains("linux")) {
      lpath = "";
    }
    if (osName.contains("win")) {
      lpath = "..\\..\\win\\x64\\Release\\dll";
    }
    scripal.base.baseInit(lpath);
    scripal.base.libInit("", scripal.base.ENC_UTF8);
    scripal.base.logInit("buffer");

    // provoke error and output messages
    scripal.base.testError();
    System.out.println("getErr: " + scripal.base.getErrMsg());
    System.out.println("getErrExp: " + scripal.base.getErrExp());
    System.out.println("getErrPos: " + scripal.base.getErrPos());
    System.out.println(" ");

    // basic methods
    scripal.base.clearErr();
    scripal.base.setEncoding(scripal.base.ENC_UTF8);

    System.out.println("Version: " + scripal.base.getVersion() + "\n");

    // matches
    scripal.base.TFScripal obj = new scripal.base.TFScripal("source", "match find('testwäö,Ɣ,Ɠ,한,글,丰,丱')");
    boolean result = obj.match("Find the phrase testwäö,Ɣ,Ɠ,한,글,丰,丱.");
    System.out.println("source match result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i) + " [" + String.valueOf(obj.results.positions.get(i)[0]) + "," + String.valueOf(obj.results.positions.get(i)[1]) + "]");
    }
    System.out.println(" ");
    obj = null;

    scripal.base.clearLogBuf();
    scripal.base.config.fromJSON("{ \"debugRun\": \"true\" }");
    obj = new scripal.base.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
    result = obj.match("Find the words apple, ape and award.");
    System.out.println("log buffer: \n" + scripal.base.getLogBuf());
    System.out.println("source match result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i) + " [" + String.valueOf(obj.results.positions.get(i)[0]) + "," + String.valueOf(obj.results.positions.get(i)[1]) + "]");
    }

    System.out.println("results as CSV:");
    System.out.println(obj.getResultCSV());
    System.out.println("results as JSON:");
    System.out.println(obj.getResultJSON() + "\n");
    System.out.println("results as HRF:");
    System.out.println(obj.getResultHRF());
    obj = null;

    obj = new scripal.base.TFScripal("nearest", "test", 0.5);
    result = obj.match("Find the words like test, tests and TEST.");
    System.out.println("nearest match result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i));
    }
    System.out.println(" ");

    result = obj.matchFile("test.txt");
    System.out.println("match file result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i));
    }
    obj = null;

    System.out.println(" ");
    obj = new scripal.base.TFScripal("block", "testing", 0.5);
    result = obj.match("resting");
    System.out.println("block match result: " + result + " 'resting'");
    System.out.println(obj.results.ratings.get(0));
    result = obj.match("testings");
    System.out.println("block match result: " + result + " 'testings'");
    System.out.println(obj.results.ratings.get(0));
    result = obj.match("nesting");
    System.out.println("block match result: " + result + " 'nesting'");
    System.out.println(obj.results.ratings.get(0));
    System.out.println(" ");
    obj = null;

    obj = new scripal.base.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
    result = obj.replace("Find the words apple, ape plus award.", "___");
    System.out.println("source replace result: " + result + " " + obj.results.text.get(0));
    System.out.println(" ");
    obj = null;

    obj = new scripal.base.TFScripal("nearest", "test", 0.5);
    result = obj.replaceFile("test.txt", "testout.txt", "___");
    obj = null;

    obj = new scripal.base.TFScripal("source", "match find(any (' ', ',', '.')); ifMatch end; loop;");
    result = obj.split("Find the words apple, ape plus award.");
    System.out.println("split result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i));
    }
    System.out.println(" ");
    obj = null;

    obj = new scripal.base.TFScripal("source", "match find(any (' ', ',','.')); ifMatch end; loop;");
    result = obj.splitFile("test.txt", "testout.txt");
    BufferedReader f = new BufferedReader(new FileReader("testout.txt"));
    StringBuilder fileContent = new StringBuilder();
    String line;
    while ((line = f.readLine()) != null) {
      fileContent.append(line).append("\n");
    }
    System.out.println("split file result:\n" + fileContent.toString());
    f.close();
    obj = null;

    // file search
    obj = new scripal.base.TFScripal("nearest", "test", 0.5);
    result = obj.searchFiles("./", "*", false);
    System.out.println("file search result: " + result + " , no. of results: " + obj.results.size());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.print("'" + obj.results.fileNames.get(i) + ",'" + obj.results.text.get(i) + "', [" + obj.results.positions.get(i)[0] + "," + obj.results.positions.get(i)[1] + "], ");
      System.out.printf("%.6f\n", obj.results.ratings.get(i));
    }
    System.out.println(" ");
    obj = null;

    // config
    System.out.println("config as JSON:");
    System.out.println(scripal.base.config.toJSON());
    System.out.println(" ");

    System.out.println("config settings 'negSign' and 'encoding' as JSON:");
    System.out.println(scripal.base.config.toJSONVal("negSign,encoding"));
    System.out.println(" ");

    // print out changed neg sign +
    System.out.println("config settings 'negSign' after changing to '+':");
    scripal.base.config.fromJSON("{ \"negSign\": \"+\" }");
    System.out.println(scripal.base.config.toJSONVal("negSign"));
    System.out.println(" ");

    System.out.println("store config in conf.java, result: " + scripal.base.config.store("conf.java"));
    System.out.println(" ");

    System.out.println("restore config from conf.java, result: " + scripal.base.config.restore("conf.java"));
    System.out.println(" ");

    // templates
    scripal.base.templates.reset();

    scripal.base.templates.set("findtest", "match find('test')");

    System.out.println("store templates in templates.py, result: " + scripal.base.templates.store("templates.java"));
    System.out.println(" ");

    System.out.println("template findtest: " + scripal.base.templates.get("findtest"));
    System.out.println(" ");

    scripal.base.templates.clear("findtest");

    System.out.println("restore templates from templates.py, result: " + scripal.base.templates.restore("templates.java"));
    System.out.println(" ");

    System.out.println("templates to JSON: " + scripal.base.templates.toJSON());
    System.out.println(" ");

    System.out.println("template findtest to JSONVal: " + scripal.base.templates.toJSONVal("findtest"));
    System.out.println(" ");

    scripal.base.libFinalize();
  }
}
