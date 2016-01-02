#ifndef SAMSAMI_H_
#define SAMSAMI_H_

#include "shared/common.h"
#include "shared/hash.h"
#include <cstdio>

using namespace std;

namespace samsami {
    
/*SAMSAMI1*/ 
    
class SamSAMi1 : public Index {
private:
	unsigned int *samSAMi;
	unsigned int *alignedSamSAMi;
	unsigned int samSAMiLen;
        unsigned char *text;
        unsigned char *alignedText;
        unsigned int textLen;
	unsigned int *sketches;
	unsigned int *alignedSketches;
	unsigned int sketchesLen;
        unsigned int bitShift;
        unsigned int minPatternLenForHash;
	HT *ht = NULL;

	int type;
        unsigned int q;
        unsigned int p;

        void (SamSAMi1::*builder)(unsigned int *, unsigned int) = NULL;
	unsigned int (SamSAMi1::*countOperation)(unsigned char *, unsigned int) = NULL;

	void freeMemory();
	void initialize();
	void setType(int indexType);
        void setQ(unsigned int q);
        void setP(unsigned int p);
        void setMinPatternLenForHash();
	void setFunctions();
	void build_std(unsigned int *sa, unsigned int saLen);
        void build_sketches(unsigned int *sa, unsigned int saLen);
        unsigned int count_std(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_std_hash(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_sketches_hash(unsigned char *pattern, unsigned int patternLen);
        
public:
	enum IndexType {
		STANDARD = 1,
		WITH_SKETCHES = 2
	};

	SamSAMi1() {
		this->initialize();
                this->setType(SamSAMi1::STANDARD);
                this->setQ(4);
                this->setP(1);
		this->setFunctions();
	}

	SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p) {
		this->initialize();
		this->setType(indexType);
                this->setQ(q);
                this->setP(p);
		this->setFunctions();
	}

	SamSAMi1(SamSAMi1::IndexType indexType, unsigned int q, unsigned int p, HT::HTType hTType, unsigned int k, double loadFactor) {
		this->initialize();
		this->setType(indexType);
                this->setQ(q);
                this->setP(p);
                this->ht = new HT(hTType, k, loadFactor);
                this->setMinPatternLenForHash();
		this->setFunctions();
	}

	~SamSAMi1() {
		this->freeMemory();
                if (this->ht != NULL) delete this->ht;
	}

	void build(unsigned char *text, unsigned int textLen);
	void save(const char *fileName);
	void load(const char *fileName);
	void free();
	unsigned int getIndexSize();
	unsigned int getTextSize();

	unsigned int count(unsigned char *pattern, unsigned int patternLen);
	unsigned int *locate(unsigned char *pattern, unsigned int patternLen);
};

/*HTSAMSAMI2*/

class HTSamSAMi2 : public HTBase {
private:
        void setFunctions();
        void buildStandardHT(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
        void buildDenseHT(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
	void fillStandardHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
        void fillDenseHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
        void getStandardHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary);
        void getDenseHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary);
        
        void (HTSamSAMi2::*getBoundariesOperation)(unsigned char *, unsigned char *, unsigned int *, unsigned int &, unsigned int &) = NULL;
        
public:

	HTSamSAMi2() {
                this->initialize();
                this->setType(HTBase::STANDARD);
		this->setK(8);
		this->setLoadFactor(0.9);
                this->setFunctions();
	}

	HTSamSAMi2(HTBase::HTType type, unsigned int k, double loadFactor) {
		this->initialize();
                this->setType(type);
		this->setK(k);
		this->setLoadFactor(loadFactor);
                this->setFunctions();
	};

        void build(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen);
        void load(FILE *inFile);
        void getBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary);

	~HTSamSAMi2() {
		this->freeMemory();
	}
};

/*SAMSAMI2*/ 
    
class SamSAMi2 : public Index {
private:
	unsigned int *samSAMi;
	unsigned int *alignedSamSAMi;
	unsigned int samSAMiLen;
        unsigned char *text;
        unsigned char *alignedText;
        unsigned int textLen;
	unsigned int *sketches;
	unsigned int *alignedSketches;
	unsigned int sketchesLen;
        unsigned int bitShift;
        unsigned int minPatternLenForHash;
	HTSamSAMi2 *ht = NULL;

	int type;
        unsigned int q;
        unsigned int p;

        void (SamSAMi2::*builder)(unsigned int *, unsigned int) = NULL;
	unsigned int (SamSAMi2::*countOperation)(unsigned char *, unsigned int) = NULL;

	void freeMemory();
	void initialize();
	void setType(int indexType);
        void setQ(unsigned int q);
        void setP(unsigned int p);
        void setMinPatternLenForHash();
	void setFunctions();
	void build_std(unsigned int *sa, unsigned int saLen);
        void build_sketches(unsigned int *sa, unsigned int saLen);
        unsigned int count_std(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_std_hash(unsigned char *pattern, unsigned int patternLen);
        unsigned int count_sketches_hash(unsigned char *pattern, unsigned int patternLen);
        
public:
	enum IndexType {
		STANDARD = 1,
		WITH_SKETCHES = 2
	};

	SamSAMi2() {
		this->initialize();
                this->setType(SamSAMi2::STANDARD);
                this->setQ(4);
                this->setP(1);
		this->setFunctions();
	}

	SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p) {
		this->initialize();
		this->setType(indexType);
                this->setQ(q);
                this->setP(p);
		this->setFunctions();
	}

	SamSAMi2(SamSAMi2::IndexType indexType, unsigned int q, unsigned int p, HTSamSAMi2::HTType hTType, unsigned int k, double loadFactor) {
		this->initialize();
		this->setType(indexType);
                this->setQ(q);
                this->setP(p);
                this->ht = new HTSamSAMi2(hTType, k, loadFactor);
                this->setMinPatternLenForHash();
		this->setFunctions();
	}

	~SamSAMi2() {
		this->freeMemory();
                if (this->ht != NULL) delete this->ht;
	}

	void build(unsigned char *text, unsigned int textLen);
	void save(const char *fileName);
	void load(const char *fileName);
	void free();
	unsigned int getIndexSize();
	unsigned int getTextSize();

	unsigned int count(unsigned char *pattern, unsigned int patternLen);
	unsigned int *locate(unsigned char *pattern, unsigned int patternLen);
};

/*SHARED STUFF*/

void markSketch(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex);
unsigned int getBitShift(unsigned char *text, unsigned int textLen);
bool isSketchEqual(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int sketch, unsigned int i);
unsigned int getUniqueSuffixNumForSamSAMi2(unsigned int k, unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars = {});
void fillLUT2ForSamSAMi2(unsigned int lut2[256][256][2], unsigned char *text, unsigned int *sa, unsigned int saLen);
void binarySearchForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);
void binarySearchAStrcmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);
void binarySearchStrncmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);

}

#endif /* SAMSAMI_H_ */
