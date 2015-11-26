#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "libs/asmlib.h"
#include "samsami.h"

using namespace std;

namespace samsami {

/*SAMSAMI1*/

void SamSAMi1::setType(int indexType) {
	if (indexType != SamSAMi1::STANDARD && indexType != SamSAMi1::WITH_SKETCHES) {
		cout << "Error: not valid index type" << endl;
		exit(1);
	}
	this->type = indexType;
}

void SamSAMi1::setQ(unsigned int q) {
	if (q == 0) {
		cout << "Error: not valid q value" << endl;
		exit(1);
	}
	this->q = q;
}

void SamSAMi1::setP(unsigned int p) {
	if (p == 0 || p > this->q) {
		cout << "Error: not valid p value" << endl;
		exit(1);
	}
	this->p = p;
}

void SamSAMi1::setMinPatternLenForHash() {
        if (this->q > (this->ht->k + this->q - this->p)) this->minPatternLenForHash = this->q;
        else this->minPatternLenForHash = this->ht->k + this->q - this->p;
}

void SamSAMi1::setFunctions() {
	if (this->ht != NULL) {
		switch (this->type) {
		case SamSAMi1::STANDARD:
                        this->builder = &SamSAMi1::build_std;
                        this->countOperation = &SamSAMi1::count_std_hash;
			break;
		case SamSAMi1::WITH_SKETCHES:
                        this->builder = &SamSAMi1::build_sketches;
                        this->countOperation = &SamSAMi1::count_sketches_hash;
			break;
		default:
			cout << "Error: not valid index type" << endl;
			exit(1);
		}
	} else {
		switch (this->type) {
		case SamSAMi1::STANDARD:
                        this->builder = &SamSAMi1::build_std;
                        this->countOperation = &SamSAMi1::count_std;
			break;
		case SamSAMi1::WITH_SKETCHES:
                        this->builder = &SamSAMi1::build_sketches;
                        this->countOperation = &SamSAMi1::count_sketches;
			break;
		default:
			cout << "Error: not valid index type" << endl;
			exit(1);
		}
	}
}

void SamSAMi1::free() {
	this->freeMemory();
	this->initialize();
}

void SamSAMi1::initialize() {
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

void SamSAMi1::freeMemory() {
	if (this->samSAMi != NULL) delete[] this->samSAMi;
        if (this->text != NULL) delete[] this->text;
        if (this->sketches != NULL) delete[] this->sketches;
	if (this->ht != NULL) this->ht->free();
}

void SamSAMi1::build(unsigned char* text, unsigned int textLen) {
	checkNullChar(text, textLen);
	this->free();
        if (this->verbose) cout << "Reading text ... " << flush;
	this->textLen = textLen;
        this->text = new unsigned char [this->textLen + this->q + 128];
        for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
        this->alignedText = this->text + this->q;
        while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
        for (unsigned int i = 0; i < this->textLen; ++i) this->alignedText[i] = text[i];
        if (this->verbose) cout << "Done" << endl;
        
        unsigned int saLen;
        unsigned int *sa = getSA(this->alignedText, this->textLen, saLen, 0, this->verbose);
        
	(this->*builder)(sa, saLen);

	delete[] sa;
}

void SamSAMi1::build_std(unsigned int* sa, unsigned int saLen) {
        if (this->verbose) cout << "Creating SamSAMi ... " << flush;
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
        
        if (this->ht != NULL) {
                if (this->verbose) cout << "Creating hash table ... " << flush;
		this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
		if (this->verbose) cout << "Done" << endl;
	}
	
        delete[] minimizer;
	delete[] curr;
	delete[] markers;
}

void SamSAMi1::build_sketches(unsigned int* sa, unsigned int saLen) {
        if (this->verbose) cout << "Creating SamSAMi ... " << flush;
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
        this->bitShift = getBitShift(this->alignedText, this->textLen);
	unsigned int *sketchesTemp = new unsigned int[sketchesTempLen];
	for (unsigned int i = 0; i < sketchesTempLen; ++i) sketchesTemp[i] = 0;
	unsigned int sketchesCounter = 0;
	markSketch(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
	++sketchesCounter;
        
	for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) {
            this->alignedSamSAMi[samSAMiCounter++] = sa[i];
            markSketch(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
	    ++sketchesCounter;
        }
        
        this->sketchesLen = sketchesTempLen / 2;
	this->sketches = new unsigned int[this->sketchesLen + 32];
        this->alignedSketches = this->sketches;
        while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
	for (unsigned int i = 0; i < this->sketchesLen; ++i) this->alignedSketches[i] = (sketchesTemp[2 * i] << 16) + sketchesTemp[2 * i + 1];
        if (this->verbose) cout << "Done" << endl;
        
        if (this->ht != NULL) {
                if (this->verbose) cout << "Creating hash table ... " << flush;
		this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
		if (this->verbose) cout << "Done" << endl;
	}
	
        delete[] minimizer;
	delete[] curr;
	delete[] markers;
}

unsigned int SamSAMi1::getIndexSize() {
	unsigned int size = sizeof(this->type) + sizeof(this->q) + sizeof(this->p) + sizeof(this->samSAMiLen) + sizeof(this->sketchesLen) + sizeof(this->bitShift) + sizeof(this->minPatternLenForHash) + sizeof(this->ht);
        if (this->samSAMiLen > 0) size += (this->samSAMiLen + 32) * sizeof(unsigned int);
        if (this->textLen > 0) size += (this->textLen + this->q + 128) * sizeof(unsigned char);
        if (this->sketchesLen > 0) size += (this->sketchesLen + 32) * sizeof(unsigned int);
        if (this->ht != NULL) size += this->ht->getHTSize();
	return size;
}

unsigned int SamSAMi1::getTextSize() {
	return this->textLen * sizeof(unsigned char);
}

unsigned int SamSAMi1::count_std(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
        }
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0;
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

unsigned int SamSAMi1::count_sketches(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
        }
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0;
	for (unsigned int i = 1; i < this->q - this->p + 1; ++i) {
		if (strncmp((const char *)(pattern + i), (const char *)(pattern + pos), this->p) < 0) {
			pos = i;
		}
	}
	binarySearch(this->alignedSamSAMi, this->alignedText, 0, this->samSAMiLen, pattern + pos, patternLen - pos, beg, end);
	if (pos == 0) return end - beg;
	else {
		for (unsigned int i = beg; i < end; ++i) {
			if (isSketchEqual(this->bitShift, pattern, pos, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
		}
	}
	return count;
}

unsigned int SamSAMi1::count_std_hash(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->minPatternLenForHash) return this->count_std(pattern, patternLen);
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0;
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

unsigned int SamSAMi1::count_sketches_hash(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->minPatternLenForHash) return this->count_sketches(pattern, patternLen);
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0;
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
			if (isSketchEqual(this->bitShift, pattern, pos, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + this->alignedSamSAMi[i] - pos), pos) == 0) ++count;
		}
	}
	return count;
}

