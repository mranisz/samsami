# SamSAMi text indexes library

##What is it?
The SamSAMi text indexes are ...

The current version handles only the count query (i.e., returns the number of occurrences of the given pattern).

##Requirements
The SamSAMi text indexes require:
- C++11 ready compiler such as g++ version 4.7 or higher
- a 64-bit operating system
- text size is limited to:
    - 4GB for SamSAMi1
    - 256MB for SamSAMi2

##Installation
To download and build the library use the following commands:
```
git clone https://github.com/mranisz/samsami.git
cd samsami
make
```

##Usage
To use the SamSAMi library:
- include "samsami/samsami.h" to your project
- compile it with "-std=c++11 -O3" options and link it with libraries:
  - samsami/libsamsami.a
  - samsami/libs/libaelf64.a (linux) or samsami/libs/libacof64.lib (windows)
- use "samsami" namespace

##API
There are several functions you can call on each of the SamSAMi text index:
- **build** the index using the text:
```
void build(unsigned char* text, unsigned int textLen);
```
- **save** the index to file called fileName:
```
void save(const char *fileName);
```
- **load** the index from file called fileName:
```
void load(const char *fileName);
```
- **free** memory occupied by index:
```
void free();
```
- get the **index size** in bytes (size in memory):
```
unsigned int getIndexSize();
```
- get the size in bytes of the text used to build the index:
```
unsigned int getTextSize();
```
- get the result of **count** query:
```
unsigned int count(unsigned char *pattern, unsigned int patternLen);
```
- set **verbose** mode:
```
void setVerbose(bool verbose);
```

##SamSAMi1

Parameters:
- indexType:
      - SamSAMi1::TYPE_STANDARD (default)
      - SamSAMi1::TYPE_SKETCHES
- q - window length (default: q = 4)
- p - minimizer length, p <= q (default: p = 1)

Limitations: 
- pattern length >= q

Constructors:
```
SamSAMi1();
SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p);
```

##SamSAMi1-hash
SamSAMi1-hash is SamSAMi1 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- indexType:
      - SamSAMi1::TYPE_STANDARD (default)
      - SamSAMi1::TYPE_SKETCHES
- q - window length (default: q = 4)
- p - minimizer length, p <= q (default: p = 1)
- hash type:
      - HT::TYPE_BASIC - 
      - HT::TYPE_DENSE - 
- k - length of prefixes of suffixes from suffix array (k ≥ 2)
- loadFactor - hash table load factor (0.0 < loadFactor < 1.0)

Limitations: 
- pattern length >= max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)

Constructors:
```
SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p, HT::HTType hTType, unsigned int k, double loadFactor);
```

##SamSAMi2

Parameters:
- indexType:
      - SamSAMi2::TYPE_STANDARD (default)
      - SamSAMi2::TYPE_SKETCHES
- q - window length (default: q = 4)
- p - minimizer length, p <= q (default: p = 1)

Limitations: 
- pattern length >= q

Constructors:
```
SamSAMi2();
SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p);
```

##SamSAMi2-hash
SamSAMi2-hash is SamSAMi2 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- indexType:
      - SamSAMi2::TYPE_STANDARD (default)
      - SamSAMi2::TYPE_SKETCHES
- q - window length (default: q = 4)
- p - minimizer length, p <= q (default: p = 1)
- hash type:
      - HT::TYPE_BASIC - 
      - HT::TYPE_DENSE - 
- k - length of prefixes of suffixes from suffix array (k ≥ 2)
- loadFactor - hash table load factor (0.0 < loadFactor < 1.0)

Limitations: 
- pattern length >= max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)

Constructors:
```
SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p, HT::HTType hTType, unsigned int k, double loadFactor);
```

##SamSAMi1 usage example
```
#include <iostream>
#include <stdlib.h>
#include "samsami/shared/common.h"
#include "samsami/shared/patterns.h"
#include "samsami/samsami.h"

using namespace std;
using namespace samsami;

int main(int argc, char *argv[]) {

	unsigned int queriesNum = 1000000;
	unsigned int patternLen = 20;
	unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi1 *samSAMi1;
	const char *textFileName = "english";
	const char *indexFileName = "english-samsami1.idx";

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1::TYPE_SKETCHES, 6, 2);
		samSAMi1->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMi1->build(text, textLen);
		samSAMi1->save(indexFileName);
	}

	double indexSize = (double)samSAMi1->getIndexSize();
	cout << "Index size: " << indexSize << "B (" << (indexSize / (double)samSAMi1->getTextSize()) << "n)" << endl << endl;

	Patterns *P = new Patterns(textFileName, queriesNum, patternLen);
	unsigned char **patterns = P->getPatterns();

	for (unsigned int i = 0; i < queriesNum; ++i) {
		cout << "Pattern |" << patterns[i] << "| occurs " << samSAMi1->count(patterns[i], patternLen) << " times." << endl;
	}

	if (text != NULL) delete[] text;
	delete samSAMi1;
	delete P;
}
```
Using other SamSAMi indexes is analogous.

##External resources used in SamSAMi project
- Suffix array building by Yuta Mori (sais)
- A multi-platform library of highly optimized functions for C and C++ by Agner Fog (asmlib)
- A very fast hash function by Yann Collet (xxHash)

##Authors
- Szymon Grabowski
- [Marcin Raniszewski](https://github.com/mranisz)