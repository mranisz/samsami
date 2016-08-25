#ifndef SAMSAMI_HPP
#define	SAMSAMI_HPP

#include <cstdio>
#include "libs/asmlib.h"
#include "shared/common.h"
#include "shared/wt.h"
#include "shared/huff.h"
#include "shared/hash.hpp"

using namespace std;
using namespace shared;

namespace samsami {
    
enum SamSAMiType {
        STANDARD = 1,
        WITH_SKETCHES_8x2 = 2,
        WITH_SKETCHES_4x4 = 3
};

void markSketch_8x2(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex);
unsigned int getBitShift_8x2(unsigned char *text, unsigned int textLen);
unsigned int getPatternSketch_8x2(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int &sketchLen);
bool isSketchEqual_8x2(unsigned int patternSketch, unsigned int sketchLen, unsigned int sketch, unsigned int i);
void markSketch_4x4(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex);
unsigned int getBitShift_4x4(unsigned char *text, unsigned int textLen);
unsigned int getPatternSketch_4x4(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int &sketchLen);
bool isSketchEqual_4x4(unsigned int patternSketch, unsigned int sketchLen, unsigned int sketch, unsigned int i);
unsigned int getUniqueSuffixNumForSamSAMi2(unsigned int k, unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars = {});
void fillLUT2ForSamSAMi2(unsigned int lut2[256][256][2], unsigned char *text, unsigned int *sa, unsigned int saLen);
void binarySearchForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);
void binarySearchAStrcmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);
void binarySearchStrncmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end);
    
template<SamSAMiType T> class SamSAMi1 : public Index {
protected:
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

        unsigned int q;
        unsigned int p;

	void freeMemory() {
            if (this->samSAMi != NULL) delete[] this->samSAMi;
            if (this->text != NULL) delete[] this->text;
            if (this->sketches != NULL) delete[] this->sketches;
        }
        
	void initialize() {
            this->samSAMi = NULL;
            this->alignedSamSAMi = NULL;
            this->samSAMiLen = 0;
            this->text = NULL;
            this->alignedText = NULL;
            this->textLen = 0;
            this->sketches = NULL;
            this->alignedSketches = NULL;
            this->sketchesLen = 0;
            this->bitShift = 0;
        }
        
        void setQ(unsigned int q) {
            if (q == 0) {
		cout << "Error: not valid q value" << endl;
		exit(1);
            }
            this->q = q;
        }
        
        void setP(unsigned int p) {
            if (p == 0 || p > this->q) {
		cout << "Error: not valid p value" << endl;
		exit(1);
            }
            this->p = p;
        }
        
        void loadText(const char *textFileName) {
            if (this->verbose) cout << "Loading text ... " << flush;
            this->textLen = getFileSize(textFileName, sizeof(unsigned char));
            this->text = new unsigned char[this->textLen + this->q + 128 + 1];
            for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
            this->alignedText = this->text + this->q;
            while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
            FILE *inFile;
            inFile = fopen(textFileName, "rb");
            size_t result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
            this->alignedText[this->textLen] = '\0';
            if (result != this->textLen) {
                    cout << "Error loading text from " << textFileName << endl;
                    exit(1);
            }
            fclose(inFile);
            checkNullChar(this->alignedText, this->textLen);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void build_std(unsigned int *sa, unsigned int saLen) {
            if (this->verbose) cout << "Building SamSAMi ... " << flush;
            bool *markers = new bool[this->textLen];
            for (unsigned int i = 0; i < this->textLen; ++i) markers[i] = false;

            this->samSAMiLen = 1;
            unsigned char *minimizer = new unsigned char[this->p + 1];
            unsigned char *curr = new unsigned char[this->p + 1];
            minimizer[this->p] = '\0';
            curr[this->p] = '\0';
            int pos = -1, prevPos = -1;

            for (unsigned int i = 0; i < this->textLen - this->q + 1; ++i) {
                    for (unsigned int j = 0; j < this->p; ++j) minimizer[j] = (unsigned char)255;
                    for (unsigned int j = i; j < i + this->q - this->p + 1; ++j) {
                            strncpy((char *)curr, (const char *)(this->alignedText + j), this->p);
                            if (strncmp((char *)curr, (const char *)minimizer, this->p) < 0) {
                                    strcpy((char *)minimizer, (const char *)curr);
                                    pos = j;
                            }
                    }
                    if (pos > prevPos) {
                            markers[pos] = true;
                            ++(this->samSAMiLen);
                            prevPos = pos;
                    }
            }

            this->samSAMi = new unsigned int[this->samSAMiLen + 32];
            this->alignedSamSAMi = this->samSAMi;
            while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
            this->alignedSamSAMi[0] = sa[0];
            unsigned int samSAMiCounter = 1;
            for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) this->alignedSamSAMi[samSAMiCounter++] = sa[i];
            if (this->verbose) cout << "Done" << endl;

            delete[] minimizer;
            delete[] curr;
            delete[] markers;
        }
        
        void build_sketches(unsigned int *sa, unsigned int saLen) {
            if (this->verbose) cout << "Building SamSAMi ... " << flush;
            bool *markers = new bool[this->textLen];
            for (unsigned int i = 0; i < this->textLen; ++i) markers[i] = false;

            this->samSAMiLen = 1;
            unsigned char *minimizer = new unsigned char[this->p + 1];
            unsigned char *curr = new unsigned char[this->p + 1];
            minimizer[this->p] = '\0';
            curr[this->p] = '\0';
            int pos = -1, prevPos = -1;

            for (unsigned int i = 0; i < this->textLen - this->q + 1; ++i) {
                    for (unsigned int j = 0; j < this->p; ++j) minimizer[j] = (unsigned char)255;
                    for (unsigned int j = i; j < i + this->q - this->p + 1; ++j) {
                            strncpy((char *)curr, (const char *)(this->alignedText + j), this->p);
                            if (strncmp((char *)curr, (const char *)minimizer, this->p) < 0) {
                                    strcpy((char *)minimizer, (const char *)curr);
                                    pos = j;
                            }
                    }
                    if (pos > prevPos) {
                            markers[pos] = true;
                            ++(this->samSAMiLen);
                            prevPos = pos;
                    }
            }

            this->samSAMi = new unsigned int[this->samSAMiLen + 32];
            this->alignedSamSAMi = this->samSAMi;
            while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
            this->alignedSamSAMi[0] = sa[0];
            unsigned int samSAMiCounter = 1;

            unsigned int sketchesTempLen = this->samSAMiLen + (this->samSAMiLen % 2);
            switch (T) {
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->bitShift = getBitShift_8x2(this->alignedText, this->textLen);
                    break;
                default:
                    this->bitShift = getBitShift_4x4(this->alignedText, this->textLen);
                    break;
            }
            unsigned int *sketchesTemp = new unsigned int[sketchesTempLen];
            for (unsigned int i = 0; i < sketchesTempLen; ++i) sketchesTemp[i] = 0;
            unsigned int sketchesCounter = 0;
            switch (T) {
                case SamSAMiType::WITH_SKETCHES_8x2:
                    markSketch_8x2(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
                    break;
                default:
                    markSketch_4x4(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
                    break;
            }
            ++sketchesCounter;

            for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) {
                    this->alignedSamSAMi[samSAMiCounter++] = sa[i];
                    switch (T) {
                        case SamSAMiType::WITH_SKETCHES_8x2:
                            markSketch_8x2(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
                            break;
                        default:
                            markSketch_4x4(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
                            break;
                    }
                    ++sketchesCounter;
            }

            this->sketchesLen = sketchesTempLen / 2;
            this->sketches = new unsigned int[this->sketchesLen + 32];
            this->alignedSketches = this->sketches;
            while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
            for (unsigned int i = 0; i < this->sketchesLen; ++i) this->alignedSketches[i] = (sketchesTemp[2 * i] << 16) + sketchesTemp[2 * i + 1];
            if (this->verbose) cout << "Done" << endl;

            delete[] minimizer;
            delete[] curr;
            delete[] markers;
        }
        
        inline unsigned int count_std(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, count = 0, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                for (unsigned int i = beg; i < end; ++i) {
                    if (strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                }
            }
            return count;
        }
        
        inline unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, count = 0, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                unsigned int sketchLen, patternSketch;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                        }
                        break;
                }
            }
            return count;
        }
        
        inline void locate_std(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) res.insert(res.end(), this->alignedSamSAMi + beg, this->alignedSamSAMi + end);
            else {
                for (unsigned int i = beg; i < end; ++i) {
                    if (strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                }
            }
        }
        
        inline void locate_sketches(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) res.insert(res.end(), this->alignedSamSAMi + beg, this->alignedSamSAMi + end);
            else {
                unsigned int sketchLen, patternSketch;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                        }
                        break;
                }
            }
        }
        
