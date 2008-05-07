ROOT_DIR=../..

generate:
	mkdir -p m
	ruby mex.rb m "" Yinspire_

compile:
	mkoctfile --mex -I${ROOT_DIR}/src -L${ROOT_DIR}/build/Release -lyinspirelib Yinspire.cc

test:
	octave -p m test.m

clean:
	rm -rf Yinspire.o Yinspire.mex
