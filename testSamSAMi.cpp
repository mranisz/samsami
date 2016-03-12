#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
#include "shared/common.h"
#include "shared/patterns.h"
#include "shared/timer.h"
#include "samsami.h"

using namespace std;
using namespace samsami;

ChronoStopWatch timer;

map<string, SamSAMi1::IndexType> SamSAMi1IndexTypesMap = {{"std", SamSAMi1::STANDARD}, {"sketches8x2", SamSAMi1::WITH_SKETCHES_8x2}, {"sketches4x4", SamSAMi1::WITH_SKETCHES_4x4}};
map<string, SamSAMi2::IndexType> SamSAMi2IndexTypesMap = {{"std", SamSAMi2::STANDARD}, {"sketches8x2", SamSAMi2::WITH_SKETCHES_8x2}, {"sketches4x4", SamSAMi2::WITH_SKETCHES_4x4}};
map<string, HT::HTType> SamSAMiHashTypesMap = {{"hash", HT::STANDARD}, {"hash-dense", HT::DENSE}};
map<string, SamSAMiFM::IndexType> SamSAMiFMIndexTypesMap = {{"512", SamSAMiFM::TYPE_512}, {"1024", SamSAMiFM::TYPE_1024}};
map<string, HTSamSAMi2::HTType> SamSAMi2HashTypesMap = {{"hash", HTSamSAMi2::STANDARD}, {"hash-dense", HTSamSAMi2::DENSE}};

void samSAMi1(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFM(string indexType, string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHash(string indexType, string q, string p, string l, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);

void getUsage(char **argv) {
	cout << "Select index you want to test:" << endl;
	cout << "SamSAMi1: " << argv[0] << " 1 std|sketches8x2|sketches4x4 q p fileName patternNum patternLen" << endl;
        cout << "SamSAMi2: " << argv[0] << " 2 std|sketches8x2|sketches4x4 q p fileName patternNum patternLen" << endl;
        cout << "SamSAMiFM: " << argv[0] << " FM 512|2014 q p l fileName patternNum patternLen" << endl;
        cout << "SamSAMi1-hash: " << argv[0] << " 1 std|sketches8x2|sketches4x4 q p hash|hash-dense k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMi2-hash: " << argv[0] << " 2 std|sketches8x2|sketches4x4 q p hash|hash-dense k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMiFM-hash: " << argv[0] << " FM 512|1024 q p l hash|hash-dense k loadFactor fileName patternNum patternLen" << endl;
        cout << "where:" << endl;
        cout << "q - window length" << endl;
	cout << "p - minimizer length, p <= q" << endl;
        cout << "l - sampling parameter (mapping between SamSAMi and SA indexes)" << endl;
	cout << "fileName - name of text file" << endl;
	cout << "patternNum - number of patterns (queries)" << endl;
	cout << "patternLen - pattern length" << endl;
	cout << "k - suffix length to be hashed (k > 0)" << endl;
	cout << "loadFactor - load factor of hash table (range: (0.0, 1.0))" << endl << endl;
}

int main(int argc, char *argv[]) {
	if (argc < 8) {
		getUsage(argv);
		exit(1);
	}
        if ((string)argv[1] == "1") {
		if (SamSAMi1IndexTypesMap.find(string(argv[2])) != SamSAMi1IndexTypesMap.end()) {
                        if (argc == 8) samSAMi1(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
                        else if (argc == 11 && SamSAMiHashTypesMap.find(string(argv[5])) != SamSAMiHashTypesMap.end()) samSAMi1Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), string(argv[7]), argv[8], atoi(argv[9]), atoi(argv[10]));
		}
	}
        else if ((string)argv[1] == "2") {
		if (SamSAMi2IndexTypesMap.find(string(argv[2])) != SamSAMi2IndexTypesMap.end()) {
                        if (argc == 8) samSAMi2(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
                        else if (argc == 11 && SamSAMi2HashTypesMap.find(string(argv[5])) != SamSAMi2HashTypesMap.end()) samSAMi2Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), string(argv[7]), argv[8], atoi(argv[9]), atoi(argv[10]));
		}
	}
        else if ((string)argv[1] == "FM") {
		if (SamSAMiFMIndexTypesMap.find(string(argv[2])) != SamSAMiFMIndexTypesMap.end()) {
                        if (argc == 9) samSAMiFM(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
                        else if (argc == 12 && SamSAMiHashTypesMap.find(string(argv[6])) != SamSAMiHashTypesMap.end()) samSAMiFMHash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), string(argv[7]), string(argv[8]), argv[9], atoi(argv[10]), atoi(argv[11]));
		}
	}
        getUsage(argv);
        exit(1);
}

void samSAMi1(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()));
		samSAMi1->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMi1->build(text, textLen);
		samSAMi1->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi1->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "count SamSAMi1-" << indexType << "-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMi1;
	delete P;
        exit(0);
}

void samSAMi1Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1-" + indexType + "-" + hTType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), SamSAMiHashTypesMap[hTType], atoi(k.c_str()), atof(loadFactor.c_str()));
		samSAMi1->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMi1->build(text, textLen);
		samSAMi1->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi1->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-" + hTType + ".txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "count SamSAMi1-" << hTType << "-" << indexType << "-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMi1;
	delete P;
        exit(0);
}

void samSAMi2(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()));
		samSAMi2->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMi2->build(text, textLen);
		samSAMi2->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi2->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "count SamSAMi2-" << indexType << "-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMi2;
	delete P;
        exit(0);
}

void samSAMi2Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2-" + indexType + "-" + hTType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), HT::STANDARD, atoi(k.c_str()), atof(loadFactor.c_str()));
		samSAMi2->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMi2->build(text, textLen);
		samSAMi2->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi2->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-" + hTType + ".txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "count SamSAMi2-" << hTType << "-" << indexType << "-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMi2;
	delete P;
        exit(0);
}

void samSAMiFM(string indexType, string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMiFM *samSAMiFM;
        string indexFileNameString = "SamSAMiFM-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMiFM = new SamSAMiFM();
		samSAMiFM->load(indexFileName);
	} else {
		samSAMiFM = new SamSAMiFM(SamSAMiFMIndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
		samSAMiFM->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMiFM->build(text, textLen);
		samSAMiFM->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMiFM->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMiFM->getIndexSize() / (double)samSAMiFM->getTextSize();
	cout << "count SamSAMiFM-" << indexType << "-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMiFM;
	delete P;
        exit(0);
}

void samSAMiFMHash(string indexType, string q, string p, string l, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMiFM *samSAMiFM;
        string indexFileNameString = "SamSAMiFM-" + indexType + "-" + hTType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMiFM = new SamSAMiFM();
		samSAMiFM->load(indexFileName);
	} else {
		samSAMiFM = new SamSAMiFM(SamSAMiFMIndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), SamSAMiHashTypesMap[hTType], atoi(k.c_str()), atof(loadFactor.c_str()));
		samSAMiFM->setVerbose(true);
		text = readText(textFileName, textLen, 0);
		samSAMiFM->build(text, textLen);
		samSAMiFM->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMiFM->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-" + hTType + ".txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMiFM->getIndexSize() / (double)samSAMiFM->getTextSize();
	cout << "count SamSAMiFM-" << hTType << "-" << indexType << "-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	if (text != NULL) delete[] text;
	delete[] indexCounts;
	delete samSAMiFM;
	delete P;
        exit(0);
}