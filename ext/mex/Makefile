ROOT_DIR=../..
INSTALL_DIR=${ROOT_DIR}/build/mex

all: compile run

compile:
	mkdir -p ${INSTALL_DIR}
	ruby mex.rb ${INSTALL_DIR}
	mkoctfile --mex -I${ROOT_DIR}/src -L${ROOT_DIR}/build/Release -lyinspirelib Yinspire.cc
	cp Yinspire.mex ${INSTALL_DIR}

run:
	octave -p ${INSTALL_DIR} test.m

clean:
	rm -rf Yinspire.cc Yinspire.o Yinspire.mex
