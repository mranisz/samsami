#include <vector>

#ifndef SHARED_HASH_H_
#define SHARED_HASH_H_

namespace samsami {

class HT {
private:
	void freeMemory();
	void initialize();
	void setLoadFactor(double loadFactor);
	void setK(unsigned int k);
	unsigned int getUniqueSuffixNum(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars = {});
	void fillHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
	void fillHTDataWithEntries(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars = {});

public:
	double loadFactor;
	unsigned int k;
	unsigned long long bucketsNum;

	alignas(128) unsigned int lut2[256][256][2];
	unsigned int *boundariesHT;
	unsigned int *alignedBoundariesHT;
	unsigned char *entriesHT;
	unsigned char *alignedEntriesHT;

	const static unsigned int emptyValueHT;

	HT() {
		this->initialize();
	}

	HT(unsigned int k, double loadFactor) {
		this->initialize();
		this->setK(k);
		this->setLoadFactor(loadFactor);
	};

	unsigned int getHTSize();
	unsigned long long getHashValue(unsigned char* str, unsigned int strLen);
	void build(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
	void buildWithEntries(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars = {});
	void getBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary);
	void getBoundariesWithEntries(unsigned char *pattern, unsigned int &leftBoundary, unsigned int &rightBoundary);
	void save(FILE *outFile);
	void load(FILE *inFile);
	void free();

	~HT() {
		this->freeMemory();
	}
};

}

#endif /* SHARED_HASH_H_ */
