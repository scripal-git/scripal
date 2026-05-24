# Packages for Linux distros and MS Windows, language support

## packages

### Ubuntu (AMD64, lastest versions)
install deb package:  [https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.ubuntu_1.0.5_amd64.deb](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.ubuntu_1.0.5_amd64.deb) <br>
headers will be under **/usr/include/scripal**, binary in **/usr/bin**, libs and others in **/usr/lib/scripal**<br>
the config file is created in **~/.config/scripal** on first startup of program or library 

### Debian (AMD64, lastest versions)
install deb package:  [https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.debian_1.0.5_amd64.deb](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.debian_1.0.5_amd64.deb) <br>
headers will be under **/usr/include/scripal**, binary in **/usr/bin**, libs and others in **/usr/lib/scripal**<br>
the config file is created in **~/.config/scripal** on first startup of program or library 

### Fedora (AMD64, lastest versions)
install deb package:  [https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.fedora.1.0.5.x84_64.rpm](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripal.fedora.1.0.5.x84_64.rpm) <br>
headers will be under **/usr/include/scripal**, binary in **/usr/bin**, libs and others in **/usr/lib64/scripal**<br>
the config file is created in **~/.config/scripal** on first startup of program or library 

### MS Windows 10 + 11, 64 bit
download zip: [https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalWin.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalWin.zip) <br>
install in preferred location<br>
zip file has the structure<br>
* inc/ .. header files<br>
*   scripal.dll<br>
*   scripal.lib<br>
*   scripal.exe<br>

Copy scripal.lib, scripal.dll and the binary to your preferred location. Remember the path to specify in language wrappers.
Example **baseInit("pathToDLL")**  

## Linux build script
To build Scripal on Linux systems, you may call [https://scripal.org/downloads/linux/buildLinux.sh][https://scripal.org/downloads/linux/buildLinux.sh](https://scripal.org/downloads/linux/buildLinux.sh) 

## language support
You may download the Scripal modules for programming languages here:

### C language
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalC.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalC.zip)

### C++ language
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalC++.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalC++.zip)

### C# language (Windows only)
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalCS.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalCS.zip)

### Python3 language
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalPython.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalPython.zip)

### JavaScript (Node.js) language
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalJS.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalJS.zip)

### Java language
[https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalJava.zip](https://github.com/scripal-git/scripal/releases/download/Scripal-1.0.5/scripalJava.zip)