public:
	SamSAMi1() {
		this->initialize();
                this->setQ(4);
                this->setP(1);
	}

	SamSAMi1(unsigned int q, unsigned int p) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
	}

	~SamSAMi1() {
		this->free();
	}

	void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->build_sketches(sa, saLen);
                    break;
                default:
                    this->build_std(sa, saLen);
                    break;
            }
            delete[] sa;
        }
        
	void save(const char *fileName) {
            if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
            FILE *outFile;
            outFile = fopen(fileName, "w");
            fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
            fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
            fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
            fwrite(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->sketchesLen > 0) fwrite(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, outFile);
            fwrite(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fclose(outFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void load(const char *fileName) {
            this->free();
            FILE *inFile;
            inFile = fopen(fileName, "rb");
            size_t result;
            result = fread(&this->verbose, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->verbose) cout << "Loading index from " << fileName << " ... " << flush;
            result = fread(&this->q, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->p, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->samSAMiLen > 0) {
                    this->samSAMi = new unsigned int[this->samSAMiLen + 32];
                    this->alignedSamSAMi = this->samSAMi;
                    while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
                    result = fread(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, inFile);
                    if (result != this->samSAMiLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->textLen > 0) {
                    this->text = new unsigned char[this->textLen + this->q + 128 + 1];
                    for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
                    this->alignedText = this->text + this->q;
                    while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
                    result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
                    this->alignedText[this->textLen] = '\0';
                    if (result != this->textLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->sketchesLen > 0) {
                    this->sketches = new unsigned int[this->sketchesLen + 32];
                    this->alignedSketches = this->sketches;
                    while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
                    result = fread(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, inFile);
                    if (result != this->sketchesLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            fclose(inFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void free() {
            this->freeMemory();
            this->initialize();
        }
        
	unsigned int getIndexSize() {
            unsigned int size = sizeof(this->q) + sizeof(this->p) + sizeof(this->samSAMiLen) + sizeof(this->sketchesLen) + sizeof(this->bitShift);
            if (this->samSAMiLen > 0) size += (this->samSAMiLen + 32) * sizeof(unsigned int);
            if (this->textLen > 0) size += (this->textLen + this->q + 128 + 1) * sizeof(unsigned char);
            if (this->sketchesLen > 0) size += (this->sketchesLen + 32) * sizeof(unsigned int);
            return size;
        }
        
	unsigned int getTextSize() {
            return this->textLen * sizeof(unsigned char);
        }

	unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    return this->count_sketches(pattern, patternLen);
                    break;
                default:
                    return this->count_std(pattern, patternLen);
                    break;
            }
        }
        
	void locate(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->locate_sketches(pattern, patternLen, res);
                    break;
                default:
                    this->locate_std(pattern, patternLen, res);
                    break;
            }
        }
};

template<SamSAMiType T, HTType HASHTYPE> class SamSAMi1Hash : public SamSAMi1<T> {
private:
        unsigned int minPatternLenForHash;
	HT<HASHTYPE> *ht = NULL;

	void freeMemory() {
            SamSAMi1<T>::freeMemory();
            if (this->ht != NULL) this->ht->free();
        }
        
        void setMinPatternLenForHash() {
            if (this->q > (this->ht->k + this->q - this->p)) this->minPatternLenForHash = this->q;
            else this->minPatternLenForHash = this->ht->k + this->q - this->p;
        }
        
	void build_std(unsigned int *sa, unsigned int saLen) {
            SamSAMi1<T>::build_std(sa, saLen);
            if (this->ht != NULL) {
                if (this->verbose) cout << "Building hash table ... " << flush;
                this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
                if (this->verbose) cout << "Done" << endl;
            }
        }
        
        void build_sketches(unsigned int *sa, unsigned int saLen) {
            SamSAMi1<T>::build_sketches(sa, saLen);
            if (this->ht != NULL) {
                if (this->verbose) cout << "Building hash table ... " << flush;
                this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
                if (this->verbose) cout << "Done" << endl;
            }
        }
        
        unsigned int count_std(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->minPatternLenForHash) return SamSAMi1<T>::count_std(pattern, patternLen);
            unsigned int beg, end, count = 0, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearch(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                    for (unsigned int i = beg; i < end; ++i) {
                            if (strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                    }
            }
            return count;
        }
        
        unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->minPatternLenForHash) SamSAMi1<T>::count_sketches(pattern, patternLen);
            unsigned int beg, end, count = 0, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearch(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                    unsigned int sketchLen, patternSketch;
                    switch(T) {
                        case SamSAMiType::WITH_SKETCHES_8x2:
                            patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                            for (unsigned int i = beg; i < end; ++i) {
                                if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                            }
                            break;
                        default:
                            patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                            for (unsigned int i = beg; i < end; ++i) {
                                if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
                            }
                            break;
                    }
            }
            return count;
        }
        
        void locate_std(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->minPatternLenForHash) {
                SamSAMi1<T>::locate_std(pattern, patternLen, res);
                return;
            }
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearch(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) res.insert(res.end(), this->alignedSamSAMi + beg, this->alignedSamSAMi + end);
            else {
                    for (unsigned int i = beg; i < end; ++i) {
                            if (strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                    }
            }
        }
        
        void locate_sketches(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->minPatternLenForHash) {
                SamSAMi1<T>::locate_sketches(pattern, patternLen, res);
                return;
            }
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                        pos = i;
                }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearch(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) res.insert(res.end(), this->alignedSamSAMi + beg, this->alignedSamSAMi + end);
            else {
                unsigned int sketchLen, patternSketch;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) res.push_back(this->alignedSamSAMi[i] - pos);
                        }
                        break;
                }
            }
        }
           
