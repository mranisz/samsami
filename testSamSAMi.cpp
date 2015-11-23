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

map<string, SamSAMi1::IndexType> SamSAMi1IndexTypesMap = {{"std", SamSAMi1::TYPE_STANDARD}, {"sketches", SamSAMi1::TYPE_SKETCHES}};
map<string, SamSAMi2::IndexType> SamSAMi2IndexTypesMap = {{"std", SamSAMi2::TYPE_STANDARD}, {"sketches", SamSAMi2::TYPE_SKETCHES}};

void samSAMi1(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Hash(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Hash(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDense(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDense(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);

void getUsage(char **argv) {
	cout << "Select index you want to test:" << endl;
	cout << "SamSAMi1: ./" << argv[0] << " 1 std|sketches q p fileName patternNum patternLen" << endl;
        cout << "SamSAMi2: ./" << argv[0] << " 2 std|sketches q p fileName patternNum patternLen" << endl;
        cout << "SamSAMi1-hash: ./" << argv[0] << " 1-hash std|sketches q p k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMi2-hash: ./" << argv[0] << " 2-hash std|sketches q p k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMi1-dense-hash: ./" << argv[0] << " 1-hash-dense std|sketches q p k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMi2-dense-hash: ./" << argv[0] << " 2-hash-dense std|sketches q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "where:" << endl;
        cout << "q - window length " << endl;
	cout << "p - minimizer length, p <= q" << endl;
	cout << "fileName - name of text file" << endl;
	cout << "patternNum - number of patterns (queries)" << endl;
	cout << "patternLen - pattern length" << endl;
	cout << "k - suffix length to be hashed (k > 0)" << endl;
	cout << "loadFactor - load factor of hash table (range: (0.0, 1.0))" << endl << endl;
}

int main(int argc, char *argv[]) {
	if (argc < 8) {
		getUsage(argv);
		return 1;
	}
        if ((string)argv[1] == "1") {
		if (SamSAMi1IndexTypesMap.find(string(argv[2])) == SamSAMi1IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi1(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	}
	else if ((string)argv[1] == "1-hash") {
		if (argc < 10 || SamSAMi1IndexTypesMap.find(string(argv[2])) == SamSAMi1IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi1Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	}
        else if ((string)argv[1] == "1-hash-dense") {
		if (argc < 10 || SamSAMi1IndexTypesMap.find(string(argv[2])) == SamSAMi1IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi1HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	}
        else if ((string)argv[1] == "2") {
		if (SamSAMi2IndexTypesMap.find(string(argv[2])) == SamSAMi2IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi2(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	}
	else if ((string)argv[1] == "2-hash") {
		if (argc < 10 || SamSAMi2IndexTypesMap.find(string(argv[2])) == SamSAMi2IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi2Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	}
        else if ((string)argv[1] == "2-hash-dense") {
		if (argc < 10 || SamSAMi2IndexTypesMap.find(string(argv[2])) == SamSAMi2IndexTypesMap.end()) {
			getUsage(argv);
			exit(1);
		}
		samSAMi2HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	}
        else {
		getUsage(argv);
		exit(1);
	}
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
	cout << "count SamSAMi1_" << indexType << "_" << q << "_" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
}

void samSAMi1Hash(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1hash-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), HT::TYPE_BASIC, atoi(k.c_str()), atof(loadFactor.c_str()));
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

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "count SamSAMi1hash_" << indexType << "_" << q << "_" << p << "_" << k << "_" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
}

void samSAMi1HashDense(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1hash-dense-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), HT::TYPE_DENSE, atoi(k.c_str()), atof(loadFactor.c_str()));
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

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "count SamSAMi1hash-dense_" << indexType << "_" << q << "_" << p << "_" << k << "_" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
	cout << "count SamSAMi2_" << indexType << "_" << q << "_" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
}

void samSAMi2Hash(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2hash-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), HT::TYPE_BASIC, atoi(k.c_str()), atof(loadFactor.c_str()));
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

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "count SamSAMi2hash_" << indexType << "_" << q << "_" << p << "_" << k << "_" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
}

void samSAMi2HashDense(string indexType, string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
        unsigned char* text = NULL;
	unsigned int textLen;
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2hash-dense-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), HT::TYPE_DENSE, atoi(k.c_str()), atof(loadFactor.c_str()));
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

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "count SamSAMi2hash-dense_" << indexType << "_" << q << "_" << p << "_" << k << "_" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
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
}