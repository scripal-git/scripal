# Building Scripal (Windows)

## toolchain essentials 

install Visual Studio with C++ support: <https://visualstudio.microsoft.com/downloads/>

## Git

install Git CLI: [https://github.com/cli/cli#installation]([https://github.com/cli/cli#installation)

## Poco C++ libraries (minimum version 1.11.0 required)

An easy way to install Poco libraries under Windows is to use vcpkg.

if vcpkg has to be installed, use git ( [https://git-scm.com/downloads/win](https://git-scm.com/downloads/win) )
<pre>
git clone https://github.com/Microsoft/vcpkg.git
</pre>
To compile vcpkg and generate the executable file, run the bootstrap-vcpkg.bat in the folder where we unzipped everything.

then to install Poco: 
<pre>
vcpkg install POCO
</pre>

or build Poco yourself 

see [https://docs.pocoproject.org/current/00200-GettingStarted.html](https://docs.pocoproject.org/current/00200-GettingStarted.html)

static and dynamic versions are needed

in terminal:
<pre>
git clone -b master https://github.com/pocoproject/poco.git
</pre>

You may build Poco with a Visual Studio solution provided in the Scripal project.
Read on for further instructions..


## Scripal

Scripal application:

<pre>
git clone -b main git@github.com:scripal-git/scripal.git
</pre>

To build Poco with Scripal, open **\win\Poco4Scripal.sln**

Select Debug or Release versions and static/dynamic build types.
Build with **Build** solution from main menu

To build Scripal, open **\win\scripal.sln**

To tell Scripal about your Poco Build: open Developer Powershell in Visual Studio and execute
<pre>
[Environment]::SetEnvironmentVariable("POCO_BASE", "XXX_PATH_XXX", "User")
</pre>
where XXX_PATH_XXX ist the full path to your Poco folder (see above)

Select Debug or Release versions and static/dynamic build types.
Build with **Build solution** from main menu