public:
	SamSAMi1Hash(unsigned int q, unsigned int p, unsigned int k, double loadFactor) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
                this->ht = new HT<HASHTYPE>(k, loadFactor);
                this->setMinPatternLenForHash();
	}

	~SamSAMi1Hash() {
		this->free();
                if (this->ht != NULL) delete this->ht;
	}
        
        void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->build_sketches(sa, saLen);
                    break;
                default:
                    this->build_std(sa, saLen);
                    break;
            }
            delete[] sa;
        }
        
	void save(const char *fileName) {
            if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
            FILE *outFile;
            outFile = fopen(fileName, "w");
            fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
            fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
            fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
            fwrite(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->sketchesLen > 0) fwrite(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, outFile);
            fwrite(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            this->ht->save(outFile);
            fclose(outFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void load(const char *fileName) {
            this->free();
            FILE *inFile;
            inFile = fopen(fileName, "rb");
            size_t result;
            result = fread(&this->verbose, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->verbose) cout << "Loading index from " << fileName << " ... " << flush;
            result = fread(&this->q, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->p, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->samSAMiLen > 0) {
                    this->samSAMi = new unsigned int[this->samSAMiLen + 32];
                    this->alignedSamSAMi = this->samSAMi;
                    while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
                    result = fread(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, inFile);
                    if (result != this->samSAMiLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->textLen > 0) {
                    this->text = new unsigned char[this->textLen + this->q + 128 + 1];
                    for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
                    this->alignedText = this->text + this->q;
                    while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
                    result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
                    this->alignedText[this->textLen] = '\0';
                    if (result != this->textLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->sketchesLen > 0) {
                    this->sketches = new unsigned int[this->sketchesLen + 32];
                    this->alignedSketches = this->sketches;
                    while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
                    result = fread(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, inFile);
                    if (result != this->sketchesLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            delete this->ht;
            this->ht = new HT<HASHTYPE>();
            this->ht->load(inFile);
            this->setMinPatternLenForHash();
            fclose(inFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
        void free() {
            this->freeMemory();
            this->initialize();
        }
        
        unsigned int getIndexSize() {
            return SamSAMi1<T>::getIndexSize() + sizeof(this->minPatternLenForHash) + sizeof(this->ht) + this->ht->getHTSize();
        }

	unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    return this->count_sketches(pattern, patternLen);
                    break;
                default:
                    return this->count_std(pattern, patternLen);
                    break;
            }
        }
        
	void locate(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->locate_sketches(pattern, patternLen, res);
                    break;
                default:
                    this->locate_std(pattern, patternLen, res);
                    break;
            }
        }
        
};

template<HTType T> class HTSamSAMi2 : public HTBase {
private:     
	void fillStandardHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
            unsigned long long hash = this->bucketsNum;
            this->boundariesHTLen = 2 * this->bucketsNum;
            this->boundariesHT = new unsigned int[this->boundariesHTLen + 32];
            this->alignedBoundariesHT = this->boundariesHT;
            while ((unsigned long long)(this->alignedBoundariesHT) % 128) ++(this->alignedBoundariesHT);

            for (unsigned long long i = 0; i < this->boundariesHTLen; ++i) this->alignedBoundariesHT[i] = HTBase::emptyValueHT;

            unsigned char *lastPattern = new unsigned char[this->k + 1];
            for (unsigned int i = 0; i < this->k; ++i) lastPattern[i] = 255;
            lastPattern[this->k] = '\0';

            unsigned char *pattern = new unsigned char[this->k + 1];
            unsigned int lastNotOutsideTextIndex;
            bool notLastOutsideText = true;

            for (unsigned int i = 0; i < saLen; i++) {
                    if ((sa[i] & 0x0FFFFFFF) > (textLen - this->k)) {
                            if (notLastOutsideText) lastNotOutsideTextIndex = i;
                            notLastOutsideText = false;
                            continue;
                    }
                    strncpy((char *)pattern, (const char *)(text + (sa[i] & 0x0FFFFFFF)), this->k);
                    pattern[this->k] = '\0';
                    if (strcmp((char *)pattern, (const char *)lastPattern) == 0) {
                            notLastOutsideText = true;
                            continue;
                    }
                    else {
                            strcpy((char *)lastPattern, (const char *)pattern);
                            if (hash != this->bucketsNum)  {
                                    if (notLastOutsideText) this->alignedBoundariesHT[2 * hash + 1] = i;
                                    else this->alignedBoundariesHT[2 * hash + 1] = lastNotOutsideTextIndex;
                            }
                            notLastOutsideText = true;
                            hash = getHashValue(pattern, this->k) % this->bucketsNum;
                    }
                    while (true) {
                            if (this->alignedBoundariesHT[2 * hash] == HTBase::emptyValueHT) {
                                    this->alignedBoundariesHT[2 * hash] = i;
                                    break;
                            }
                            else {
                                    hash = (hash + 1) % this->bucketsNum;
                            }
                    }
            }
            if (hash != this->bucketsNum) this->alignedBoundariesHT[2 * hash + 1] = saLen;

            delete[] lastPattern;
            delete[] pattern;
        }
        
        void fillDenseHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
            unsigned long long hash = this->bucketsNum;
            unsigned int beg = 0, end = 0, step = 0;
            this->boundariesHTLen = this->bucketsNum;
            this->boundariesHT = new unsigned int[this->boundariesHTLen + 32];
            this->alignedBoundariesHT = this->boundariesHT;
            while ((unsigned long long)(this->alignedBoundariesHT) % 128) ++(this->alignedBoundariesHT);
            this->denseBoundariesHTLen = this->boundariesHTLen / 2 + this->boundariesHTLen % 2;
            this->denseBoundariesHT = new unsigned int[this->denseBoundariesHTLen + 32];
            this->alignedDenseBoundariesHT = this->denseBoundariesHT;
            while ((unsigned long long)(this->alignedDenseBoundariesHT) % 128) ++(this->alignedDenseBoundariesHT);
            unsigned int* boundariesHTTemp = new unsigned int[this->boundariesHTLen];

            for (unsigned long long i = 0; i < this->boundariesHTLen; ++i) {
                this->alignedBoundariesHT[i] = HTBase::emptyValueHT;
                boundariesHTTemp[i] = 0;
            }

            unsigned char *lastPattern = new unsigned char[this->k + 1];
            for (unsigned int i = 0; i < this->k; ++i) lastPattern[i] = 255;
            lastPattern[this->k] = '\0';

            unsigned char *pattern = new unsigned char[this->k + 1];
            unsigned int lastNotOutsideTextIndex;
            bool notLastOutsideText = true;

            for (unsigned int i = 0; i < saLen; i++) {
                    if ((sa[i] & 0x0FFFFFFF) > (textLen - this->k)) {
                            if (notLastOutsideText) lastNotOutsideTextIndex = i;
                            notLastOutsideText = false;
                            continue;
                    }
                    strncpy((char *)pattern, (const char *)(text + (sa[i] & 0x0FFFFFFF)), this->k);
                    pattern[this->k] = '\0';
                    if (strcmp((char *)pattern, (const char *)lastPattern) == 0) {
                            notLastOutsideText = true;
                            continue;
                    }
                    else {
                            strcpy((char *)lastPattern, (const char *)pattern);
                            if (hash != this->bucketsNum)  {
                                    if (notLastOutsideText) boundariesHTTemp[hash] = (unsigned int)ceil(((double)i - beg) / step);
                                    else boundariesHTTemp[hash] = (unsigned int)ceil(((double)lastNotOutsideTextIndex - beg) / step);

                            }
                            notLastOutsideText = true;
                            hash = getHashValue(pattern, this->k) % this->bucketsNum;
                    }
                    while (true) {
                            if (this->alignedBoundariesHT[hash] == HTBase::emptyValueHT) {
                                    beg = this->lut2[pattern[0]][pattern[1]][0];
                                    end = this->lut2[pattern[0]][pattern[1]][1];
                                    step = (unsigned int)ceil(((double)end + 1 - beg) / 65536);
                                    this->alignedBoundariesHT[hash] = i;
                                    break;
                            }
                            else {
                                    hash = (hash + 1) % this->bucketsNum;
                            }
                    }
            }
            if (hash != this->bucketsNum) boundariesHTTemp[hash] = (unsigned int)ceil(((double)saLen - beg) / step);

            for (unsigned long long i = 0; i < this->denseBoundariesHTLen; ++i) this->alignedDenseBoundariesHT[i] = (boundariesHTTemp[2 * i] << 16) + boundariesHTTemp[2 * i + 1];

            delete[] boundariesHTTemp;
            delete[] lastPattern;
            delete[] pattern;
        }
        
        void getStandardHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
            unsigned int leftBoundaryLUT2 = this->lut2[pattern[0]][pattern[1]][0];
            unsigned int rightBoundaryLUT2 = this->lut2[pattern[0]][pattern[1]][1];
            if (leftBoundaryLUT2 < rightBoundaryLUT2) {
                    unsigned long long hash = this->getHashValue(pattern, this->k) % this->bucketsNum;
                    while (true) {
                            leftBoundary = this->alignedBoundariesHT[2 * hash];
                            if (leftBoundary >= leftBoundaryLUT2 && leftBoundary < rightBoundaryLUT2 && strncmp((const char *)pattern, (const char *)(text + (sa[leftBoundary] & 0x0FFFFFFF)), this->k) == 0) {
                                    rightBoundary = this->alignedBoundariesHT[2 * hash + 1];
                                    break;
                            }
                            if (leftBoundary == HTBase::emptyValueHT) {
                                    leftBoundary = 0;
                                    rightBoundary = 0;
                                    return;
                            }
                            ++hash;
                            if (hash == this->bucketsNum) {
                                    hash = 0;
                            }
                    }
            } else {
                    leftBoundary = 0;
                    rightBoundary = 0;
            }
        }
        
        void getDenseHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
            unsigned int step;
            unsigned int leftBoundaryLUT2 = this->lut2[pattern[0]][pattern[1]][0];
            unsigned int rightBoundaryLUT2 = this->lut2[pattern[0]][pattern[1]][1];
            if (leftBoundaryLUT2 < rightBoundaryLUT2) {
                    unsigned long long hash = this->getHashValue(pattern, this->k) % this->bucketsNum;
                    while (true) {
                            leftBoundary = this->alignedBoundariesHT[hash];
                            if (leftBoundary >= leftBoundaryLUT2 && leftBoundary < rightBoundaryLUT2 && strncmp((const char *)pattern, (const char *)(text + (sa[leftBoundary] & 0x0FFFFFFF)), this->k) == 0) {
                                    step = (unsigned int)ceil(((double)rightBoundaryLUT2 + 1 - leftBoundaryLUT2) / 65536);
                                    if ((hash & 1) == 0) rightBoundary = (this->alignedDenseBoundariesHT[hash / 2] >> 16) * step + leftBoundaryLUT2;
                                    else rightBoundary = (this->alignedDenseBoundariesHT[hash / 2] & 0xFFFF) * step + leftBoundaryLUT2;
                                    break;
                            }
                            if (leftBoundary == HTBase::emptyValueHT) {
                                    leftBoundary = 0;
                                    rightBoundary = 0;
                                    return;
                            }
                            ++hash;
                            if (hash == this->bucketsNum) {
                                    hash = 0;
                            }
                    }
            } else {
                    leftBoundary = 0;
                    rightBoundary = 0;
            }
        }
        
        
public:

	HTSamSAMi2() {
                this->initialize();
		this->setK(8);
		this->setLoadFactor(0.9);
	}

	HTSamSAMi2(unsigned int k, double loadFactor) {
		this->initialize();
		this->setK(k);
		this->setLoadFactor(loadFactor);
	};
        
        ~HTSamSAMi2() {
		this->free();
	}

        void build(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
            this->free();
            fillLUT2ForSamSAMi2(this->lut2, text, sa, saLen);
            unsigned int uniqueSuffixNum = getUniqueSuffixNumForSamSAMi2(this->k, text, textLen, sa, saLen);
            this->bucketsNum = (double)uniqueSuffixNum * (1.0 / this->loadFactor);
            switch(T) {
                case HTType::DENSE:
                    this->fillDenseHTData(text, textLen, sa, saLen);
                    break;
                default:
                    this->fillStandardHTData(text, textLen, sa, saLen);
                    break;
            }
        }
        
        void getBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
            switch(T) {
                case HTType::DENSE:
                    return this->getDenseHTBoundaries(pattern, text, sa, leftBoundary, rightBoundary);
                    break;
                default:
                    return this->getStandardHTBoundaries(pattern, text, sa, leftBoundary, rightBoundary);
                    break;
            }
        }
};
    
