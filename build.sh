#!/bin/sh

#
# Flags than can be passed:
#
#   -DYINSPIRE_EXPENSIVE_RECORD=ON
#

mkdir -p build/Release
mkdir -p build/Debug
mkdir -p build/Release.Win32
mkdir -p build/Debug.Win32

verbose=""

cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release $@ ../..
make ${verbose}
cd ../..

cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug $@ ../..
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
