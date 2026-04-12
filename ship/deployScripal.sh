#!/bin/bash

# prepare scripalweb/downloads , take files and zip them, remember to set symbolic links afterwards

# copy Windows package and Linux packages to ../../scripalweb/downloads/linux and ../win
if ! [ -d "../../scripalweb/downloads" ]; then
  echo "/scripalweb/downloads does not exist"
  exit 1
fi

# pull latest scripal
git -C ../ pull

cd ../../scripalweb/downloads

if ! [ -f "./win/scripal.exe" ]; then
  echo "cannot cd into directory downloads or Windows scripal.exe not present"
  exit 1
fi

if ! [ -f "./linux/libscripal.so" ]; then
  echo "cannot cd into directory downloads or Linux libscripal.so not present"
  exit 1
fi
cd linux
rm -f scripal-so-linux-x64.tar.xz
tar -cJf scripal-so-linux-x64.tar.xz  libscripal.so
cd ..

rm -f ./win/scripalWin.zip
rm -f ./win/scripal-dll-win32-x64.tar.xz
cp -fr ../../scripal/test ./win
rm -fr ./win/inc
cp -fr ../../scripal/inc ./win
cd win
zip -r ../scripalWin.zip .
tar -cJf ../scripal-dll-win32-x64.tar.xz scripal.dll
cd ..
rm -fr ./win/*
mv scripalWin.zip ./win
mv scripal-dll-win32-x64.tar.xz ./win

rm -rf wrapper
cp -r ../../scripal/wrapper wrapper

cd ./wrapper

cd ./c
zip -r ../scripalC.zip .
cd ..
rm -rf ./c/.*  2> /dev/null
rm -rf ./c/*
mv scripalC.zip ./c/

cd ./c++
zip -r ../scripalC++.zip .
cd ..
rm -rf ./c++/.*  2> /dev/null
rm -rf ./c++/*
mv scripalC++.zip ./c++/

cd ./cs
zip -r ../scripalCS.zip .
cd ..
rm -rf ./cs/.*  2> /dev/null
rm -rf ./cs/*
mv scripalCS.zip ./cs/

cd ./js
zip -r ../scripalJS.zip .
cd ..
rm -rf ./js/.*  2> /dev/null
rm -rf ./js/*
mv scripalJS.zip ./js/

cd ./python
zip -r ../scripalPython.zip .
cd ..
rm -rf ./python/.*  2> /dev/null
rm -rf ./python/*
mv scripalPython.zip ./python/

cd ./java
zip -r ../scripalJava.zip .
cd ..
rm -rf ./java/.*  2> /dev/null
rm -rf ./java/*
mv scripalJava.zip ./java/
