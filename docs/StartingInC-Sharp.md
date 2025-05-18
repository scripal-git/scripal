# Scripal: Starting in C#

## Installation

[prerequisites](/docs/InstallPrerequisites.md)<br>

### clean build (else skip to ready to use packages) 

[build Scripal under Linux](/docs/BuildLinux.md) <br>
[build Scripal under Windows](/docs/BuildWindows.md) <br>

### ready to use packages  

[install packages](/docs/InstallPackage.md) <br>

### C# module

To use Scripal with C# you need the C/C++ scripal.dll and the C# wrapper for it.<br>
The Scripal repository contains the solution **\scripal\wrapper\cs\ScripalWrapper.sln** with the C# wrapper and an example project.<br>
The **ScripalWrapper.csproj** project must be referenced in your own C# project and **scripal.dll** must be located in the execution directory.
 
## run C# test program scripal

C# example 

<pre>
using ScripalWrapper;  // Assuming the scripal library is available as a .NET assembly with the same namespace

Scripal.baseInit(Environment.CurrentDirectory);
Scripal.libInit("", Scripal.ENC_UTF8);

ScripalWrapper.TFScripal obj = new ScripalWrapper.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
var result = obj.match("Find the words apple, ape and award.");
Console.WriteLine("source match result: " + result + " , no. of results: " + obj.getResultSize());

for (int i = 0; i < obj.results.size(); i++)
{
    Console.WriteLine(obj.results.text[i] + "; position:" + obj.results.positions[i][0] + "-" +  obj.results.positions[i][1]);
}

Console.ReadKey();
</pre>


For more information see [matching with Scripal](/docs/IntroMatching.md) and [library](/docs/LibraryBase.md) <br>





