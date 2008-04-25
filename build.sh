#!/bin/sh

#VERBOSE="VERBOSE=1"
ARGS=$@

build()
{
  mkdir -p build/$1
  cd build/$1
  cmake -DCMAKE_BUILD_TYPE=$2 $3 $4 $5 $6 $7 ${ARGS} ../..
  make ${VERBOSE}
  cd ../..
}

build Release Release
build Release.Double Release -DYINSPIRE_DOUBLE_PRECISION=ON
#build Debug Debug
#build Release.Win32 Release -DYINSPIRE_CROSSCOMPILE_MINGW=ON
#build Debug.Win32 Release -DYINSPIRE_CROSSCOMPILE_MINGW=ON
