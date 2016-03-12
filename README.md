# SamSAMi text indexes library

##What is it?
The SamSAMi text indexes are variations of the sampled suffix array, which uses only a subset of the offsets in the suffix array yet allows to search for a pattern relatively fast. The novelty of the SamSAMi (Sampled Suffix Array with Minimizers) idea is to select the offset subset in such a way that allows to search for the pattern with only a single binary search over the suffixes (followed with verifications).

SamSAMi1 uses 5n bytes of RAM in the worst case (which is however unlikely). This includes 1n bytes for the input text and 4n bytes for the index. Typically its memory use depends on the parameters q, p and on average the required memory is about 1n+8/(q-p+2)n bytes.

The current version handles only the count query (i.e., returns the number of occurrences of the given pattern).

##Requirements
The SamSAMi text indexes require:
- C++11 ready compiler such as g++ version 4.7 or higher
- a 64-bit operating system
- text size is limited to:
    - 4GB for SamSAMi1 and SamSAMiFM
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
      - SamSAMi1::STANDARD (default)
      - SamSAMi1::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMi1::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length (default: q = 4)
- p - minimizer length, p ≤ q (default: p = 1)

Limitations: 
- pattern length ≥ q

Constructors:
```
SamSAMi1();
SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p);
```

##SamSAMi1-hash
SamSAMi1-hash is SamSAMi1 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- indexType:
      - SamSAMi1::STANDARD
      - SamSAMi1::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMi1::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length
- p - minimizer length, p ≤ q
- hash type:
      - HT::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HT::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array (k ≥ 2)
- loadFactor - hash table load factor (0.0 < loadFactor < 1.0)

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)

Constructors:
```
SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p, HT::HTType hTType, unsigned int k, double loadFactor);
```

##SamSAMi2
To speed up searches, SamSAMi2 stores some extra data on 4 most significant bits in each SamSAMi offset (this is the reason for which the longest indexed text for SamSAMi2 is limited in our implementation to 256 MB only).

Parameters:
- indexType:
      - SamSAMi2::STANDARD (default)
      - SamSAMi2::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMi2::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length (default: q = 4)
- p - minimizer length, p ≤ q (default: p = 1)

Limitations: 
- pattern length ≥ q

Constructors:
```
SamSAMi2();
SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p);
```

##SamSAMi2-hash
SamSAMi2-hash is SamSAMi2 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- indexType:
      - SamSAMi2::STANDARD
      - SamSAMi2::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMi2::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length
- p - minimizer length, p ≤ q
- hash type:
      - HTSamSAMi2::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HTSamSAMi2::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array (k ≥ 2)
- loadFactor - hash table load factor (0.0 < loadFactor < 1.0)

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)

Constructors:
```
SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p, HTSamSAMi2::HTType hTType, unsigned int k, double loadFactor);
```

##SamSAMi-FM
SamSAMi-FM is a hybrid of SamSAMi and FM. To speed up the verification phase (which is costly for short patterns in standard SamSAMi indexes), the FM index (employing a binary Huffman-shaped wavelet tree) is used.

Parameters:
- indexType:
      - SamSAMiFM::TYPE_512 (default) - using 512b blocks in WT: 64b of rank data and 448b of encoded text data
      - SamSAMiFM::TYPE_1024 - using 1024b blocks in WT: 64b of rank data and 960b of encoded text data
- q - window length (default: q = 4)
- p - minimizer length, p ≤ q (default: p = 1)
- l - sampling parameter for mapping between SamSAMi and SA indexes, larger l reduces the space somewhat but also makes the search somewhat faster (default: l = 16)

Limitations: 
- pattern length ≥ q

Constructors:
```
SamSAMiFM();
SamSAMiFM(SamSAMiFM::IndexType indexType, unsigned int q, unsigned int p, unsigned int l);
```

##SamSAMi-FM-hash
SamSAMi-FM-hash is SamSAMi-FM with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- indexType:
      - SamSAMiFM::TYPE_512 - using 512b blocks in WT: 64b of rank data and 448b of encoded text data
      - SamSAMiFM::TYPE_1024 - using 1024b blocks in WT: 64b of rank data and 960b of encoded text data
- q - window length
- p - minimizer length, p ≤ q
- l - sampling parameter for mapping between SamSAMi and SA indexes, larger l reduces the space somewhat but also makes the search somewhat faster
- hash type:
      - HT::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HT::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array (k ≥ 2)
- loadFactor - hash table load factor (0.0 < loadFactor < 1.0)

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)

Constructors:
```
SamSAMiFM(SamSAMiFM::IndexType indexType, unsigned int q, unsigned int p, unsigned int l, HT::HTType hTType, unsigned int k, double loadFactor);
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
		samSAMi1 = new SamSAMi1(SamSAMi1::WITH_SKETCHES_4x4, 6, 2);
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
