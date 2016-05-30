#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
#include "../shared/patterns.h"
#include "../shared/timer.h"
#include "../samsami.h"

using namespace std;
using namespace samsami;

ChronoStopWatch timer;

map<string, SamSAMi1::IndexType> SamSAMi1IndexTypesMap = {{"std", SamSAMi1::STANDARD}, {"sketches8x2", SamSAMi1::WITH_SKETCHES_8x2}, {"sketches4x4", SamSAMi1::WITH_SKETCHES_4x4}};
map<string, SamSAMi2::IndexType> SamSAMi2IndexTypesMap = {{"std", SamSAMi2::STANDARD}, {"sketches8x2", SamSAMi2::WITH_SKETCHES_8x2}, {"sketches4x4", SamSAMi2::WITH_SKETCHES_4x4}};
map<string, HT::HTType> SamSAMiHashTypesMap = {{"hash", HT::STANDARD}, {"hash-dense", HT::DENSE}};
map<string, HTSamSAMi2::HTType> SamSAMi2HashTypesMap = {{"hash", HTSamSAMi2::STANDARD}, {"hash-dense", HTSamSAMi2::DENSE}};

void samSAMi1(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);

void getUsage(char **argv) {
	cout << "Select index you want to test (locate):" << endl;
	cout << "SamSAMi1: " << argv[0] << " 1 std|sketches8x2|sketches4x4 q p fileName patternNum patternLen" << endl;
        cout << "SamSAMi2: " << argv[0] << " 2 std|sketches8x2|sketches4x4 q p fileName patternNum patternLen" << endl;
        cout << "SamSAMi1-hash: " << argv[0] << " 1 std|sketches8x2|sketches4x4 q p hash|hash-dense k loadFactor fileName patternNum patternLen" << endl;
        cout << "SamSAMi2-hash: " << argv[0] << " 2 std|sketches8x2|sketches4x4 q p hash|hash-dense k loadFactor fileName patternNum patternLen" << endl;
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
        getUsage(argv);
        exit(1);
}

void samSAMi1(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()));
		samSAMi1->setVerbose(true);
		samSAMi1->build(textFileName);
		samSAMi1->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi1->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "locate SamSAMi1-" << indexType << "-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete samSAMi1;
	delete P;
        exit(0);
}

void samSAMi1Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1 *samSAMi1;
        string indexFileNameString = "SamSAMi1-" + indexType + "-" + hTType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi1 = new SamSAMi1();
		samSAMi1->load(indexFileName);
	} else {
		samSAMi1 = new SamSAMi1(SamSAMi1IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), SamSAMiHashTypesMap[hTType], atoi(k.c_str()), atof(loadFactor.c_str()));
		samSAMi1->setVerbose(true);
		samSAMi1->build(textFileName);
		samSAMi1->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi1->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi1-" + hTType + ".txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi1->getIndexSize() / (double)samSAMi1->getTextSize();
	cout << "locate SamSAMi1-" << hTType << "-" << indexType << "-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete samSAMi1;
	delete P;
        exit(0);
}

void samSAMi2(string indexType, string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2-" + indexType + "-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()));
		samSAMi2->setVerbose(true);
		samSAMi2->build(textFileName);
		samSAMi2->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi2->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "locate SamSAMi2-" << indexType << "-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete samSAMi2;
	delete P;
        exit(0);
}

void samSAMi2Hash(string indexType, string q, string p, string hTType, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2 *samSAMi2;
        string indexFileNameString = "SamSAMi2-" + indexType + "-" + hTType + "-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi2 = new SamSAMi2();
		samSAMi2->load(indexFileName);
	} else {
		samSAMi2 = new SamSAMi2(SamSAMi2IndexTypesMap[indexType], atoi(q.c_str()), atoi(p.c_str()), SamSAMi2HashTypesMap[hTType], atoi(k.c_str()), atof(loadFactor.c_str()));
		samSAMi2->setVerbose(true);
		samSAMi2->build(textFileName);
		samSAMi2->save(indexFileName);
	}

	Patterns *P = new Patterns(textFileName, queriesNum, m);
	unsigned char **patterns = P->getPatterns();
	vector<unsigned int> *indexLocates = new vector<unsigned int>[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		samSAMi2->locate(patterns[i], m, indexLocates[i]);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_locate_SamSAMi2-" + hTType + ".txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi2->getIndexSize() / (double)samSAMi2->getTextSize();
	cout << "locate SamSAMi2-" << hTType << "-" << indexType << "-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " " << indexType << " " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorLocatesNumber(indexLocates);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete samSAMi2;
	delete P;
        exit(0);
}