unsigned int SamSAMi1::count(unsigned char *pattern, unsigned int patternLen) {
	return (this->*countOperation)(pattern, patternLen);
}

unsigned int *SamSAMi1::locate(unsigned char *pattern, unsigned int patternLen) {
	return 0;
}

void SamSAMi1::save(const char *fileName) {
	if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
	bool nullPointer = false;
	bool notNullPointer = true;
	FILE *outFile;
	outFile = fopen(fileName, "w");
	fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
	fwrite(&this->type, (size_t)sizeof(int), (size_t)1, outFile);
        fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
        fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
        fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
        fwrite(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->sketchesLen > 0) fwrite(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, outFile);
	fwrite(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, outFile);
        if (this->ht == NULL) fwrite(&nullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
        else {
                fwrite(&notNullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
                this->ht->save(outFile);
        }
	fclose(outFile);
	if (this->verbose) cout << "Done" << endl;
}

void SamSAMi1::load(const char *fileName) {
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
	result = fread(&this->type, (size_t)sizeof(int), (size_t)1, inFile);
	if (result != 1) {
		cout << "Error loading index from " << fileName << endl;
		exit(1);
	}
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
		this->text = new unsigned char[this->textLen + this->q + 128];
                for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
		this->alignedText = this->text + this->q;
		while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
		result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
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
	result = fread(&isNotNullPointer, (size_t)sizeof(bool), (size_t)1, inFile);
	if (result != 1) {
		cout << "Error loading index from " << fileName << endl;
		exit(1);
	}
	if (isNotNullPointer) {
                this->ht = new HT();
                this->ht->load(inFile);
                this->setMinPatternLenForHash();
	}
	fclose(inFile);
	this->setFunctions();
	if (this->verbose) cout << "Done" << endl;
}

/*SAMSAMI2*/

void SamSAMi2::setType(int indexType) {
	if (indexType != SamSAMi2::STANDARD && indexType != SamSAMi2::WITH_SKETCHES) {
		cout << "Error: not valid index type" << endl;
		exit(1);
	}
	this->type = indexType;
}

void SamSAMi2::setQ(unsigned int q) {
	if (q == 0) {
		cout << "Error: not valid q value" << endl;
		exit(1);
	}
	this->q = q;
}

void SamSAMi2::setP(unsigned int p) {
	if (p == 0 || p > this->q) {
		cout << "Error: not valid p value" << endl;
		exit(1);
	}
	this->p = p;
}

void SamSAMi2::setMinPatternLenForHash() {
        if (this->q > (this->ht->k + this->q - this->p)) this->minPatternLenForHash = this->q;
        else this->minPatternLenForHash = this->ht->k + this->q - this->p;
}

void SamSAMi2::setFunctions() {
	if (this->ht != NULL) {
		switch (this->type) {
		case SamSAMi2::STANDARD:
                        this->builder = &SamSAMi2::build_std;
                        this->countOperation = &SamSAMi2::count_std_hash;
			break;
		case SamSAMi2::WITH_SKETCHES:
                        this->builder = &SamSAMi2::build_sketches;
                        this->countOperation = &SamSAMi2::count_sketches_hash;
			break;
		default:
			cout << "Error: not valid index type" << endl;
			exit(1);
		}
	} else {
		switch (this->type) {
		case SamSAMi1::STANDARD:
                        this->builder = &SamSAMi2::build_std;
                        this->countOperation = &SamSAMi2::count_std;
			break;
		case SamSAMi1::WITH_SKETCHES:
                        this->builder = &SamSAMi2::build_sketches;
                        this->countOperation = &SamSAMi2::count_sketches;
			break;
		default:
			cout << "Error: not valid index type" << endl;
			exit(1);
		}
	}
}

void SamSAMi2::free() {
	this->freeMemory();
	this->initialize();
}

void SamSAMi2::initialize() {
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

void SamSAMi2::freeMemory() {
	if (this->samSAMi != NULL) delete[] this->samSAMi;
        if (this->text != NULL) delete[] this->text;
        if (this->sketches != NULL) delete[] this->sketches;
	if (this->ht != NULL) this->ht->free();
}

void SamSAMi2::build(unsigned char* text, unsigned int textLen) {
	checkNullChar(text, textLen);
	this->free();
        if (this->verbose) cout << "Reading text ... " << flush;
	this->textLen = textLen;
        this->text = new unsigned char [this->textLen + this->q + 128];
        for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
        this->alignedText = this->text + this->q;
        while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
        for (unsigned int i = 0; i < this->textLen; ++i) this->alignedText[i] = text[i];
        if (this->verbose) cout << "Done" << endl;
        
        unsigned int saLen;
        unsigned int *sa = getSA(this->alignedText, this->textLen, saLen, 0, this->verbose);
        
	(this->*builder)(sa, saLen);

	delete[] sa;
}

void SamSAMi2::build_std(unsigned int* sa, unsigned int saLen) {
        if (this->verbose) cout << "Creating SamSAMi ... " << flush;
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
        
        if (this->ht != NULL) {
                if (this->verbose) cout << "Creating hash table ... " << flush;
		this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
		if (this->verbose) cout << "Done" << endl;
	}
        
        delete[] minimizer;
	delete[] curr;
	delete[] markers;
        delete[] positions;
}

void SamSAMi2::build_sketches(unsigned int* sa, unsigned int saLen) {
        if (this->verbose) cout << "Creating SamSAMi ... " << flush;
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
        this->bitShift = getBitShift(this->alignedText, this->textLen);
	unsigned int *sketchesTemp = new unsigned int[sketchesTempLen];
	for (unsigned int i = 0; i < sketchesTempLen; ++i) sketchesTemp[i] = 0;
	unsigned int sketchesCounter = 0;
	markSketch(this->alignedText, this->bitShift, sa[0], sketchesTemp, sketchesCounter);
	++sketchesCounter;

	for (unsigned int i = 1; i < saLen; ++i) if (markers[sa[i]]) {
            this->alignedSamSAMi[samSAMiCounter++] = (((unsigned int)sa[i]) | (positions[sa[i]] << 28));
            markSketch(this->alignedText, this->bitShift, sa[i], sketchesTemp, sketchesCounter);
	    ++sketchesCounter;
	}
        
        this->sketchesLen = sketchesTempLen / 2;
	this->sketches = new unsigned int[this->sketchesLen + 32];
        this->alignedSketches = this->sketches;
        while ((unsigned long long)this->alignedSketches % 128) ++this->alignedSketches;
	for (unsigned int i = 0; i < this->sketchesLen; ++i) this->alignedSketches[i] = (sketchesTemp[2 * i] << 16) + sketchesTemp[2 * i + 1];
        if (this->verbose) cout << "Done" << endl;
        
        if (this->ht != NULL) {
                if (this->verbose) cout << "Creating hash table ... " << flush;
		this->ht->build(this->alignedText, this->textLen, this->alignedSamSAMi, this->samSAMiLen);
		if (this->verbose) cout << "Done" << endl;
	}
        
        delete[] minimizer;
	delete[] curr;
	delete[] markers;
        delete[] positions;
        delete[] sketchesTemp;
}

unsigned int SamSAMi2::getIndexSize() {
	unsigned int size = sizeof(this->type) + sizeof(this->q) + sizeof(this->p) + sizeof(this->samSAMiLen) + sizeof(this->sketchesLen) + sizeof(this->bitShift) + sizeof(this->minPatternLenForHash) + sizeof(this->ht);
        if (this->samSAMiLen > 0) size += (this->samSAMiLen + 32) * sizeof(unsigned int);
        if (this->textLen > 0) size += (this->textLen + this->q + 128) * sizeof(unsigned char);
        if (this->sketchesLen > 0) size += (this->sketchesLen + 32) * sizeof(unsigned int);
        if (this->ht != NULL) size += this->ht->getHTSize();
	return size;
}

unsigned int SamSAMi2::getTextSize() {
	return this->textLen * sizeof(unsigned char);
}

unsigned int SamSAMi2::count_std(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
        }
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0, prevPos = 0;
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

unsigned int SamSAMi2::count_sketches(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->q) {
                cout << "Error: pattern length must be greater than " << (this->q - 1) << endl;
                exit(1);
        }
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0, prevPos = 0;
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
			if (isSketchEqual(this->bitShift, pattern, pos, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
		}
	}
	return count;
}

unsigned int SamSAMi2::count_std_hash(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->minPatternLenForHash) return this->count_std(pattern, patternLen);
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0, prevPos = 0;
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

unsigned int SamSAMi2::count_sketches_hash(unsigned char *pattern, unsigned int patternLen) {
        if (patternLen < this->minPatternLenForHash) return this->count_sketches(pattern, patternLen);
	unsigned int beg = 0, end = 0, count = 0;
	unsigned int pos = 0, prevPos = 0;
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
			if (isSketchEqual(this->bitShift, pattern, pos, this->alignedSketches[i / 2], i) && strncmp((const char *)pattern, (const char *)(this->alignedText + (this->alignedSamSAMi[i] & 0x0FFFFFFF) - pos), pos) == 0) ++count;
		}
	}
	return count;
}

