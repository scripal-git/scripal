#!/bin/bash

## build Scripal Linux packages 

# build static versions
cmake -S . -B ../cmake-build -D CMAKE_BUILD_TYPE=Release
cmake --build ../cmake-build --target scripal a_scripal so_scripal

# set parameters
version=$(grep -Po '(?<=\bVERSION  = ")\d+(\.\d+)*' ../inc/definitions.hpp)
parms="--exclude-file ship.sh -n scripal --license BSL -v $version -m scripal@test.de -C ./build"
description1='--description "generic processor to search, match and manipulate text or text files"' 
eval "description=($description1)" 
echo $description

# remove packages
rm -f *.deb
rm -f *.rpm

# create directories for Linux distro
rm -fr ./build
mkdir -p ./build
mkdir -p ./build/usr
mkdir -p ./build/usr/bin
mkdir -p ./build/usr/lib
mkdir -p ./build/usr/lib/scripal
mkdir -p ./build/usr/lib/scripal/wrapper
mkdir -p ./build/usr/lib/scripal/test
mkdir -p ./build/usr/include
mkdir -p ./build/usr/include/scripal

# copy files
cp -f ../cmake-build/*.a ./build/usr/lib/scripal
cp -f ../cmake-build/*.so ./build/usr/lib/scripal
cp -fra ../wrapper/* ./build/usr/lib/scripal/wrapper
rm -fr ./build/usr/lib/scripal/wrapper/js/node_modules
cp -f ../scripal ./build/usr/bin
cp -f ../inc/* ./build/usr/include/scripal
cp -rf ../test/* ./build/usr/lib/scripal/test

# Ubuntu version 25_x
#fpm -s dir -t deb -p "scripal.ubuntu.25_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" --depends libpocofoundation100 \
#  --depends libpocoutil100 --depends libpocojson100 --depends libpocoencodings100 \
#  --depends libpocozip100  --depends pandoc --depends poppler-utils ./

# Ubuntu version 24_10
#fpm -s dir -t deb -p "scripal.ubuntu.24.10_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" --depends libpocofoundation100 \
#  --depends libpocoutil100 --depends libpocojson100 --depends libpocoencodings100 \
#  --depends libpocozip100  --depends pandoc --depends poppler-utils ./

# Ubuntu version 24_04 
#fpm -s dir -t deb -p "scripal.ubuntu.24.04_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" --depends libpocofoundation80t64 \
#  --depends libpocoutil80t64 --depends libpocojson80t64 --depends libpocoencodings80t64 \
#  --depends libpocozip80t64  --depends pandoc --depends poppler-utils ./

# Ubuntu version 22_x
#fpm -s dir -t deb -p "scripal.ubuntu.22_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" --depends libpocofoundation80 \
#  --depends libpocoutil80 --depends libpocojson80 --depends libpocoencodings80 \
#  --depends libpocozip80  --depends pandoc --depends poppler-utils ./

# Debian 
#fpm -s dir -t deb -p "scripal.debian.11.12_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" \
#  --depends libpocofoundation70 --depends libpocoutil70 --depends libpocojson70 --depends libpocoencodings70 \
#  --depends libpocozip70 --depends pandoc --depends poppler-utils ./

# Fedora
#fpm -s dir -t rpm -p "scripal.fedora.${version}.x84_64.rpm" -a x86_64 $parms "${description[@]}" --rpm-auto-add-directories --rpm-use-file-permissions --rpm-use-file-permissions \
#  --depends poco-foundation --depends poco-util --depends poco-json --depends poco-encodings --depends poco-zip --depends poppler-utils --depends pandoc ./

# Ubuntu 
fpm -s dir -t deb -p "scripal.ubuntu_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" \
  --depends pandoc --depends poppler-utils ./

# Debian 
fpm -s dir -t deb -p "scripal.debian_${version}_amd64.deb" -a x86_64 $parms "${description[@]}" \
  --depends pandoc --depends poppler-utils ./

# change /usr/lib/scripal to /usr/lib64/scripal on Fedora/RedHat 
mv -f ./build/usr/lib ./build/usr/lib64

# Fedora
sudo fpm -s dir -t rpm -p "scripal.fedora.${version}.x84_64.rpm" -a x86_64 $parms "${description[@]}" \
  --rpm-auto-add-directories --rpm-use-file-permissions \
  --depends poppler-utils --depends pandoc  ./
