#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "../shared/patterns.h"
#include "../shared/timer.h"
#include "../samsami.hpp"

using namespace std;
using namespace shared;
using namespace samsami;

ChronoStopWatch timer;

void samSAMi1(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Sketches8x2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Sketches8x2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDenseSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDenseSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);

void getUsage(char **argv) {
	cout << "Select index you want to test (locate):" << endl;
	cout << "SamSAMi1: " << argv[0] << " 1 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-sketches4x4: " << argv[0] << " 1-4x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-sketches8x2: " << argv[0] << " 1-8x2 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2: " << argv[0] << " 2 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-sketches4x4: " << argv[0] << " 2-4x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-sketches8x2: " << argv[0] << " 2-8x2 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash: " << argv[0] << " 1-hash q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-sketches4x4: " << argv[0] << " 1-hash-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-sketches8x2: " << argv[0] << " 1-hash-8x2 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash: " << argv[0] << " 2-hash q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-sketches4x4: " << argv[0] << " 2-hash-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-sketches8x2: " << argv[0] << " 2-hash-8x2 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense: " << argv[0] << " 1-hash-dense q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense-sketches4x4: " << argv[0] << " 1-hash-dense-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense-sketches8x2: " << argv[0] << " 1-hash-dense-8x2 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense: " << argv[0] << " 2-hash-dense q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense-sketches4x4: " << argv[0] << " 2-hash-dense-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense-sketches8x2: " << argv[0] << " 2-hash-dense-8x2 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "where:" << endl;
	cout << "q - window length" << endl;
	cout << "p - minimizer length, p <= q" << endl;
	cout << "fileName - name of text file" << endl;
	cout << "patternNum - number of patterns (queries)" << endl;
	cout << "patternLen - pattern length" << endl;
	cout << "k - suffix length to be hashed (k > 0)" << endl;
	cout << "loadFactor - load factor of hash table (range: (0.0, 1.0))" << endl << endl;
}

int main(int argc, char *argv[]) {
	if (argc < 7) {
		getUsage(argv);
		exit(1);
	}
    if (string(argv[1]) == "1") samSAMi1(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-4x4") samSAMi1Sketches4x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-8x2") samSAMi1Sketches8x2(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2") samSAMi2(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2-4x4") samSAMi2Sketches4x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2-8x2") samSAMi2Sketches8x2(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-hash") samSAMi1Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-4x4") samSAMi1HashSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-8x2") samSAMi1HashSketches8x2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash") samSAMi2Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-4x4") samSAMi2HashSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-8x2") samSAMi2HashSketches8x2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense") samSAMi1HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense-4x4") samSAMi1HashDenseSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense-8x2") samSAMi1HashDenseSketches8x2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense") samSAMi2HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense-4x4") samSAMi2HashDenseSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense-8x2") samSAMi2HashDenseSketches8x2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	getUsage(argv);
	exit(1);
}

void samSAMi1(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1<SAMSAMI_STANDARD> *samSAMi = new SamSAMi1<SAMSAMI_STANDARD>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi1-std-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-std-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1<SAMSAMI_SKETCHES_4x4> *samSAMi = new SamSAMi1<SAMSAMI_SKETCHES_4x4>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi1-4x4-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-4x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1Sketches8x2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1<SAMSAMI_SKETCHES_8x2> *samSAMi = new SamSAMi1<SAMSAMI_SKETCHES_8x2>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi1-8x2-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-8x2-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2<SAMSAMI_STANDARD> *samSAMi = new SamSAMi2<SAMSAMI_STANDARD>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi2-std-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-std-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2<SAMSAMI_SKETCHES_4x4> *samSAMi = new SamSAMi2<SAMSAMI_SKETCHES_4x4>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi2-4x4-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-4x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2Sketches8x2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2<SAMSAMI_SKETCHES_8x2> *samSAMi = new SamSAMi2<SAMSAMI_SKETCHES_8x2>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi2-8x2-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-8x2-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_STANDARD, HT_STANDARD> *samSAMi = new SamSAMi1Hash<SAMSAMI_STANDARD, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-std-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_4x4, HT_STANDARD> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_4x4, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-4x4-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_8x2, HT_STANDARD> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_8x2, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-8x2-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-8x2-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_STANDARD, HT_STANDARD> *samSAMi = new SamSAMi2Hash<SAMSAMI_STANDARD, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-std-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_4x4, HT_STANDARD> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_4x4, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-4x4-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_8x2, HT_STANDARD> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_8x2, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-8x2-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-8x2-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_STANDARD, HT_DENSE> *samSAMi = new SamSAMi1Hash<SAMSAMI_STANDARD, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-std-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-dense-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_4x4, HT_DENSE> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_4x4, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-4x4-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-dense-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashDenseSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_8x2, HT_DENSE> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_8x2, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-8x2-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi1-hash-dense-8x2-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_STANDARD, HT_DENSE> *samSAMi = new SamSAMi2Hash<SAMSAMI_STANDARD, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-std-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-dense-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_4x4, HT_DENSE> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_4x4, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-4x4-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-dense-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashDenseSketches8x2(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_8x2, HT_DENSE> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_8x2, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-8x2-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	//NegativePatterns *P = new NegativePatterns(textFileName, queriesNum, m);
	/*MaliciousPatterns *P = new MaliciousPatterns(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "locate SamSAMi2-hash-dense-8x2-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 8x2 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexLocates;
	delete samSAMi;
	delete P;
    exit(0);
}