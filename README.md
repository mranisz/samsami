# SamSAMi text indexes library

##What is it?
The SamSAMi text indexes are variations of the sampled suffix array, which uses only a subset of the offsets in the suffix array yet allows to search for a pattern relatively fast. The novelty of the SamSAMi (Sampled Suffix Array with Minimizers) idea is to select the offset subset in such a way that allows to search for the pattern with only a single binary search over the suffixes (followed with verifications).

SamSAMi1 uses 5n bytes of RAM in the worst case (which is however unlikely). This includes 1n bytes for the input text and 4n bytes for the index. Typically its memory use depends on the parameters q, p and on average the required memory is about 1n+8/(q-p+2)n bytes.

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
- include "samsami/samsami.hpp" to your project
- compile it with "-std=c++11 -O3 -mpopcnt" options and link it with libraries:
  - samsami/libsamsami.a
  - samsami/libs/libaelf64.a (linux) or samsami/libs/libacof64.lib (windows)
- use "samsami" and "shared" namespaces

##API
There are several functions you can call on each of the FBCSA text index:
- **build** the index using text file called textFileName:
```
void build(const char *textFileName);
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
- get the result of **locate** query:
```
void locate(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res);
```
- set **verbose** mode:
```
void setVerbose(bool verbose);
```

##SamSAMi1\<SamSAMiType T\>

Parameters:
- T:
      - SamSAMiType::STANDARD
      - SamSAMiType::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMiType::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length (default: q = 4)
- p - minimizer length (default: p = 1)

Limitations: 
- pattern length ≥ q
- p ≤ q
- q > 0 and p > 0

Constructors:
```
SamSAMi1<SamSAMiType T>();
SamSAMi1<SamSAMiType T>(unsigned int q, unsigned int p);
```

##SamSAMi1Hash\<SamSAMiType T, HTType HASHTYPE\>
SamSAMi1Hash is SamSAMi1 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- T:
      - SamSAMiType::STANDARD
      - SamSAMiType::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMiType::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length
- p - minimizer length
- HASHTYPE:
      - HTType::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HTType::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array
- loadFactor - hash table load factor

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)
- p ≤ q
- q > 0 and p > 0
- k ≥ 2
- 0.0 < loadFactor < 1.0

Constructors:
```
SamSAMi1<SamSAMiType T, HTType HASHTYPE>(unsigned int q, unsigned int p, unsigned int k, double loadFactor);
```

##SamSAMi2\<SamSAMiType T\> 
To speed up searches, SamSAMi2 stores some extra data on 4 most significant bits in each SamSAMi offset (this is the reason for which the longest indexed text for SamSAMi2 is limited in our implementation to 256 MB only).

Parameters:
- T:
      - SamSAMiType::STANDARD
      - SamSAMiType::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMiType::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length (default: q = 4)
- p - minimizer length (default: p = 1)

Limitations: 
- pattern length ≥ q
- p ≤ q
- q > 0 and p > 0

Constructors:
```
SamSAMi2<SamSAMiType T>();
SamSAMi2<SamSAMiType T>(unsigned int q, unsigned int p);
```

##SamSAMi2Hash\<SamSAMiType T, HTType HASHTYPE\>
SamSAMi2Hash is SamSAMi2 with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- T:
      - SamSAMiType::STANDARD
      - SamSAMiType::WITH_SKETCHES_8x2 - reduces the number of verifications using 8 2-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
      - SamSAMiType::WITH_SKETCHES_4x4 - reduces the number of verifications using 4 4-bits sketches of sampled suffix preceding symbols, on average the additional required memory is about (4/(q-p+2))n bytes (half of the memory occupied by SamSAMi structure without text)
- q - window length
- p - minimizer length
- HASHTYPE:
      - HTType::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HTType::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array
- loadFactor - hash table load factor

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)
- p ≤ q
- q > 0 and p > 0
- k ≥ 2
- 0.0 < loadFactor < 1.0

Constructors:
```
SamSAMi2<SamSAMiType T, HTType HASHTYPE>(unsigned int q, unsigned int p, unsigned int k, double loadFactor);
```

##SamSAMiFM\<SamSAMiFMType T\>
SamSAMiFM is a hybrid of SamSAMi and FM. To speed up the verification phase (which is costly for short patterns in standard SamSAMi indexes), the FM index (employing a binary Huffman-shaped wavelet tree) is used.
The current version handles only the count query.

Parameters:
- T:
      - SamSAMiFMType::TYPE_512 - using 512b blocks in WT: 64b of rank data and 448b of encoded text data
      - SamSAMiFMType::TYPE_1024 - using 1024b blocks in WT: 64b of rank data and 960b of encoded text data
- q - window length (default: q = 4)
- p - minimizer length (default: p = 1)
- l - sampling parameter for mapping between SamSAMi and SA indexes, larger l reduces the space somewhat but also makes the search somewhat faster (default: l = 16)

Limitations: 
- pattern length ≥ q
- p ≤ q
- q > 0, p > 0 and l > 0

Constructors:
```
SamSAMiFM<SamSAMiFMType T>();
SamSAMiFM<SamSAMiFMType T>(unsigned int q, unsigned int p, unsigned int l);
```

##SamSAMiFMHash\<SamSAMiFMType T, HTType HASHTYPE\>
SamSAMiFMHash is SamSAMiFM with hashed k-symbol prefixes of suffixes from sampled suffix array to speed up searches (k ≥ 2). This variant is particularly efficient in speed for short patterns (not much longer than max(q, k + q - p)).

Parameters:
- T:
      - SamSAMiFMType::TYPE_512 - using 512b blocks in WT: 64b of rank data and 448b of encoded text data
      - SamSAMiFMType::TYPE_1024 - using 1024b blocks in WT: 64b of rank data and 960b of encoded text data
- q - window length
- p - minimizer length
- l - sampling parameter for mapping between SamSAMi and SA indexes, larger l reduces the space somewhat but also makes the search somewhat faster
- HASHTYPE:
      - HTType::STANDARD - using 8 bytes for each hashed entry: 4 bytes for left boundary + 4 bytes for right boundary
      - HTType::DENSE - using 6 bytes for each hashed entry: 4 bytes for left boundary + 2 bytes for right boundary
- k - length of prefixes of suffixes from suffix array
- loadFactor - hash table load factor

Limitations: 
- pattern length ≥ max(q, k + q - p) (in case k - p > 0, patterns shorter than (k + q - p) are handled by not-hashed variant of SamSAMi index if they are not shorter than q)
- p ≤ q
- q > 0, p > 0 and l > 0
- k ≥ 2
- 0.0 < loadFactor < 1.0

Constructors:
```
SamSAMiFM<SamSAMiFMType T, HTType HASHTYPE>(unsigned int q, unsigned int p, unsigned int l, unsigned int k, double loadFactor);
```

##SamSAMi1 usage example
```
#include <iostream>
#include <stdlib.h>
#include "samsami/shared/patterns.h"
#include "samsami/samsami.hpp"

using namespace std;
using namespace shared;
using namespace samsami;

int main(int argc, char *argv[]) {

	unsigned int queriesNum = 1000000;
	unsigned int patternLen = 20;
	SamSAMi1<SamSAMiType::STANDARD> *samSAMi1 = SamSAMi1<SamSAMiType::STANDARD>();
	const char *textFileName = "english";
	const char *indexFileName = "english-samsami1.idx";

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1::WITH_SKETCHES_4x4, 6, 2);
		samSAMi1->setVerbose(true);
		samSAMi1->build(textFileName);
		samSAMi1->save(indexFileName);
	}

	double indexSize = (double)samSAMi1->getIndexSize();
	cout << "Index size: " << indexSize << "B (" << (indexSize / (double)samSAMi1->getTextSize()) << "n)" << endl << endl;

	Patterns *P = new Patterns(textFileName, queriesNum, patternLen);
	unsigned char **patterns = P->getPatterns();

	for (unsigned int i = 0; i < queriesNum; ++i) {
		cout << "Pattern |" << patterns[i] << "| occurs " << samSAMi1->count(patterns[i], patternLen) << " times." << endl;
	}

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
