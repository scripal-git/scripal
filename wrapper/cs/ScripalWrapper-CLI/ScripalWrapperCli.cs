// Copyright © 2025 Oliver Flick + Miroslaw Kuziemski  
// License:  BSL 1.0

// Scripal in C#
using System;
using System.Runtime.InteropServices;
using System.IO;
using ScripalWrapper;  // Assuming the scripal library is available as a .NET assembly with the same namespace

namespace ScripalWrapper_CLI
{
    class ScripalWrapperCli
    {
        static void Main(string[] args)
        {

            // init functions
            string lpath = "";
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                lpath = "";
            }
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                lpath = Environment.CurrentDirectory; 
            }
            Scripal.baseInit(lpath);
            Scripal.libInit("", Scripal.ENC_UTF8);
            Scripal.logInit("stdout");

            // provoke error and output messages
            Scripal.testError();
            Console.WriteLine("getErr: " + Scripal.getErrMsg());
            Console.WriteLine("getErrExp: " + Scripal.getErrExp());
            Console.WriteLine("getErrPos: " + Scripal.getErrPos());
            Console.WriteLine(" ");

            // basic methods
            Scripal.clearErr();
            Scripal.setEncoding(Scripal.ENC_UTF8);

            Console.WriteLine("Version: " + Scripal.getVersion() + "\n");

            // matches
            ScripalWrapper.TFScripal obj = new ScripalWrapper.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
            var result = obj.match("Find the words apple, ape and award.");
            Console.WriteLine("source match result: " + result + " , no. of results: " + obj.getResultSize());
            for (int i = 0; i < obj.results.size(); i++)
            {
                Console.WriteLine(obj.results.text[i] + " " + obj.results.positions[i]);
            }
            Console.WriteLine(" ");
            Console.WriteLine("results as CSV:");
            Console.WriteLine(obj.getResultCSV());
            Console.WriteLine("results as JSON:");
            Console.WriteLine(obj.getResultJSON() + "\n");
            Console.WriteLine("results as HRF:");
            Console.WriteLine(obj.getResultHRF());

            obj = new ScripalWrapper.TFScripal("nearest", "test", 0.5);
            result = obj.match("Find the words like test, tests and TEST.");
            Console.WriteLine("nearest match result: " + result + " , no. of results: " + obj.getResultSize());
            for (int i = 0; i < obj.results.size(); i++)
            {
                Console.WriteLine(obj.results.text[i]);
            }
            Console.WriteLine(" ");

            result = obj.matchFile("test.txt");
            Console.WriteLine("match file result: " + result + " , no. of results: " + obj.getResultSize());
            for (int i = 0; i < obj.results.size(); i++)
            {
                Console.WriteLine(obj.results.text[i]);
            }

            obj = new ScripalWrapper.TFScripal("block", "testing", 0.5);
            result = obj.match("resting");
            Console.WriteLine("block match result: " + result + " 'resting'");
            Console.WriteLine(obj.results.ratings[0]);
            result = obj.match("testings");
            Console.WriteLine("block match result: " + result + " 'testings'");
            Console.WriteLine(obj.results.ratings[0]);
            result = obj.match("nesting");
            Console.WriteLine("block match result: " + result + " 'nesting'");
            Console.WriteLine(obj.results.ratings[0]);
            Console.WriteLine(" ");

            obj = new ScripalWrapper.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
            result = obj.replace("Find the words apple, ape plus award.", "___");
            Console.WriteLine("source replace result: " + result + " " + obj.results.text[0]);
            Console.WriteLine(" ");

            obj = new ScripalWrapper.TFScripal("nearest", "test", 0.5);
            result = obj.replaceFile("test.txt", "testout.txt", "___");

            obj = new ScripalWrapper.TFScripal("source", "match find(any (' ', ',', '.')); ifMatch end; loop;");
            result = obj.split("Find the words apple, ape plus award.");
            Console.WriteLine("split result: " + result + " , no. of results: " + obj.getResultSize());
            for (int i = 0; i < obj.results.size(); i++)
            {
                Console.WriteLine(obj.results.text[i]);
            }
            Console.WriteLine(" ");

            obj = new ScripalWrapper.TFScripal("source", "match find(any (' ', ',','.')); ifMatch end; loop;");
            result = obj.splitFile("test.txt", "testout.txt");
            string f = File.ReadAllText("testout.txt");
            Console.WriteLine("split file result:\n" + f);

            // file search
            obj = new ScripalWrapper.TFScripal("nearest", "test", 0.5);
            result = obj.searchFiles("./", "*");
            Console.WriteLine("file search result: " + result + " , no. of results: " + obj.results.size());
            for (int i = 0; i < obj.results.size(); i++)
            {
                Console.WriteLine(obj.results.fileNames[i] + ",'" + obj.results.text[i] + "'," + obj.results.positions[i] + "," + obj.results.ratings[i]);
            }
            Console.WriteLine(" ");

            // config
            Console.WriteLine("config as JSON:");
            Console.WriteLine(Scripal.config.toJSON());
            Console.WriteLine(" ");

            Console.WriteLine("config settings 'negSign' and 'encoding' as JSON:");
            Console.WriteLine(Scripal.config.toJSONVal("negSign,encoding"));
            Console.WriteLine(" ");

            // print out changed neg sign +
            Console.WriteLine("config settings 'negSign' after changing to '+':");
            Scripal.config.fromJSON("{ \"negSign\": \"+\" }");
            Console.WriteLine(Scripal.config.toJSONVal("negSign"));
            Console.WriteLine(" ");

            Console.WriteLine("store config in conf.py, result: " + Scripal.config.store("conf.py"));
            Console.WriteLine(" ");

            Console.WriteLine("restore config from conf.py, result: " + Scripal.config.restore("conf.py"));
            Console.WriteLine(" ");

            // templates
            Scripal.templates.reset();

            Scripal.templates.set("findtest", "match find('test')");

            //Console.WriteLine("store templates in templates.py, result: " + Scripal.templates.store("templates.py"));
            //Console.WriteLine(" ");

            Console.WriteLine("template findtest: " + Scripal.templates.get("findtest"));
            Console.WriteLine(" ");

            Scripal.templates.clear("findtest");

            //Console.WriteLine("restore templates from templates.py, result: " + Scripal.templates.restore("templates.py"));
            //Console.WriteLine(" ");

            //Console.WriteLine("templates to JSON: " + Scripal.templates.toJSON());
            //Console.WriteLine(" ");

            //Console.WriteLine("template findtest to JSONVal: " + Scripal.templates.toJSONVal("findtest"));
            //Console.WriteLine(" ");
        }
    }
}

