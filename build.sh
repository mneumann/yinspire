#!/bin/sh

mkdir -p build/Default
mkdir -p build/Release
mkdir -p build/Debug
mkdir -p build/Default.Win32
mkdir -p build/Release.Win32
mkdir -p build/Debug.Win32

verbose=""
#verbose="VERBOSE=1"

cd build/Default
cmake $@ ../..
make ${verbose}
cd ../..

cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release $@ ../..
make ${verbose}
cd ../..

cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug $@ ../..
make ${verbose}
cd ../..

cd build/Default.Win32
cmake -DYINSPIRE_CROSSCOMPILE_MINGW=ON $@ ../..
make ${verbose}
cd ../..

cd build/Release.Win32
cmake -DCMAKE_BUILD_TYPE=Release -DYINSPIRE_CROSSCOMPILE_MINGW=ON $@ ../..
make ${verbose}
cd ../..

cd build/Debug.Win32
cmake -DCMAKE_BUILD_TYPE=Debug -DYINSPIRE_CROSSCOMPILE_MINGW=ON $@ ../..
make ${verbose}
cd ../..
