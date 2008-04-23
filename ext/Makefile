all: compile run

compile:
	ruby mex.rb
run:
	octave -p build test.m

clean:
	rm -rf Yinspire.cc Yinspire.o Yinspire.mex build
