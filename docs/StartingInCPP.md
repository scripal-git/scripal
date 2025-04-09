# Scripal: Starting in C++

## Installation

[prerequisites](/docs/InstallPrerequisites.md)<br>

### clean build (else skip to ready to use packages) 

[build Scripal under Linux](/docs/BuildLinux.md) <br>
[build Scripal under Windows](/docs/BuildWindows.md) <br>

### ready to use packages  

[install packages](/docs/InstallPackage.md) <br>

### C++ module
See how to use Scripal in C++ [main.cpp](/wrapper/c++/main.cpp). <br>
Every single thread must call **initLibrary(config, encoding)** to specify the entire path to the config file used (leave empty for default) and the encodig of the OS environment. **ENC_DEFAULT** will denote the standard. 
For compile options see below.
 
## run C++ test program scripal

C++ example main.cpp

<pre>
#include &lt;stdio.h&gt; 
#include "scripal/scripalLib.hpp"

int main(int argc, char **argv) {
  using namespace Scripal;
  initLibrary(""); // put path to your own config file here, "" = use default

  std::cout << "version: " << Scripal::VERSION << "\n\n";

  // set time measurement on
  getConfig()->set("measureTime", true);

  // scripal object
  auto obj = new TFScripal("match find('235')");;
  if (obj->match("(234) 235-5678")) {
    std::cout << "results: " << obj->results.size() << " , '" << \
    obj->results.text[0] << "'" << "  position: " << obj->results.positions[0][0] << \
    ", " << obj->results.positions[0][1] << "\n\n";
  }
  exit(0);

};
</pre>

Compile and link against Scripal library using gcc/g++ or Microsoft MSVC, other compilers have not been tested.
Make sure that at least C++17 standard is supported.

Built example using g++: 

<pre>
g++ -c "main.cpp" -std=c++17 -Wall -o main.cpp.o -I.
g++ -o scripalTest main.cpp.o -lscripal 
</pre>

calling scripalTest should result in something like:

<pre>
version: 01.02.0

results: 1 , '235'  position: 6, 8
</pre>

Also look at the Visual Studio Code (Linux) or Visual Studio (Windows) projects in ./wrapper/c++.<br>

**under Windows**:<br>
To tell Scripal about your Poco Build: open Developer Powershell in Visual Studio and execute
<pre>
[Environment]::SetEnvironmentVariable("POCO_BASE", "XXX_PATH_XXX", "User")
</pre>
where XXX_PATH_XXX ist the full path to your Poco folder 

For more information see [matching with Scripal](/docs/IntroMatching.md) and [C++ library](/docs/LibraryCPP.md).<br>





