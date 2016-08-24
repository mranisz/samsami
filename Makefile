UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
ASMLIB = libaelf64.a
else
ASMLIB = libacof64.lib
endif

CXX=g++
CFLAGS=-Wall -std=c++11 -O3 -mpopcnt
	
all: countSamSAMi locateSamSAMi

countSamSAMi: test/countSamSAMi.cpp libsamsami.a libs/$(ASMLIB)
	$(CXX) $(CFLAGS) test/countSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o test/countSamSAMi

locateSamSAMi: test/locateSamSAMi.cpp libsamsami.a libs/$(ASMLIB)
	$(CXX) $(CFLAGS) test/locateSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o test/locateSamSAMi

libsamsami.a: samsami.hpp shared/common.h shared/common.cpp shared/patterns.h shared/patterns.cpp shared/timer.h shared/timer.cpp shared/sais.h shared/sais.c shared/xxhash.h shared/xxhash.c shared/hash.hpp shared/wt.h shared/wt.cpp shared/huff.h shared/huff.cpp
	$(CXX) $(CFLAGS) -c shared/common.cpp shared/patterns.cpp shared/timer.cpp shared/sais.c shared/xxhash.c shared/wt.cpp shared/huff.cpp
	ar rcs libsamsami.a samsami.hpp common.o patterns.o timer.o sais.o xxhash.o wt.o huff.o shared/hash.hpp
	make cleanObjects

cleanObjects:
	rm -f *o

clean:
	rm -f *o test/countSamSAMi test/locateSamSAMi libsamsami.a