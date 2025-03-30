# Scripal: Starting in Java

Scripal is most efficient when using UTF-8 as base encoding in Java (standard since Java 18).
Check default encoding by calling **Charset defaultCharset()**.
Using other encodings causes some performance overhead.

## Installation

[prerequisites](/docs/InstallPrerequisites.md) <br>

### ready to use packages and module

[install packages](/docs/InstallPackage.md) <br>

### Java module
Scripal relies on https://github.com/java-native-access/jna , Java Native Access. <br>
This Library must be available.
The Java module is [base.java](/wrapper/java/base.java). It consists of only one file. <br>
See how to use Scripal in Java [TestScripal.java](/wrapper/java/TestScripal.java) <br>
Make sure to specify the correct path to the Scripal shared library when calling **scripal.base.baseInit(path)**. <br>This must be a path to the folder holding libscripal.so oder scripal.dll.
The function must be called once to initialize the library. <br>
Every single thread must call **scripal.base.libInit(config, encoding)** to specify the entire path to the config file used (leave empty for default) and the encodig of the OS environment. **scripal.base.ENC_DEFAULT** will denote the standard.  
You must specfiy the JNA jar in CLASSPATH when using Scripal.
To create classes under ./scripal folder, call javac. <br><br>
Example for **Linux**  
<pre>
javac -d . -cp "/usr/share/maven-repo/net/java/dev/jna/jna/debian/jna-debian.jar" base.java  
</pre>
Example for **Windows**
<pre>
javac -d . -cp ".\jna-jpms-5.16.0.jar" base.java
</pre>
Alternative: You may use the included Jar files under **/wrapper/java/linux** or **/wrapper/java/win**. 
Assuming that the scripal package and its classes are available in the classpath. <br><br>
**Linux:** example call with (find out the path to jna-xxx.jar)
<pre>
java -cp "./linux/scripalwrapper.jar:/usr/share/maven-repo/net/java/dev/jna/jna/debian/jna-debian.jar" TestScripal.java
</pre>
**Windows:** example call with (find out the path to jna-xxx.jar)
<pre>
java -cp ".;.\windows\scripalwrapper.jar;.\jna-jpms-5.16.0.jar" TestScripal.java
</pre> 
To avoid warnings with native access, set **--enable-native-access=ALL-UNNAMED** when calling Java.<br>
## run Java test program 
<pre>
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import scripal.*;

public class MyScripal {
  public static void main(String[] args) throws IOException {
    String os = System.getProperty("os.name");

    // init functions
    String lpath = "";
    String osName = System.getProperty("os.name").toLowerCase();
    if (osName.contains("linux")) {
      lpath = "";
    }
    if (osName.contains("win")) {
      lpath = "..\\..\\win\\x64\\Debug";
    }
    scripal.base.baseInit(lpath);
    scripal.base.libInit("", scripal.base.ENC_UTF8);
    scripal.base.logInit("stdout");


    scripal.base.TFScripal obj = new scripal.base.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;");
    boolean result = obj.match("Find the words apple, ape and award.");
    System.out.println("source match result: " + result + " , no. of results: " + obj.getResultSize());
    for (int i = 0; i < obj.results.size(); i++) {
      System.out.println(obj.results.text.get(i) + " [" + String.valueOf(obj.results.positions.get(i)[0]) + "," + String.valueOf(obj.results.positions.get(i)[1]) + "]");
    }
  }
}
</pre>

for more information see [matching with Scripal](/docs/IntroMatching.md) and [library](/docs/LibraryBase.md) 





