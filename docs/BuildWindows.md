# Building Scripal (Windows)

## toolchain essentials 

install Visual Studio with C++ support: <https://visualstudio.microsoft.com/downloads/>

## Git

install Git CLI: [https://github.com/cli/cli#installation]([https://github.com/cli/cli#installation)

## Poco C++ libraries (minimum version 1.11.0 required)

in terminal:
<pre>
git clone -b master https://github.com/pocoproject/poco.git
</pre>

build Poco - static and dynamic versions are needed (only x64 and x86)
[https://docs.pocoproject.org/current/99150-WindowsPlatformNotes.html]

To tell Scripal about your Poco Build: open Developer Powershell in Visual Studio and execute
<pre>
[Environment]::SetEnvironmentVariable("POCO_BASE", "XXX_PATH_XXX", "User")
</pre>
where XXX_PATH_XXX ist the full path to your Poco folder (see above)

You can also build necessary Poco projects with a Visual Studio solution included in the Scripal project.
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

Select Debug or Release versions and static/dynamic build types.
Build with **Build solution** from main menu



