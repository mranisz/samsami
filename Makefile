UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
ASMLIB = libaelf64.a
else
ASMLIB = libacof64.lib
endif

CXX=g++
CFLAGS=-Wall -faligned-new -std=c++11 -O3 -mpopcnt
	
all: countSamSAMi locateSamSAMi

countSamSAMi: test/countSamSAMi.cpp libsamsami.a libs/$(ASMLIB)
	$(CXX) $(CFLAGS) test/countSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o test/countSamSAMi

locateSamSAMi: test/locateSamSAMi.cpp libsamsami.a libs/$(ASMLIB)
	$(CXX) $(CFLAGS) test/locateSamSAMi.cpp libsamsami.a libs/$(ASMLIB) -o test/locateSamSAMi

libsamsami.a: samsami.hpp shared/common.hpp shared/patterns.hpp shared/timer.hpp shared/sais.h shared/sais.c shared/xxhash.h shared/xxhash.c shared/hash.hpp shared/rank.hpp shared/wt.hpp shared/fm.hpp shared/huff.hpp
	$(CXX) $(CFLAGS) -c shared/sais.c shared/xxhash.c
	ar rcs libsamsami.a samsami.hpp sais.o xxhash.o shared/common.hpp shared/patterns.hpp shared/timer.hpp shared/hash.hpp shared/rank.hpp shared/wt.hpp shared/fm.hpp shared/huff.hpp
	make cleanObjects

cleanObjects:
	rm -f *o

clean:
	rm -f *o test/countSamSAMi test/locateSamSAMi libsamsami.a