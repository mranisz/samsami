UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
ASMLIB = libaelf64.a
else
ASMLIB = libacof64.lib
endif

CXX=g++
CFLAGS=-Wall -std=c++11 -O3
	
all: testSamSAMi

testSamSAMi: testSamSAMi.cpp libfmdummy.a libs/libaelf64.a
	$(CXX) $(CFLAGS) testSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o testSamSAMi

libsamsami.a: samsami.h samsami.cpp shared/common.h shared/common.cpp shared/patterns.h shared/patterns.cpp shared/sais.h shared/sais.c shared/timer.h shared/timer.cpp shared/xxhash.h shared/xxhash.c shared/hash.h shared/hash.cpp
	$(CXX) $(CFLAGS) -c samsami.cpp shared/common.cpp shared/patterns.cpp shared/sais.c shared/timer.cpp shared/xxhash.c shared/hash.cpp
	ar rcs libsamsami.a samsami.o common.o patterns.o sais.o timer.o xxhash.o hash.o
	make cleanObjects

cleanObjects:
	rm -f *o

clean:
	rm -f *o testSamSAMi libsamsami.a