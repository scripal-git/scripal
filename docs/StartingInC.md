# Scripal: Starting in C

## Installation

[prerequisites](/docs/InstallPrerequisites.md) <br>

### ready to use packages  

[install packages](/docs/InstallPackage.md) <br>

### C module

See how to use Scripal in C [main.cpp](/wrapper/c/main.cpp). <br>
Every single thread must call **initLibraryC(config, encoding)** to specify the entire path to the config file used (leave empty for default) and the encodig of the OS environment. **ENC_DEFAULT** will denote the standard.<br> 
For compile options see below.<br>
All functions returning results in char** parameters will allocate and reallocate the necessary memory. Use NULL on first call.<br>

Example:<br> 
**bool getResultStr(int aObjID, int aIndex, const char\*\* aResult, int\* aResultLen);** <br>
takes **aResult** as a parameter and sets a pointer to the allocated memory in the reference.
The length of the result ist returned in resultLen.<br>
Let **char\*\* aResult** point to NULL on the first call. If the memory ist to be freed, use **deletePtr(aResult)**.<br>
All memory ist handled by internal buffers in the Scripal library, just calling **free(aResult)** would cause memory leaks!
Always use **deletePtr()** instead of **free()** for those memory chunks returned in char** pointers!<br>
C programs must call **libInitC()** not **libInit()** in order to use the same pointer for memory reallocation.

## run C test program scripal

C example main.c

<pre>
#include "../../inc/interface.hpp"

int main(int argc, char **argv) {
  libInitC("", 0, SCRIPAL_ENC_UTF8);

  const char* result = NULL;
  const char* source = NULL;
  const char* text = NULL;

  int resultLen;
  int objID, posStart, posEnd;

  // basic methods
  clearErr();
  getVersion(&result, &resultLen);
  printf("Version: %s\n\n", result);

  // matches
  source = "match find(bow 'a'); ifMatch matchEnd find(eow); loop;";
  text = "Find the words apple, ape and award.";
  objID = newCodeObj(source, strlen(source));

  if (match(objID, text, strlen(text))) {
    printf("source match result: true, no. of results: %d\n" , getResultSize(objID));
    for (int i = 0; i < getResultSize(objID); ++i) {
      getResultStr(objID, i, &result, &resultLen);
      getResultPos(objID, i, &posStart, &posEnd);
      printf("'%s', [%d, %d]\n", result, posStart, posEnd);
    }
  }
  printf("\n\n");
  deleteObj(objID);
  deletePtr(result);
}  
</pre>

Compile and link against Scripal library using gcc or Microsoft MSVC, other compilers have not been tested.

Built example using gcc: 

<pre>
gcc main.c -Wall -o scripalTest -I. -lscripal
</pre>

calling scripalTest should result in something like:

<pre>
Version: 0.2.3

source match result: true, no. of results: 4
'apple', [15, 19]
'ape', [22, 24]
'and', [26, 28]
'award', [30, 34]
</pre>

Also look at the Visual Studio Code (Linux) or Visual Studio (Windows) projects in ./wrapper/c.

**under Windows**:<br>
To tell Scripal about your Poco Build: open Developer Powershell in Visual Studio and execute
<pre>
[Environment]::SetEnvironmentVariable("POCO_BASE", "XXX_PATH_XXX", "User")
</pre>
where XXX_PATH_XXX ist the full path to your Poco folder

For more information see [matching with Scripal](/docs/IntroMatching.md) and [C library](/docs/LibraryC.md).
<br>


