UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
ASMLIB = libaelf64.a
else
ASMLIB = libacof64.lib
endif

CXX=g++
CFLAGS=-Wall -std=c++11 -O3 -mpopcnt
	
all: testSamSAMi

testSamSAMi: testSamSAMi.cpp libsamsami.a libs/$(ASMLIB)
	$(CXX) $(CFLAGS) testSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o testSamSAMi

libsamsami.a: samsami.h samsami.cpp shared/common.h shared/common.cpp shared/patterns.h shared/patterns.cpp shared/timer.h shared/timer.cpp shared/sais.h shared/sais.c shared/xxhash.h shared/xxhash.c shared/hash.h shared/hash.cpp shared/wt.h shared/wt.cpp shared/huff.h shared/huff.cpp
	$(CXX) $(CFLAGS) -c samsami.cpp shared/common.cpp shared/patterns.cpp shared/timer.cpp shared/sais.c shared/xxhash.c shared/hash.cpp shared/wt.cpp shared/huff.cpp
	ar rcs libsamsami.a samsami.o common.o patterns.o timer.o sais.o xxhash.o hash.o wt.o huff.o
	make cleanObjects

cleanObjects:
	rm -f *o

clean:
	rm -f *o testSamSAMi libsamsami.a