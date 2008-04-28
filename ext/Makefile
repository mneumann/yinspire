all: compile run

compile:
	rm -rf build
	mkdir -p build
	ruby mex.rb build 
	mkoctfile --mex -I../src -L../build/Release -lyinspirelib Yinspire.cc
	cp Yinspire.mex build

run:
	octave -p build test.m

clean:
	rm -rf Yinspire.cc Yinspire.o Yinspire.mex build
