# Building Scripal (Windows)

## toolchain essentials 

install Visual Studio with C++ support: <https://visualstudio.microsoft.com/downloads/>

## Git

install Git CLI: [https://git-scm.com/downloads/win]([https://git-scm.com/downloads/win)

## Prepare Scripal

Scripal application:

in terminal, clone the Scripal repository:
<pre>
git clone -b main git@github.com:scripal-git/scripal.git
</pre>
Make sure, the system path contains the directory, holding **git.exe**. 

## Poco C++ libraries (minimum version 1.11.0 required)

in terminal, clone the Poco repository:
<pre>
git clone -b master https://github.com/pocoproject/poco.git
</pre>

If you are already using Poco, the following libraries are needed:

**PocoFoundation, PocoEncodings, PocoUtil, PocoJSON, PocoXML, PocoZip**

We recommend to build Poco libraries using our supplied Visual Studio solution as part of the Scripal project.<br>
In the Scripal repository, open **\win\Poco4Scripal.sln** and build the relase or debug version. Choose dynamic and static builds.  
Scripal runs only on x64 and x86. 

For more information on building Poco, see:
[https://docs.pocoproject.org/current/99150-WindowsPlatformNotes.html]

## Scripal

To build Scripal, open the Visual Studio solution **\win\scripal.sln**.<br>
Select debug or release versions and static/dynamic build types.

To tell Scripal about your Poco Build: open Developer Powershell in Visual Studio and execute
<pre>
[Environment]::SetEnvironmentVariable("POCO_BASE", "XXX_PATH_XXX", "User")
</pre>
where XXX_PATH_XXX ist the full path to your Poco folder (see above)

The solution holds 3 projects: <br>
**scripal-cli**: build the Scripal console tool scripal.exe in **.\x64\Debug\cli** and **.\x64\Release\cli** respectively<br> 
**scripal-dll**: build the Scripal DLL in **.\x64\Debug\dll** and **.\x64\Release\dll** respectively<br> 
**scripal-lib**: build the static library scripal.lib in **.\x64\Debug\lib** and **.\x64\Release\lib** respectively<br> 


