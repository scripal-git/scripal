#!/bin/bash
set -e

###############################################################
# install Poco libraries, scripal binary and libs under Linux

if [ -f "./ship.sh" ]
then
  echo "wrong directory to execute script"
  exit 1
fi

# test for installed packages

if ! command -v gcc &> /dev/null
then
    echo "error: package <gcc> could not be found"
    exit 1
fi

if ! command -v g++ &> /dev/null
then
    echo "error: package <g++> could not be found"
    exit 1
fi

if ! command -v make &> /dev/null
then
    echo "error: package <make> could not be found"
    exit 1
fi

if ! command -v cmake &> /dev/null
then
    echo "error: package <cmake> could not be found"
    exit 1
fi

if ! command -v git &> /dev/null
then
    echo "error: package <git> could not be found"
    exit 1
fi

if ! command -v pdftotext &> /dev/null
then
    echo "warning: package <poppler-utils> could not be found, needed for PDF file scans"
fi

if ! command -v pandoc /? &> /dev/null
then
    echo "warning: package <pandoc> could not be found, needed for docx file scans"
fi

# install and build Poco library 
if [ ! -e "./poco" ] 
then
echo clone Poco library
git clone -b main https://github.com/pocoproject/poco.git

echo build Poco library
cd poco
rm -rf build

# build Poco dynamic 
cmake -H./ -B./build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DENABLE_TESTS=OFF -DENABLE_SEVENZIP=ON -DENABLE_NETSSL=OFF -DENABLE_CRYPTO=OFF -DENABLE_JWT=OFF -DENABLE_NET=OFF -DENABLE_DATA=OFF -DENABLE_DATA_MYSQL=OFF -DENABLE_DATA_SQLITE=OFF -DENABLE_DATA_POSTGRESQL=OFF -DENABLE_DATA_ODBC=OFF -DENABLE_MONGODB=OFF -DENABLE_REDIS=OFF -DENABLE_APACHECONNECTOR=OFF -DENABLE_ENCODINGS=ON -DENABLE_JSON=ON -DENABLE_ZIP=ON  -DENABLE_XML=ON          
cmake --build ./build
sudo cmake --install ./build 
sudo ldconfig

# build Poco static
cmake -H./ -B./build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF -DENABLE_SEVENZIP=ON -DENABLE_NETSSL=OFF -DENABLE_CRYPTO=OFF -DENABLE_JWT=OFF -DENABLE_NET=OFF -DENABLE_DATA=OFF -DENABLE_DATA_MYSQL=OFF -DENABLE_DATA_SQLITE=OFF -DENABLE_DATA_POSTGRESQL=OFF -DENABLE_DATA_ODBC=OFF -DENABLE_MONGODB=OFF -DENABLE_REDIS=OFF -DENABLE_APACHECONNECTOR=OFF -DENABLE_ENCODINGS=ON -DENABLE_JSON=ON -DENABLE_ZIP=ON -DENABLE_XML=ON 
cmake --build ./build
sudo cmake --install ./build 
cd ..
fi

if [ ! -e "./scripal" ]
then
echo clone scripal
git clone -b master https://github.com/scripal/scripal.git
else
cd scripal
git pull -f
cd ..
fi

cd scripal/cmake-build
cmake --build . --target clean
cmake .
cmake --build . --target scripal_d a_scripal_d so_scripal_d
sudo cmake --install .
sudo ldconfig

cd ../..

if [ -e "/usr/lib/scripal" ]
then
sudo cp -fr ./scripal/test /usr/lib/scripal
fi

if [ -e "/usr/lib64/scripal" ]
then
sudo cp -fr ./scripal/test /usr/lib64/scripal
fi

printf "\n***** Scripal installed *****\n"