unsigned int SamSAMi2::count(unsigned char *pattern, unsigned int patternLen) {
	return (this->*countOperation)(pattern, patternLen);
}

unsigned int *SamSAMi2::locate(unsigned char *pattern, unsigned int patternLen) {
	return 0;
}

void SamSAMi2::save(const char *fileName) {
	if (this->verbose) cout << "Saving index in " << fileName << " ... " << flush;
	bool nullPointer = false;
	bool notNullPointer = true;
	FILE *outFile;
	outFile = fopen(fileName, "w");
	fwrite(&this->verbose, (size_t)sizeof(bool), (size_t)1, outFile);
	fwrite(&this->type, (size_t)sizeof(int), (size_t)1, outFile);
        fwrite(&this->q, (size_t)sizeof(unsigned int), (size_t)1, outFile);
        fwrite(&this->p, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	fwrite(&this->samSAMiLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->samSAMiLen > 0) fwrite(this->alignedSamSAMi, (size_t)sizeof(unsigned int), (size_t)this->samSAMiLen, outFile);
        fwrite(&this->textLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->textLen > 0) fwrite(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, outFile);
        fwrite(&this->sketchesLen, (size_t)sizeof(unsigned int), (size_t)1, outFile);
	if (this->sketchesLen > 0) fwrite(this->alignedSketches, (size_t)sizeof(unsigned int), (size_t)this->sketchesLen, outFile);
	fwrite(&this->bitShift, (size_t)sizeof(unsigned int), (size_t)1, outFile);
        if (this->ht == NULL) fwrite(&nullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
        else {
                fwrite(&notNullPointer, (size_t)sizeof(bool), (size_t)1, outFile);
                this->ht->save(outFile);
        }
	fclose(outFile);
	if (this->verbose) cout << "Done" << endl;
}

void SamSAMi2::load(const char *fileName) {
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
	result = fread(&this->type, (size_t)sizeof(int), (size_t)1, inFile);
	if (result != 1) {
		cout << "Error loading index from " << fileName << endl;
		exit(1);
	}
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
		this->text = new unsigned char[this->textLen + this->q + 128];
                for (unsigned int i = 0; i < this->q; ++i) this->text[i] = '\0';
		this->alignedText = this->text + this->q;
		while ((unsigned long long)this->alignedText % 128) ++this->alignedText;
		result = fread(this->alignedText, (size_t)sizeof(unsigned char), (size_t)this->textLen, inFile);
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
	result = fread(&isNotNullPointer, (size_t)sizeof(bool), (size_t)1, inFile);
	if (result != 1) {
		cout << "Error loading index from " << fileName << endl;
		exit(1);
	}
	if (isNotNullPointer) {
                this->ht = new HTSamSAMi2();
                this->ht->load(inFile);
                this->setMinPatternLenForHash();
	}
	fclose(inFile);
	this->setFunctions();
	if (this->verbose) cout << "Done" << endl;
}

/*HTSAMSAMI2*/

void HTSamSAMi2::load(FILE *inFile) {
        this->loadBase(inFile);
        this->setFunctions();
}

void HTSamSAMi2::setFunctions() {
        switch (this->type) {
        case HT::STANDARD:
                this->getBoundariesOperation = &HTSamSAMi2::getBasicHTBoundaries;
                break;
        case HT::DENSE:
                this->getBoundariesOperation = &HTSamSAMi2::getDenseHTBoundaries;
                break;
        default:
                cout << "Error: not valid hash type" << endl;
                exit(1);
        }
}

void HTSamSAMi2::fillBasicHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
	unsigned long long hash = this->bucketsNum;
        this->boundariesHTLen = 2 * this->bucketsNum;
	this->boundariesHT = new unsigned int[this->boundariesHTLen + 32];
	this->alignedBoundariesHT = this->boundariesHT;
	while ((unsigned long long)(this->alignedBoundariesHT) % 128) ++(this->alignedBoundariesHT);

	for (unsigned long long i = 0; i < this->boundariesHTLen; ++i) this->alignedBoundariesHT[i] = HT::emptyValueHT;

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
			if (this->alignedBoundariesHT[2 * hash] == HT::emptyValueHT) {
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
void HTSamSAMi2::fillDenseHTData(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
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
            this->alignedBoundariesHT[i] = HT::emptyValueHT;
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
			if (this->alignedBoundariesHT[hash] == HT::emptyValueHT) {
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


void HTSamSAMi2::build(unsigned char *text, unsigned int textLen, unsigned int *sa, unsigned int saLen) {
        this->free();
        fillLUT2ForSamSAMi2(this->lut2, text, sa, saLen);
	unsigned int uniqueSuffixNum = getUniqueSuffixNumForSamSAMi2(this->k, text, textLen, sa, saLen);
	this->bucketsNum = (double)uniqueSuffixNum * (1.0 / this->loadFactor);
        switch(this->type) {
        case HTBase::STANDARD:
                this->fillBasicHTData(text, textLen, sa, saLen);
                break;
        case HTBase::DENSE:
                this->fillDenseHTData(text, textLen, sa, saLen);
                break;     
        }
}

void HTSamSAMi2::getBasicHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
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
			if (leftBoundary == HT::emptyValueHT) {
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

void HTSamSAMi2::getDenseHTBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
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
			if (leftBoundary == HT::emptyValueHT) {
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

void HTSamSAMi2::getBoundaries(unsigned char *pattern, unsigned char *text, unsigned int *sa, unsigned int &leftBoundary, unsigned int &rightBoundary) {
    return (this->*getBoundariesOperation)(pattern, text, sa, leftBoundary, rightBoundary);
}

/*SHARED STUFF*/

void markSketch(unsigned char *text, unsigned int bitShift, unsigned int textIndex, unsigned int *sketches, unsigned int sketchesIndex) {
	unsigned int k = 8;
	if (textIndex < 8) k = textIndex;
	for (unsigned int i = 1; i <= k; ++i) {
		sketches[sketchesIndex] += (((text[textIndex - i] >> bitShift) & 3) << (16 - 2 * i));
	}
	for (unsigned int i = k; i < 8; ++i) {
		sketches[sketchesIndex] += (3 << (16 - 2 * (i + 1)));
	}
}

unsigned int getBitShift(unsigned char *text, unsigned int textLen) {
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

bool isSketchEqual(unsigned int bitShift, unsigned char *pattern, unsigned int pos, unsigned int sketch, unsigned int i) {
	int k = 8;
	if (pos < 8) k = pos;
	if ((i & 1) == 0) sketch = sketch >> 16;
	else sketch = sketch & 0x0000FFFF;
	for (int i = 1; i <= k; ++i) {
		if (((sketch >> (16 - 2 * i)) & 3) != ((pattern[pos - i] >> bitShift) & 3)) return false;
	}
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
			++lut2[i][j][1];
		}
	}
}

void binarySearchForSamSAMi2(unsigned int *sa, unsigned char *text, unsigned int lStart, unsigned int rStart, unsigned char *pattern, int patternLength, unsigned int &beg, unsigned int &end) {
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

}