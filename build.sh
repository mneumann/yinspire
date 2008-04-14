#!/bin/sh

#
# Flags than can be passed:
#
#   -DYINSPIRE_EXPENSIVE_RECORD=ON
#

mkdir -p build/Release
mkdir -p build/Release.Win32

cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release $@ ../..
make VERBOSE=1

cd ../..

cd build/Release.Win32
cmake -DCMAKE_BUILD_TYPE=Release -DYINSPIRE_CROSSCOMPILE_MINGW=ON $@ ../..
make VERBOSE=1

cd ../..