template<SamSAMiType T> class SamSAMi2 : public SamSAMi1<T> {
protected:
	void build_std(unsigned int *sa, unsigned int saLen) {
            if (this->verbose) cout << "Building SamSAMi ... " << flush;
            bool *markers = new bool[this->textLen];
            unsigned int *positions = new unsigned int[this->textLen];
            for (unsigned int i = 0; i < this->textLen; ++i) {
                    markers[i] = false;
                    positions[i] = 0;
            }
            this->samSAMiLen = 1;
            unsigned char *minimizer = new unsigned char[this->p + 1];
            unsigned char *curr = new unsigned char[this->p + 1];
            minimizer[this->p] = '\0';
            curr[this->p] = '\0';
            int pos = -1, prevPos = -1;

            for (unsigned int i = 0; i < this->textLen - this->q + 1; ++i) {
                    for (unsigned int j = 0; j < this->p; ++j) minimizer[j] = (unsigned char)255;
                    for (unsigned int j = i; j < i + this->q - this->p + 1; ++j) {
                            strncpy((char *)curr, (const char *)(this->alignedText + j), this->p);
                            if (strncmp((char *)curr, (const char *)minimizer, this->p) < 0) {
                                    strcpy((char *)minimizer, (const char *)curr);
                                    pos = j;
                            }
                    }
                    if (pos > prevPos) {
                            if (prevPos != 1 && pos - prevPos < 16) positions[pos] = pos - prevPos;
                            markers[pos] = true;
                            ++(this->samSAMiLen);
                            prevPos = pos;
                    }
            }

            this->samSAMi = new unsigned int[this->samSAMiLen + 32];
            this->alignedSamSAMi = this->samSAMi;
            while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
            this->alignedSamSAMi[0] = sa[0];
            unsigned int samSAMiCounter = 1;
            for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) this->alignedSamSAMi[samSAMiCounter++] = (((unsigned int)sa[i]) | (positions[sa[i]] << 28));
            if (this->verbose) cout << "Done" << endl;

            delete[] minimizer;
            delete[] curr;
            delete[] markers;
            delete[] positions;
        }
        
        void build_sketches(unsigned int *sa, unsigned int saLen) {
            if (this->verbose) cout << "Building SamSAMi ... " << flush;
            bool *markers = new bool[this->textLen];
            unsigned int *positions = new unsigned int[this->textLen];
            for (unsigned int i = 0; i < this->textLen; ++i) {
                    markers[i] = false;
                    positions[i] = 0;
            }
            this->samSAMiLen = 1;
            unsigned char *minimizer = new unsigned char[this->p + 1];
            unsigned char *curr = new unsigned char[this->p + 1];
            minimizer[this->p] = '\0';
            curr[this->p] = '\0';
            int pos = -1, prevPos = -1;

            for (unsigned int i = 0; i < this->textLen - this->q + 1; ++i) {
                    for (unsigned int j = 0; j < this->p; ++j) minimizer[j] = (unsigned char)255;
                    for (unsigned int j = i; j < i + this->q - this->p + 1; ++j) {
                            strncpy((char *)curr, (const char *)(this->alignedText + j), this->p);
                            if (strncmp((char *)curr, (const char *)minimizer, this->p) < 0) {
                                    strcpy((char *)minimizer, (const char *)curr);
                                    pos = j;
                            }
                    }
                    if (pos > prevPos) {
                            if (prevPos != 1 && pos - prevPos < 16) positions[pos] = pos - prevPos;
                            markers[pos] = true;
                            ++(this->samSAMiLen);
                            prevPos = pos;
                    }
            }

            this->samSAMi = new unsigned int[this->samSAMiLen + 32];
            this->alignedSamSAMi = this->samSAMi;
            while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
            this->alignedSamSAMi[0] = sa[0];
            unsigned int samSAMiCounter = 1;

            unsigned int sketchesTempLen = this->samSAMiLen + (this->samSAMiLen % 2);
            switch (T) {
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->bitShift = getBitShift_8x2(this->alignedText, this->textLen);
                    break;
                default:
                    this->bitShift = getBitShift_4x4(this->alignedText, this->textLen);
                    break;
            }
            unsigned int *sketchesTemp = new unsigned int[sketchesTempLen];
            for (unsigned int i = 0; i < sketchesTempLen; ++i) sketchesTemp[i] = 0;
            unsigned int sketchesCounter = 0;
            switch (T) {
                case SamSAMiType::WITH_SKETCHES_8x2:
                    markSketch_8x2(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
                    break;
                default:
                    markSketch_4x4(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
                    break;
            }
            ++sketchesCounter;

            for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) {
                    this->alignedSamSAMi[samSAMiCounter++] = (((unsigned int)sa[i]) | (positions[sa[i]] << 28));
                    switch (T) {
                        case SamSAMiType::WITH_SKETCHES_8x2:
                            markSketch_8x2(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
                            break;
                        default:
                            markSketch_4x4(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
                            break;
                    }
                    ++sketchesCounter;
            }

            this->sketchesLen = sketchesTempLen / 2;
            this->sketches = new unsigned int[this->sketchesLen + 32];
            this->alignedSketches = this->sketches;
            while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
            for (unsigned int i = 0; i < this->sketchesLen; ++i) this->alignedSketches[i] = (sketchesTemp[2 * i] << 16) + sketchesTemp[2 * i + 1];
            if (this->verbose) cout << "Done" << endl;

            delete[] minimizer;
            delete[] curr;
            delete[] markers;
            delete[] positions;
            delete[] sketchesTemp;
        }
        
        unsigned int count_std(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, count = 0, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    prevPos = pos;
                    pos = i;
                }
            }
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                for (unsigned int i = beg; i < end; ++i) {
                    unsigned int diffPos = this->alignedSamSAMi[i] >> 28;
                    if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                    if (strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                }
            }
            return count;
        }
        
        unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, count = 0, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    prevPos = pos;
                    pos = i;
                }
            }
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                unsigned int sketchLen, patternSketch, diffPos;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                        }
                        break;
                }
            }
            return count;
        }
        
        void locate_std(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    prevPos = pos;
                    pos = i;
                }
            }
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) {
                for (unsigned int i = beg; i < end; ++i) {
                    res.push_back(this->alignedSamSAMi[i] & 0x0FFFFFFF);
                }
            } else {
                for (unsigned int i = beg; i < end; ++i) {
                    unsigned int diffPos = this->alignedSamSAMi[i] >> 28;
                    if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                    if (strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                }
            }
        }
        
        void locate_sketches(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    prevPos = pos;
                    pos = i;
                }
            }
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) {
                for (unsigned int i = beg; i < end; ++i) {
                    res.push_back(this->alignedSamSAMi[i] & 0x0FFFFFFF);
                }
            } else {
                unsigned int sketchLen, patternSketch, diffPos;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                        }
                        break;
                }
            }
        }
        
        
public:
	SamSAMi2() {
		this->initialize();
                this->setQ(4);
                this->setP(1);
	}

	SamSAMi2(unsigned int q, unsigned int p) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
	}

	~SamSAMi2() {
		this->free();
	}
        
        void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->build_sketches(sa, saLen);
                    break;
                default:
                    this->build_std(sa, saLen);
                    break;
            }
            delete[] sa;
        }
        
        unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    return this->count_sketches(pattern, patternLen);
                    break;
                default:
                    return this->count_std(pattern, patternLen);
                    break;
            }
        }
        
	void locate(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->locate_sketches(pattern, patternLen, res);
                    break;
                default:
                    this->locate_std(pattern, patternLen, res);
                    break;
            }
        }
};

