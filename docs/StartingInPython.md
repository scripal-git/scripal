# Scripal: Starting in Python3

Earlier Python versions are not supported. Scripal is most efficient when using UTF-8 as base encoding in Python3.
Check default encoding by calling **locale.getpreferredencoding()**
or set **PYTHONUTF8 = 1** to force.
Using other encodings causes some performance overhead.

## Installation

[prerequisites](/docs/InstallPrerequisites.md) <br>

### ready to use packages and module 

[install packages](/docs/InstallPackage.md) <br>

### Python module
Scripal use ctypes https://docs.python.org/3/library/ctypes.html <br>
This module must be available.

The python Scripal module is [scripal.py](/wrapper/python/scripal.py). It ist not available in pip and consists of only one file. <br>
See how to use Scripal in python [test.py](/wrapper/python/test.py) <br>
Make sure to specify the correct path to the Scripal shared library when calling **scripal.baseInit(path)**. This must be an absolute path to the folder holding libscripal.so oder scripal.dll.
The function must be called once to initialize the library. <br>
Every single thread must call **scripal.libInit(config, encoding)** to specify the entire path to the config file used (empty string for default) and the encodig of the OS environment. **scripal.ENC_DEFAULT** will denote the standard.  
 

## run python test program 
<pre>
import sys
import scripal
if sys.platform == "linux":
  lpath = ""
if sys.platform == "win32":  
  cwd = os.getcwd()
  lpath = cwd + "\\..\\..\\win\\x64\\Debug"

scripal.baseInit(lpath)
scripal.libInit("", scripal.ENC_UTF8)

obj = scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;")
result = obj.match("Find the words apple, ape and award.")
print ("source match result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.results.size()):
  print(obj.results.text[i], obj.results.positions[i])
</pre>

for more information see [matching with Scripal](/docs/IntroMatching.md) and [library](/docs/LibraryBase.md) 





