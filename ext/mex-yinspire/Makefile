ROOT_DIR=../..

compile: generate
	mkoctfile --mex -I${ROOT_DIR}/src -L${ROOT_DIR}/build/Release -lyinspirelib Yinspire.cc

generate:
	mkdir -p m
	ruby mex.rb m "" Yinspire_

test:
	octave -p m test.m

clean:
	rm -rf Yinspire.o Yinspire.mex