template<SamSAMiType T, HTType HASHTYPE> class SamSAMi2Hash : public SamSAMi2<T> {
private:
        unsigned int minPatternLenForHash;
	HTSamSAMi2<HASHTYPE> *ht = NULL;

	void freeMemory() {
            SamSAMi2<T>::freeMemory();
            if (this->ht != NULL) this->ht->free();
        }
        
        void setMinPatternLenForHash() {
            if (this->q > (this->ht->k + this->q - this->p)) this->minPatternLenForHash = this->q;
            else this->minPatternLenForHash = this->ht->k + this->q - this->p;
        }
        
	void build_std(unsigned int *sa, unsigned int saLen) {
            SamSAMi2<T>::build_std(sa, saLen);
            if (this->ht != NULL) {
                if (this->verbose) cout << "Building hash table ... " << flush;
                this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
                if (this->verbose) cout << "Done" << endl;
            }
        }
        
        void build_sketches(unsigned int *sa, unsigned int saLen) {
            SamSAMi2<T>::build_sketches(sa, saLen);
            if (this->ht != NULL) {
                if (this->verbose) cout << "Building hash table ... " << flush;
                this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
                if (this->verbose) cout << "Done" << endl;
            }
        }
        
        unsigned int count_std(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->minPatternLenForHash) return SamSAMi2<T>::count_std(pattern, patternLen);
            unsigned int beg, end, count = 0, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            prevPos = pos;
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                    for (unsigned int i = beg; i < end; ++i) {
                            unsigned int diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                    }
            }
            return count;
        }
        
        unsigned int count_sketches(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->minPatternLenForHash) return SamSAMi2<T>::count_sketches(pattern, patternLen);
            unsigned int beg, end, count = 0, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            prevPos = pos;
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                unsigned int sketchLen, patternSketch, diffPos;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
                        }
                        break;
                }
            }
            return count;
        }
        
        void locate_std(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->minPatternLenForHash) {
                SamSAMi2<T>::locate_std(pattern, patternLen, res);
                return;
            }
            unsigned int beg, end, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            prevPos = pos;
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) {
                for (unsigned int i = beg; i < end; ++i) {
                    res.push_back(this->alignedSamSAMi[i] & 0x0FFFFFFF);
                }
            } else {
                    for (unsigned int i = beg; i < end; ++i) {
                            unsigned int diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                    }
            }
        }
        
        void locate_sketches(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            if (patternLen < this->minPatternLenForHash) {
                SamSAMi2<T>::locate_sketches(pattern, patternLen, res);
                return;
            }
            unsigned int beg, end, pos = 0, prevPos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                    if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                            prevPos = pos;
                            pos = i;
                    }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearchForSamSAMi2(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) {
                for (unsigned int i = beg; i < end; ++i) {
                    res.push_back(this->alignedSamSAMi[i] & 0x0FFFFFFF);
                }
            } else {
                unsigned int sketchLen, patternSketch, diffPos;
                switch(T) {
                    case SamSAMiType::WITH_SKETCHES_8x2:
                        patternSketch = getPatternSketch_8x2(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_8x2(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                        }
                        break;
                    default:
                        patternSketch = getPatternSketch_4x4(this->bitShift, pattern, pos, sketchLen);
                        for (unsigned int i = beg; i < end; ++i) {
                            diffPos = this->alignedSamSAMi[i] >> 28;
                            if (diffPos != 0 && diffPos <= pos && prevPos != (pos - diffPos)) continue;
                            if (isSketchEqual_4x4(patternSketch, sketchLen, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) res.push_back((this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos);
                        }
                        break;
                }
            }
        }
           
public:
	SamSAMi2Hash(unsigned int q, unsigned int p, unsigned int k, double loadFactor) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
                this->ht = new HTSamSAMi2<HASHTYPE>(k, loadFactor);
                this->setMinPatternLenForHash();
	}

	~SamSAMi2Hash() {
		this->free();
                if (this->ht != NULL) delete this->ht;
	}
        
                void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->build_sketches(sa, saLen);
                    break;
                default:
                    this->build_std(sa, saLen);
                    break;
            }
            delete[] sa;
        }
        
	void save(const char *fileName) {
            if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
            FILE *outFile;
            outFile = fopen(fileName, "w");
            fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
            fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
            fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
            fwrite(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->sketchesLen > 0) fwrite(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, outFile);
            fwrite(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            this->ht->save(outFile);
            fclose(outFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void load(const char *fileName) {
            this->free();
            FILE *inFile;
            inFile = fopen(fileName, "rb");
            size_t result;
            result = fread(&this->verbose, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->verbose) cout << "Loading index from " << fileName << " ... " << flush;
            result = fread(&this->q, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->p, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->samSAMiLen > 0) {
                    this->samSAMi = new unsigned int[this->samSAMiLen + 32];
                    this->alignedSamSAMi = this->samSAMi;
                    while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
                    result = fread(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, inFile);
                    if (result != this->samSAMiLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->textLen > 0) {
                    this->text = new unsigned char[this->textLen + this->q + 128 + 1];
                    for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
                    this->alignedText = this->text + this->q;
                    while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
                    result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
                    this->alignedText[this->textLen] = '\0';
                    if (result != this->textLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->sketchesLen > 0) {
                    this->sketches = new unsigned int[this->sketchesLen + 32];
                    this->alignedSketches = this->sketches;
                    while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
                    result = fread(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, inFile);
                    if (result != this->sketchesLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            delete this->ht;
            this->ht = new HTSamSAMi2<HASHTYPE>();
            this->ht->load(inFile);
            this->setMinPatternLenForHash();
            fclose(inFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
        void free() {
            this->freeMemory();
            this->initialize();
        }
        
	unsigned int getIndexSize() {
            return SamSAMi2<T>::getIndexSize() + sizeof(this->minPatternLenForHash) + sizeof(this->ht) + this->ht->getHTSize();
        }
        
        unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    return this->count_sketches(pattern, patternLen);
                    break;
                default:
                    return this->count_std(pattern, patternLen);
                    break;
            }
        }
        
	void locate(unsigned char* pattern, unsigned int patternLen, vector<unsigned int>& res) {
            switch(T) {
                case SamSAMiType::WITH_SKETCHES_4x4:
                case SamSAMiType::WITH_SKETCHES_8x2:
                    this->locate_sketches(pattern, patternLen, res);
                    break;
                default:
                    this->locate_std(pattern, patternLen, res);
                    break;
            }
        }
        
};

enum SamSAMiFMType {
        TYPE_512 = 8,
        TYPE_1024 = 16
};

template<SamSAMiFMType T> class SamSAMiFM : public Index {
protected:
	unsigned int *samSAMi;
	unsigned int *alignedSamSAMi;
	unsigned int samSAMiLen;
        unsigned char *text;
        unsigned char *alignedText;
        unsigned int textLen;
        unsigned int *M;
        unsigned int MLen;
        unsigned int *alignedM;
        unsigned int *H;
        unsigned int HLen;
        unsigned int *alignedH;
        alignas(128) unsigned int c[257];
        alignas(128) unsigned long long code[256];
	alignas(128) unsigned int codeLen[256];
        WT *wt;

        unsigned int q;
        unsigned int p;
        unsigned int l;
        
	void freeMemory() {
            if (this->samSAMi != NULL) delete[] this->samSAMi;
            if (this->text != NULL) delete[] this->text;
            if (this->wt != NULL) delete this->wt;
            if (this->M != NULL) delete this->M;
            if (this->H != NULL) delete this->H;
        }
        
	void initialize() {
            this->wt = NULL;
            this->samSAMi = NULL;
            this->alignedSamSAMi = NULL;
            this->samSAMiLen = 0;
            this->text = NULL;
            this->alignedText = NULL;
            this->textLen = 0;
            this->M = NULL;
            this->alignedM = NULL;
            this->MLen = 0;
            this->H = NULL;
            this->alignedH = NULL;
            this->HLen = 0;
            for (int i = 0; i < 256; ++i) {
                    this->code[i] = 0;
                    this->codeLen[i] = 0;
            }
            for (int i = 0; i < 257; ++i) this->c[i] = 0;
        }
        
        void setQ(unsigned int q) {
            if (q == 0) {
                cout << "Error: not valid q value" << endl;
                exit(1);
            }
            this->q = q;
        }
        
        void setP(unsigned int p) {
            if (p == 0 || p > this->q) {
                cout << "Error: not valid p value" << endl;
                exit(1);
            }
            this->p = p;
        }
        
        void setL(unsigned int l) {
            if (l == 0) {
                cout << "Error: not valid l value" << endl;
                exit(1);
            }
            this->l = l;
        }
        
        void loadText(const char *textFileName) {
            if (this->verbose) cout << "Loading text ... " << flush;
            this->textLen = getFileSize(textFileName, sizeof(unsigned char));
            this->text = new unsigned char[this->textLen + this->q + 128 + 1];
            for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
            this->alignedText = this->text + this->q;
            while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
            FILE *inFile;
            inFile = fopen(textFileName, "rb");
            size_t result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
            this->alignedText[this->textLen] = '\0';
            if (result != this->textLen) {
                    cout << "Error loading text from " << textFileName << endl;
                    exit(1);
            }
            fclose(inFile);
            checkNullChar(this->alignedText, this->textLen);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void build_samsami(unsigned int *sa, unsigned int saLen) {
            if (this->verbose) cout << "Building SamSAMi ... " << flush;
            bool *markers = new bool[this->textLen];
            for (unsigned int i = 0; i < this->textLen; ++i) markers[i] = false;

            this->samSAMiLen = 1;
            unsigned char *minimizer = new unsigned char[this->p + 1];
            unsigned char *curr = new unsigned char[this->p + 1];
            minimizer[this->p] = '\0';
            curr[this->p] = '\0';
            int pos = -1, prevPos = -1;

            for (unsigned int i = 0; i < this->textLen - this->q + 1; ++i) {
                    for (unsigned int j = 0; j < this->p; ++j) minimizer[j] = (unsigned char)255;
                    for (unsigned int j = i; j < i + this->q - this->p + 1; ++j) {
                            strncpy((char *)curr, (const char *)(this->alignedText + j), this->p);
                            if (strncmp((char *)curr, (const char *)minimizer, this->p) < 0) {
                                    strcpy((char *)minimizer, (const char *)curr);
                                    pos = j;
                            }
                    }
                    if (pos > prevPos) {
                            markers[pos] = true;
                            ++(this->samSAMiLen);
                            prevPos = pos;
                    }
            }

            this->MLen = saLen / 32;
            if (saLen % 32) ++this->MLen;
            this->M = new unsigned int[this->MLen + 32];
            this->alignedM = this->M;
            while ((unsigned long long)this->alignedM % 128) ++this->alignedM;
            for (unsigned int i = 0; i < this->MLen; ++i) this->alignedM[i] = 0;

            this->HLen = this->samSAMiLen / this->l;
            if (this->samSAMiLen % this->l) ++this->HLen;
            this->H = new unsigned int[this->HLen + 32];
            this->alignedH = this->H;
            while ((unsigned long long)this->alignedH % 128) ++this->alignedH;
            for (unsigned int i = 0; i < this->HLen; ++i) this->alignedH[i] = 0;

            this->samSAMi = new unsigned int[this->samSAMiLen + 32];
            this->alignedSamSAMi = this->samSAMi;
            while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
            this->alignedSamSAMi[0] = sa[0];
            unsigned int samSAMiCounter = 1, HCounter = 1;
            for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) {
                if ((samSAMiCounter % this->l) == 0) this->alignedH[HCounter++] = i;
                this->alignedSamSAMi[samSAMiCounter++] = sa[i];
                this->alignedM[i / 32] += (1 << (i % 32));
            }
            if (this->verbose) cout << "Done" << endl;

            delete[] minimizer;
            delete[] curr;
            delete[] markers;
        }
        
        unsigned int getSAPos(unsigned int samSAMiPos) {
            if (samSAMiPos % this->l) {
            unsigned int saPos = this->alignedH[samSAMiPos / this->l];
            unsigned int rest = samSAMiPos % this->l;
            unsigned int startBitInBlock = (saPos + 1) % 32;
            unsigned int setBits = 0;
            unsigned int bitsBlock;
            if (startBitInBlock) {
                bitsBlock = this->alignedM[saPos / 32];
                setBits = __builtin_popcount(bitsBlock >> startBitInBlock);
                if (rest <= setBits) {
                    for (unsigned int i = startBitInBlock; i < 32; ++i) {
                        if ((bitsBlock >> i) & 1) {
                            if (rest == 1) return saPos + (i - startBitInBlock) + 1;
                            --rest;
                        }
                    }
                }
            }
            unsigned int MCounter = (saPos / 32) + 1;
            while (rest > setBits) {
                rest -= setBits;
                setBits = __builtin_popcount(this->alignedM[MCounter++]);
            }
            --MCounter;
            bitsBlock = this->alignedM[MCounter];
            for (unsigned int i = 0; i < 32; ++i) {
                if ((bitsBlock >> i) & 1) {
                    if (rest == 1) return 32 * MCounter + i;
                    --rest;
                }
            }
        }
        return this->alignedH[samSAMiPos / this->l];
        }
        
        
public:
	SamSAMiFM() {
		this->initialize();
                this->setQ(4);
                this->setP(1);
                this->setL(16);
	}

	SamSAMiFM(unsigned int q, unsigned int p, unsigned int l) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
                this->setL(l);
	}

	~SamSAMiFM() {
		this->free();
	}

	void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            fillArrayC(this->alignedText, this->textLen, this->c, this->verbose);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);

            unsigned int bwtLen;
            unsigned char *bwt = getBWT(this->alignedText, this->textLen, sa, saLen, bwtLen, 0, this->verbose);
            if (this->verbose) cout << "Huffman encoding ... " << flush;
            encodeHuffFromText(2, bwt, bwtLen, this->code, this->codeLen);
            if (this->verbose) cout << "Done" << endl;
            if (this->verbose) cout << "Building WT ... " << flush;
            switch(T) {
                case SamSAMiFMType::TYPE_1024:
                    this->wt = createWT2_1024_counter32(bwt, bwtLen, 0, this->code, this->codeLen);
                    break;
                default:
                    this->wt = createWT2_512_counter40(bwt, bwtLen, 0, this->code, this->codeLen);
                    break;
            }
            delete[] bwt;
            if (this->verbose) cout << "Done" << endl;

            this->build_samsami(sa, saLen);

            delete[] sa;
        }
        
	void save(const char *fileName) {
            if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
            bool nullPointer = false;
            bool notNullPointer = true;
            FILE *outFile;
            outFile = fopen(fileName, "w");
            fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
            fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->l, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
            fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
            fwrite(this->c, (size_t)sizeof(unsigned int), (size_t)257, outFile);
            fwrite(this->code, (size_t)sizeof(unsigned long long), (size_t)256, outFile);
            fwrite(this->codeLen, (size_t)sizeof(unsigned int), (size_t)256, outFile);
            fwrite(&this->MLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->MLen > 0) fwrite(this->alignedM, (size_t)sizeof(unsigned int), (size_t)this->MLen, outFile);
            fwrite(&this->HLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->HLen > 0) fwrite(this->alignedH, (size_t)sizeof(unsigned int), (size_t)this->HLen, outFile);
            if (this->wt == NULL) fwrite(&nullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
            else {
                    fwrite(&notNullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
                    this->wt->save(outFile);
            }
            fclose(outFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void load(const char *fileName) {
            this->free();
            bool isNotNullPointer;
            FILE *inFile;
            inFile = fopen(fileName, "rb");
            size_t result;
            result = fread(&this->verbose, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->verbose) cout << "Loading index from " << fileName << " ... " << flush;
            result = fread(&this->q, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->p, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->l, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->samSAMiLen > 0) {
                    this->samSAMi = new unsigned int[this->samSAMiLen + 32];
                    this->alignedSamSAMi = this->samSAMi;
                    while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
                    result = fread(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, inFile);
                    if (result != this->samSAMiLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->textLen > 0) {
                    this->text = new unsigned char[this->textLen + this->q + 128 + 1];
                    for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
                    this->alignedText = this->text + this->q;
                    while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
                    result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
                    this->alignedText[this->textLen] = '\0';
                    if (result != this->textLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(this->c, (size_t)sizeof(unsigned int), (size_t)257, inFile);
            if (result != 257) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(this->code, (size_t)sizeof(unsigned long long), (size_t)256, inFile);
            if (result != 256) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(this->codeLen, (size_t)sizeof(unsigned int), (size_t)256, inFile);
            if (result != 256) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->MLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->MLen > 0) {
                    this->M = new unsigned int[this->MLen + 32];
                    this->alignedM = this->M;
                    while ((unsigned long long)this->alignedM % 128) ++this->alignedM;
                    result = fread(this->alignedM, (size_t)sizeof(unsigned int), (size_t)this->MLen, inFile);
                    if (result != this->MLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->HLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->HLen > 0) {
                    this->H = new unsigned int[this->HLen + 32];
                    this->alignedH = this->H;
                    while ((unsigned long long)this->alignedH % 128) ++this->alignedH;
                    result = fread(this->alignedH, (size_t)sizeof(unsigned int), (size_t)this->HLen, inFile);
                    if (result != this->HLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&isNotNullPointer, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (isNotNullPointer) {
                    this->wt = new WT();
                    this->wt->load(inFile);
            }
            fclose(inFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void free() {
            this->freeMemory();
            this->initialize();
        }
        
	unsigned int getIndexSize() {
            unsigned int size = sizeof(this->q) + sizeof(this->p) + sizeof(this->l) + sizeof(this->samSAMiLen) + sizeof(this->wt) + sizeof(this->MLen) + sizeof(HLen) + 257 * sizeof(unsigned int) + 256 * sizeof(unsigned int) + 256 * sizeof(unsigned long long);
            if (this->samSAMiLen > 0) size += (this->samSAMiLen + 32) * sizeof(unsigned int);
            if (this->textLen > 0) size += (this->textLen + this->q + 128 + 1) * sizeof(unsigned char);
            if (this->MLen > 0) size += (this->MLen + 32) * sizeof(unsigned int);
            if (this->HLen > 0) size += (this->HLen + 32) * sizeof(unsigned int);
            if (this->wt != NULL) size += this->wt->getWTSize();
            return size;
        }
        
	unsigned int getTextSize() {
            return this->textLen * sizeof(unsigned char);
        }
        
        unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
            }
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {  
                if (beg == end) return 0;
                switch(T) {
                    case SamSAMiFMType::TYPE_1024:
                        return count_WT2_1024_counter32(pattern, pos, this->c, this->wt, this->getSAPos(beg) + 1, this->getSAPos(end - 1) + 1, this->code, this->codeLen);
                        break;
                    default:
                        return count_WT2_512_counter40(pattern, pos, this->c, this->wt, this->getSAPos(beg) + 1, this->getSAPos(end - 1) + 1, this->code, this->codeLen);
                        break;
                } 
            }
        }
        
};

template<SamSAMiFMType T, HTType HASHTYPE> class SamSAMiFMHash : public SamSAMiFM<T> {
private:
        unsigned int minPatternLenForHash;
	HT<HASHTYPE> *ht = NULL;

	void freeMemory() {
            SamSAMiFM<T>::freeMemory();
            if (this->ht != NULL) this->ht->free();
        }
        
        void setMinPatternLenForHash() {
            if (this->q > (this->ht->k + this->q - this->p)) this->minPatternLenForHash = this->q;
            else this->minPatternLenForHash = this->ht->k + this->q - this->p;
        }
        
	void build_samsami(unsigned int *sa, unsigned int saLen) {
            SamSAMiFM<T>::build_samsami(sa, saLen);
            if (this->ht != NULL) {
                if (this->verbose) cout << "Building hash table ... " << flush;
                this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
                if (this->verbose) cout << "Done" << endl;
            }
        }
        
public:
	enum IndexType {
		TYPE_512 = 8,
		TYPE_1024 = 16
	};

	SamSAMiFMHash(unsigned int q, unsigned int p, unsigned int l, unsigned int k, double loadFactor) {
		this->initialize();
                this->setQ(q);
                this->setP(p);
                this->setL(l);
                this->ht = new HT<HASHTYPE>(k, loadFactor);
                this->setMinPatternLenForHash();
	}

	~SamSAMiFMHash() {
		this->freeMemory();
                if (this->ht != NULL) delete this->ht;
	}
        
        void build(const char *textFileName) {
            this->free();
            this->loadText(textFileName);
            fillArrayC(this->alignedText, this->textLen, this->c, this->verbose);
            unsigned int saLen;
            unsigned int *sa = getSA(textFileName, this->alignedText, this->textLen, saLen, 0, this->verbose);

            unsigned int bwtLen;
            unsigned char *bwt = getBWT(this->alignedText, this->textLen, sa, saLen, bwtLen, 0, this->verbose);
            if (this->verbose) cout << "Huffman encoding ... " << flush;
            encodeHuffFromText(2, bwt, bwtLen, this->code, this->codeLen);
            if (this->verbose) cout << "Done" << endl;
            if (this->verbose) cout << "Building WT ... " << flush;
            switch(T) {
                case SamSAMiFMType::TYPE_1024:
                    this->wt = createWT2_1024_counter32(bwt, bwtLen, 0, this->code, this->codeLen);
                    break;
                default:
                    this->wt = createWT2_512_counter40(bwt, bwtLen, 0, this->code, this->codeLen);
                    break;
            }
            delete[] bwt;
            if (this->verbose) cout << "Done" << endl;

            this->build_samsami(sa, saLen);

            delete[] sa;
        }

	void save(const char *fileName) {
            if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
            bool nullPointer = false;
            bool notNullPointer = true;
            FILE *outFile;
            outFile = fopen(fileName, "w");
            fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
            fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->l, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
            fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
            fwrite(this->c, (size_t)sizeof(unsigned int), (size_t)257, outFile);
            fwrite(this->code, (size_t)sizeof(unsigned long long), (size_t)256, outFile);
            fwrite(this->codeLen, (size_t)sizeof(unsigned int), (size_t)256, outFile);
            fwrite(&this->MLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->MLen > 0) fwrite(this->alignedM, (size_t)sizeof(unsigned int), (size_t)this->MLen, outFile);
            fwrite(&this->HLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
            if (this->HLen > 0) fwrite(this->alignedH, (size_t)sizeof(unsigned int), (size_t)this->HLen, outFile);
            if (this->wt == NULL) fwrite(&nullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
            else {
                    fwrite(&notNullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
                    this->wt->save(outFile);
            }
            this->ht->save(outFile);
            fclose(outFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	void load(const char *fileName) {
            this->free();
            if (this->ht != NULL) {
                    delete this->ht;
                    this->ht = NULL;
            }
            bool isNotNullPointer;
            FILE *inFile;
            inFile = fopen(fileName, "rb");
            size_t result;
            result = fread(&this->verbose, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->verbose) cout << "Loading index from " << fileName << " ... " << flush;
            result = fread(&this->q, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->p, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->l, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->samSAMiLen > 0) {
                    this->samSAMi = new unsigned int[this->samSAMiLen + 32];
                    this->alignedSamSAMi = this->samSAMi;
                    while ((unsigned long long)this->alignedSamSAMi % 128) ++this->alignedSamSAMi;
                    result = fread(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, inFile);
                    if (result != this->samSAMiLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->textLen > 0) {
                    this->text = new unsigned char[this->textLen + this->q + 128 + 1];
                    for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
                    this->alignedText = this->text + this->q;
                    while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
                    result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
                    this->alignedText[this->textLen] = '\0';
                    if (result != this->textLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(this->c, (size_t)sizeof(unsigned int), (size_t)257, inFile);
            if (result != 257) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(this->code, (size_t)sizeof(unsigned long long), (size_t)256, inFile);
            if (result != 256) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(this->codeLen, (size_t)sizeof(unsigned int), (size_t)256, inFile);
            if (result != 256) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            result = fread(&this->MLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->MLen > 0) {
                    this->M = new unsigned int[this->MLen + 32];
                    this->alignedM = this->M;
                    while ((unsigned long long)this->alignedM % 128) ++this->alignedM;
                    result = fread(this->alignedM, (size_t)sizeof(unsigned int), (size_t)this->MLen, inFile);
                    if (result != this->MLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&this->HLen, (size_t)sizeof(unsigned int), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (this->HLen > 0) {
                    this->H = new unsigned int[this->HLen + 32];
                    this->alignedH = this->H;
                    while ((unsigned long long)this->alignedH % 128) ++this->alignedH;
                    result = fread(this->alignedH, (size_t)sizeof(unsigned int), (size_t)this->HLen, inFile);
                    if (result != this->HLen) {
                            cout << "Error loading index from " << fileName << endl;
                            exit(1);
                    }
            }
            result = fread(&isNotNullPointer, (size_t)sizeof(bool), (size_t)1, inFile);
            if (result != 1) {
                    cout << "Error loading index from " << fileName << endl;
                    exit(1);
            }
            if (isNotNullPointer) {
                    this->wt = new WT();
                    this->wt->load(inFile);
            }
            delete this->ht;
            this->ht = new HT<HASHTYPE>();
            this->ht->load(inFile);
            this->setMinPatternLenForHash();
            fclose(inFile);
            if (this->verbose) cout << "Done" << endl;
        }
        
	unsigned int getIndexSize() {
            return SamSAMiFM<T>::getIndexSize() + sizeof(this->minPatternLenForHash) + sizeof(this->ht) + this->ht->getHTSize();
        }
        
        void free() {
            this->freeMemory();
            this->initialize();
        }

	unsigned int count(unsigned char *pattern, unsigned int patternLen) {
            if (patternLen < this->minPatternLenForHash) return SamSAMiFM<T>::count(pattern, patternLen);
            unsigned int beg, end, pos = 0;
            for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
                if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
                    pos = i;
                }
            }
            unsigned int leftBoundary, rightBoundary;
            this->ht->getBoundaries(pattern + pos, this->alignedText, this->alignedSamSAMi, leftBoundary, rightBoundary);
            binarySearch(this->alignedSamSAMi, this->alignedText, leftBoundary, rightBoundary, pattern + pos, patternLen - pos, beg, end);
            if (pos == 0) return end - beg;
            else {
                if (beg == end) return 0;
                switch(T) {
                    case SamSAMiFMType::TYPE_1024:
                        return count_WT2_1024_counter32(pattern, pos, this->c, this->wt, this->getSAPos(beg) + 1, this->getSAPos(end - 1) + 1, this->code, this->codeLen);
                        break;
                    default:
                        return count_WT2_512_counter40(pattern, pos, this->c, this->wt, this->getSAPos(beg) + 1, this->getSAPos(end - 1) + 1, this->code, this->codeLen);
                        break;
                }
            }
        }
};

void markSketch_8x2(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex) {
	unsigned int k = 8;
	if (textIndex < 8) k = textIndex;
	for (unsigned int i = 1; i <= k; ++i) {
		sketches[sketchesIndex] += (((text[textIndex - i] >> bitShift) & 3) << (16 - 2 * i));
	}
	for (unsigned int i = k; i < 8; ++i) {
		sketches[sketchesIndex] += (3 << (16 - 2 * (i + 1)));
	}
}

void markSketch_4x4(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex) {
	unsigned int k = 4;
	if (textIndex < 4) k = textIndex;
	for (unsigned int i = 1; i <= k; ++i) {
		sketches[sketchesIndex] += (((text[textIndex - i] >> bitShift) & 15) << (16 - 4 * i));
	}
	for (unsigned int i = k; i < 4; ++i) {
		sketches[sketchesIndex] += (15 << (16 - 4 * (i + 1)));
	}
}

unsigned int getBitShift_8x2(unsigned char *text, unsigned int textLen) {
	unsigned int charCountsInText[256];
	for (int i = 0; i < 256; ++i) charCountsInText[i] = 0;
	for (unsigned int i = 0; i < textLen; ++i) ++charCountsInText[text[i]];
        
	unsigned int avg = textLen / 4;
	unsigned int subAlphCounts[4];
	unsigned int curr = 0;
	unsigned int mask;
	long long bestSum = -1;
        unsigned int bitShift = 0;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 4; ++j) {
			subAlphCounts[j] = 0;
		}
		mask = (1 << i) + (1 << (i + 1));
		for (unsigned int ch = 0; ch < 256; ++ch) {
			subAlphCounts[((unsigned char)(ch & mask)) >> i] += charCountsInText[ch];
		}
		curr = 0;
		for (int j = 0; j < 4; ++j) {
			if (subAlphCounts[j] >= avg) curr += (subAlphCounts[j] - avg);
			else curr += (avg - subAlphCounts[j]);
		}
		if ((curr < bestSum) || (bestSum == -1)) {
			bestSum = curr;
			bitShift = i;
		}
	}
        return bitShift;
}

unsigned int getBitShift_4x4(unsigned char *text, unsigned int textLen) {
	unsigned int charCountsInText[256];
	for (int i = 0; i < 256; ++i) charCountsInText[i] = 0;
	for (unsigned int i = 0; i < textLen; ++i) ++charCountsInText[text[i]];
        
	unsigned int avg = textLen / 16;
	unsigned int subAlphCounts[16];
	unsigned int curr = 0;
	unsigned int mask;
	long long bestSum = -1;
        unsigned int bitShift = 0;
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 16; ++j) {
			subAlphCounts[j] = 0;
		}
		mask = (1 << i) + (1 << (i + 1)) + (1 << (i + 2)) + (1 << (i + 3));
		for (unsigned int ch = 0; ch < 256; ++ch) {
			subAlphCounts[((unsigned char)(ch & mask)) >> i] += charCountsInText[ch];
		}
		curr = 0;
		for (int j = 0; j < 16; ++j) {
			if (subAlphCounts[j] >= avg) curr += (subAlphCounts[j] - avg);
			else curr += (avg - subAlphCounts[j]);
		}
		if ((curr < bestSum) || (bestSum == -1)) {
			bestSum = curr;
			bitShift = i;
		}
	}
        return bitShift;
}

unsigned int getPatternSketch_8x2(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int &sketchLen) {
        sketchLen = 8;
        if (pos < 8) sketchLen = pos;
        unsigned int patternSketch = 0;
        for (unsigned int i = 1; i <= sketchLen; ++i) {
                patternSketch += (((pattern[pos - i] >> bitShift) & 3) << (2 * (sketchLen - i)));
        }
        return patternSketch;
}

bool isSketchEqual_8x2(unsigned int patternSketch, unsigned int sketchLen, unsigned int sketch, unsigned int i) {
	if ((i & 1) == 0) sketch = sketch >> 16;
	else sketch = sketch & 0x0000FFFF;
	if ((sketch >> (2 * (8 - sketchLen))) != patternSketch) return false;
	return true;
}

unsigned int getPatternSketch_4x4(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int &sketchLen) {
        sketchLen = 4;
        if (pos < 4) sketchLen = pos;
        unsigned int patternSketch = 0;
        for (unsigned int i = 1; i <= sketchLen; ++i) {
                patternSketch += (((pattern[pos - i] >> bitShift) & 15) << (4 * (sketchLen - i)));
        }
        return patternSketch;
}

bool isSketchEqual_4x4(unsigned int patternSketch, unsigned int sketchLen, unsigned int sketch, unsigned int i) {
	if ((i & 1) == 0) sketch = sketch >> 16;
	else sketch = sketch & 0x0000FFFF;
	if ((sketch >> (4 * (4 - sketchLen))) != patternSketch) return false;
	return true;
}

unsigned int getUniqueSuffixNumForSamSAMi2(unsigned int k, unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen, vector<unsigned char> selectedChars) {
	unsigned int uniqueSuffixNum = 0;
	bool isSelectedChars = (selectedChars.size() != 0);

	unsigned char *lastPattern = new unsigned char[k + 1];
	for (unsigned int i = 0; i < k; ++i) lastPattern[i] = 255;
	lastPattern[k] = '\0';

	unsigned char *pattern = new unsigned char[k + 1];

	for (unsigned int i = 0; i < saLen; i++) {
		if ((sa[i] & 0x0FFFFFFF) > (textLen - k)) continue;
		strncpy((char *)pattern, (const char*)(text + (sa[i] & 0x0FFFFFFF)), k);
		pattern[k] = '\0';
		if (strcmp((char *)pattern, (const char*)lastPattern) == 0) continue;
		else {
			strcpy((char *)lastPattern, (const char*)pattern);
			if (isSelectedChars) {
				bool rejectPattern = false;
				for (unsigned int j = 0; j < k; ++j) {
					bool symbolInSelectedChars = false;
					for (vector<unsigned char>::iterator it = selectedChars.begin(); it != selectedChars.end(); ++it) {
						if ((unsigned int)pattern[j] == (*it)) {
							symbolInSelectedChars = true;
							break;
						}
					}
					if (!symbolInSelectedChars) {
						rejectPattern = true;
						break;
					}
				}
				if (rejectPattern) continue;
			}
			++uniqueSuffixNum;
		}
	}

	delete[] lastPattern;
	delete[] pattern;

	return uniqueSuffixNum;
}

void fillLUT2ForSamSAMi2(unsigned int lut2[256][256][2], unsigned char *text, unsigned int *sa, unsigned int saLen) {
	unsigned char lutPattern[3];
	lutPattern[2] = '\0';
	for (int i = 0; i < 256; ++i) {
		lutPattern[0] = (unsigned char)i;
		for (int j = 0; j < 256; ++j) {
			lutPattern[1] = (unsigned char)j;
			binarySearchForSamSAMi2(sa, text, 0, saLen, lutPattern, 2, lut2[i][j][0], lut2[i][j][1]);
		}
	}
}

void binarySearchForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end) {
	if (pattern[patternLength - 1] == 255) binarySearchStrncmpForSamSAMi2(sa, text, lStart,rStart, pattern, patternLength, beg, end);
	else binarySearchAStrcmpForSamSAMi2(sa, text, lStart,rStart, pattern, patternLength, beg, end);
}

void binarySearchAStrcmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end) {
	unsigned int l = lStart;
	unsigned int r = rStart;
	unsigned int mid;
	while (l < r) {
		mid = (l + r) / 2;
		if (A_strcmp((const char*)pattern, (const char*)(text + (sa[mid] & 0x0FFFFFFF))) > 0) {
			l = mid + 1;
		}
		else {
			r = mid;
		}
	}
	beg = l;
	r = rStart;
	++pattern[patternLength - 1];
	while (l < r) {
		mid = (l + r) / 2;
		if (A_strcmp((const char*)pattern, (const char*)(text + (sa[mid] & 0x0FFFFFFF))) <= 0) {
			r = mid;
		}
		else {
			l = mid + 1;
		}
	}
	--pattern[patternLength - 1];
	end = r;
}

void binarySearchStrncmpForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end) {
	unsigned int l = lStart;
	unsigned int r = rStart;
	unsigned int mid;
	while (l < r) {
		mid = (l + r) / 2;
		if (strncmp((const char*)pattern, (const char*)(text + (sa[mid] & 0x0FFFFFFF)), patternLength) > 0) {
			l = mid + 1;
		}
		else {
			r = mid;
		}
	}
	beg = l;
	r = rStart;
	while (l < r) {
		mid = (l + r) / 2;
		if (strncmp((const char*)pattern, (const char*)(text + (sa[mid] & 0x0FFFFFFF)), patternLength) < 0) {
			r = mid;
		}
		else {
			l = mid + 1;
		}
	}
	end = r;
}

}

#endif	/* SAMSAMI_HPP */
