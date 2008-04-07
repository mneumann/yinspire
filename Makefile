CFLAGS=-Isrc -O3 -DYINSPIRE__EXPENSIVE_RECORD

yinspire: src/Core/*.cc src/Loaders/*.cc src/*.cc
	${CXX} ${CFLAGS} -o yinspire $>

clean: 
	rm -f yinspire
