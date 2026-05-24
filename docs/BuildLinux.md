# Building Scripal (Linux)

## toolchain essentials 

most Linux distros have preinstalled toolchains if not ->

install **build-essential** package (Ubuntu, Debian) or separately **gcc, g++, make** packages

## Git

install Git CLI: [https://github.com/cli/cli#installation](https://github.com/cli/cli#installation)

or use package 'git' of your Linux distro

## CMake

install CMake: [https://cmake.org/download/](https://cmake.org/download/) 

or use package 'cmake' of your Linux distro

## build script 

to install Poco libraries and Scripal you may call the bash script **buildLinux.sh**<br>
download script from: [https://scripal.org/downloads/linux/buildLinux.sh](https://github.com/scripal-git/scripal/blob/main/ship/buildLinux.sh) <br>

copy **buildLinux.sh** into the directory where Scripal and Poco libraries should be installed (recommendation: '/opt') and execute<br>

## or -> follow the next steps 

## Poco C++ libraries (minimum version 1.11.0 required)

static and dynamic versions are needed<br>
The Poco team strongly recommends to build the libraries as needed and avoid prebuilding packages

in console/terminal:
<pre>
git clone -b main https://github.com/pocoproject/poco.git
</pre>

**Notice for special use cases (skip for default builds and continue with building Poco):**<br>
To link statically against Scripal libraries and for easier distribution on many AMD_X64 systems, you may build position independent code.
In file poco/CMakeLists.txt add settings to build position independent code (-fPIC or -fpic)<br>
line to insert at begin of file is: **set(CMAKE_POSITION_INDEPENDENT_CODE ON)** 
<pre>
set(PROJECT_VERSION${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
</pre>
Scripal targets scripal, a_scripal and so_scripal are built with position independent code.<br>
Scripal targets scripal_d, a_scripal_d and so_scripal_d are used for standard automated builds.

**now build Poco libraries:**
<pre>
cd poco
rm -rf build

# build Poco dynamic 
cmake -H./ -B./build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DENABLE_TESTS=OFF -DENABLE_SEVENZIP=ON -DENABLE_NETSSL=OFF -DENABLE_CRYPTO=OFF -DENABLE_JWT=OFF -DENABLE_NET=OFF -DENABLE_DATA=OFF -DENABLE_DATA_MYSQL=OFF -DENABLE_DATA_SQLITE=OFF -DENABLE_DATA_POSTGRESQL=OFF -DENABLE_DATA_ODBC=OFF -DENABLE_MONGODB=OFF -DENABLE_REDIS=OFF -DENABLE_APACHECONNECTOR=OFF -DENABLE_ENCODINGS=ON -DENABLE_JSON=ON -DENABLE_ZIP=ON  -DENABLE_XML=ON          
cmake --build ./build
sudo cmake --install ./build
sudo ldconfig

# build Poco static
cmake -H./ -B./build -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF -DENABLE_SEVENZIP=ON -DENABLE_NETSSL=OFF -DENABLE_CRYPTO=OFF -DENABLE_JWT=OFF -DENABLE_NET=OFF -DENABLE_DATA=OFF -DENABLE_DATA_MYSQL=OFF -DENABLE_DATA_SQLITE=OFF -DENABLE_DATA_POSTGRESQL=OFF -DENABLE_DATA_ODBC=OFF -DENABLE_MONGODB=OFF -DENABLE_REDIS=OFF -DENABLE_APACHECONNECTOR=OFF -DENABLE_ENCODINGS=ON -DENABLE_JSON=ON -DENABLE_ZIP=ON -DENABLE_XML=ON           
cmake --build ./build
sudo cmake --install ./build
sudo ldconfig
</pre>

## Scripal

<pre>
git clone -b main https://github.com/scripal/scripal.git
</pre>

switch into scripal build directory cmake-build 
<pre>
cd scripal/cmake-build
</pre>

then call 

<pre>
cmake .
cmake --build . --target scripal_d a_scripal_d so_scripal_d 
sudo cmake --install . 
sudo ldconfig
</pre>

to build with position independent code (needs special position independent Poco build)
<pre>
cmake --build . --target scripal a_scripal so_scripal 
</pre>

to switch between release and debug configs, instead of standard configure  with 'cmake .' use

<pre>
cmake -S . -B ./ -D CMAKE_BUILD_TYPE=Debug
</pre>

or 

<pre>
cmake -S . -B ./ -D CMAKE_BUILD_TYPE=Release
</pre>

clean entire project build<br>
<pre>
cd scripal/cmake-build
cmake --build . --target clean
</pre>

Files are stored in: <br>
headers : **/usr/include/scripal**<br>
binary  : **./scripal**<br>
shared library : **/usr/lib/scripal** or  **/usr/lib64/scripal**<br>
static library : **/usr/local/lib/** or  **/usr/local/lib64**<br>
