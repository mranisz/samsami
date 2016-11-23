#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "../shared/patterns.hpp"
#include "../shared/timer.hpp"
#include "../samsami.hpp"

using namespace std;
using namespace shared;
using namespace samsami;

ChronoStopWatch timer;

void samSAMi1(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Sketches2x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Sketches4x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Sketches2x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2Hash(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi1HashDenseSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDense(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDenseSketches4x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMi2HashDenseSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTBasic(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTBch(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTCf(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTMpe1(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTMpe2(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTMpe3(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashBasic(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashBch(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashCf(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashMpe1(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashMpe2(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashMpe3(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseBasic(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseBch(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseCf(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseMpe1(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseMpe2(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);
void samSAMiFMHWTHashDenseMpe3(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m);

void getUsage(char **argv) {
	cout << "Select index you want to test (count):" << endl;
	cout << "SamSAMi1: " << argv[0] << " 1 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-sketches4x4: " << argv[0] << " 1-4x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-sketches2x4: " << argv[0] << " 1-2x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2: " << argv[0] << " 2 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-sketches4x4: " << argv[0] << " 2-4x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-sketches2x4: " << argv[0] << " 2-2x4 q p fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash: " << argv[0] << " 1-hash q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-sketches4x4: " << argv[0] << " 1-hash-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-sketches2x4: " << argv[0] << " 1-hash-2x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash: " << argv[0] << " 2-hash q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-sketches4x4: " << argv[0] << " 2-hash-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-sketches2x4: " << argv[0] << " 2-hash-2x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense: " << argv[0] << " 1-hash-dense q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense-sketches4x4: " << argv[0] << " 1-hash-dense-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi1-hash-dense-sketches2x4: " << argv[0] << " 1-hash-dense-2x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense: " << argv[0] << " 2-hash-dense q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense-sketches4x4: " << argv[0] << " 2-hash-dense-4x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMi2-hash-dense-sketches2x4: " << argv[0] << " 2-hash-dense-2x4 q p k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMiFMHWT: " << argv[0] << " FMHWT-basic|FMHWT-bch|FMHWT-cf|FMHWT-mpe1|FMHWT-mpe2|FMHWT-mpe3 q p l fileName patternNum patternLen" << endl;
	cout << "SamSAMiFMHWT-hash: " << argv[0] << " FMHWT-hash-basic|FMHWT-hash-bch|FMHWT-hash-cf|FMHWT-hash-mpe1|FMHWT-hash-mpe2|FMHWT-hash-mpe3 q p l k loadFactor fileName patternNum patternLen" << endl;
	cout << "SamSAMiFMHWT-hash-dense: " << argv[0] << " FMHWT-hash-dense-basic|FMHWT-hash-dense-bch|FMHWT-hash-dense-cf|FMHWT-hash-dense-mpe1|FMHWT-hash-dense-mpe2|FMHWT-hash-dense-mpe3 q p l k loadFactor fileName patternNum patternLen" << endl;
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
	if (argc < 7) {
		getUsage(argv);
		exit(1);
	}
    if (string(argv[1]) == "1") samSAMi1(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-4x4") samSAMi1Sketches4x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-2x4") samSAMi1Sketches2x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2") samSAMi2(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2-4x4") samSAMi2Sketches4x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "2-2x4") samSAMi2Sketches2x4(string(argv[2]), string(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
	if (string(argv[1]) == "1-hash") samSAMi1Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-4x4") samSAMi1HashSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-2x4") samSAMi1HashSketches2x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash") samSAMi2Hash(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-4x4") samSAMi2HashSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-2x4") samSAMi2HashSketches2x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense") samSAMi1HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense-4x4") samSAMi1HashDenseSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "1-hash-dense-2x4") samSAMi1HashDenseSketches2x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense") samSAMi2HashDense(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense-4x4") samSAMi2HashDenseSketches4x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "2-hash-dense-2x4") samSAMi2HashDenseSketches2x4(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), argv[6], atoi(argv[7]), atoi(argv[8]));
	if (string(argv[1]) == "FMHWT-basic") samSAMiFMHWTBasic(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-bch") samSAMiFMHWTBch(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-cf") samSAMiFMHWTCf(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-mpe1") samSAMiFMHWTMpe1(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-mpe2") samSAMiFMHWTMpe2(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-mpe3") samSAMiFMHWTMpe3(string(argv[2]), string(argv[3]), string(argv[4]), argv[5], atoi(argv[6]), atoi(argv[7]));
	if (string(argv[1]) == "FMHWT-hash-basic") samSAMiFMHWTHashBasic(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-bch") samSAMiFMHWTHashBch(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-cf") samSAMiFMHWTHashCf(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-mpe1") samSAMiFMHWTHashMpe1(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-mpe2") samSAMiFMHWTHashMpe2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-mpe3") samSAMiFMHWTHashMpe3(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-basic") samSAMiFMHWTHashDenseBasic(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-bch") samSAMiFMHWTHashDenseBch(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-cf") samSAMiFMHWTHashDenseCf(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-mpe1") samSAMiFMHWTHashDenseMpe1(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-mpe2") samSAMiFMHWTHashDenseMpe2(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
	if (string(argv[1]) == "FMHWT-hash-dense-mpe3") samSAMiFMHWTHashDenseMpe3(string(argv[2]), string(argv[3]), string(argv[4]), string(argv[5]), string(argv[6]), argv[7], atoi(argv[8]), atoi(argv[9]));
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-std-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-4x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1Sketches2x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1<SAMSAMI_SKETCHES_2x4> *samSAMi = new SamSAMi1<SAMSAMI_SKETCHES_2x4>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi1-2x4-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-2x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-std-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-4x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2Sketches2x4(string q, string p, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2<SAMSAMI_SKETCHES_2x4> *samSAMi = new SamSAMi2<SAMSAMI_SKETCHES_2x4>(atoi(q.c_str()), atoi(p.c_str()));
    string indexFileNameString = "SamSAMi2-2x4-" + (string)textFileName + "-" +  q + "-" + p + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-2x4-" << q << "-" << p << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_2x4, HT_STANDARD> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_2x4, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-2x4-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-2x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_2x4, HT_STANDARD> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_2x4, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-2x4-hash-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-2x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-dense-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-dense-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi1HashDenseSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi1Hash<SAMSAMI_SKETCHES_2x4, HT_DENSE> *samSAMi = new SamSAMi1Hash<SAMSAMI_SKETCHES_2x4, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi1-2x4-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi1-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi1-hash-dense-2x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-dense-std-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " std " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
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

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-dense-4x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 4x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMi2HashDenseSketches2x4(string q, string p, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMi2Hash<SAMSAMI_SKETCHES_2x4, HT_DENSE> *samSAMi = new SamSAMi2Hash<SAMSAMI_SKETCHES_2x4, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMi2-2x4-hash-dense-" + (string)textFileName + "-" +  q + "-" + p + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMi2-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMi2-hash-dense-2x4-" << q << "-" << p << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " 2x4 " << q << " " << p << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTBasic(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankBasic32<RANK_BASIC_STANDARD>> *samSAMi = new SamSAMiFMHWT<RankBasic32<RANK_BASIC_STANDARD>>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-basic-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-basic-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " basic " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTBch(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>> *samSAMi = new SamSAMiFMHWT<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-bch-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-bch-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " bch " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTCf(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankCF32> *samSAMi = new SamSAMiFMHWT<RankCF32>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-cf-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-cf-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " cf " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTMpe1(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankMPE32<RANK_MPE1>> *samSAMi = new SamSAMiFMHWT<RankMPE32<RANK_MPE1>>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-mpe1-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-mpe1-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe1 " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTMpe2(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankMPE32<RANK_MPE2>> *samSAMi = new SamSAMiFMHWT<RankMPE32<RANK_MPE2>>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-mpe2-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-mpe2-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe2 " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTMpe3(string q, string p, string l, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWT<RankMPE32<RANK_MPE3>> *samSAMi = new SamSAMiFMHWT<RankMPE32<RANK_MPE3>>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-mpe3-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-mpe3-" << q << "-" << p << "-" << l << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe3 " << q << " " << p << " " << l << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashBasic(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_STANDARD>, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_STANDARD>, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-basic-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-basic-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " basic " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashBch(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-bch-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-bch-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " bch " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashCf(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankCF32, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankCF32, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-cf-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-cf-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " cf " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashMpe1(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE1>, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE1>, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-mpe1-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-mpe1-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe1 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashMpe2(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE2>, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE2>, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-mpe2-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-mpe2-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe2 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashMpe3(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE3>, HT_STANDARD> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE3>, HT_STANDARD>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-mpe3-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFMHWT-hash.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFM-hash-mpe3-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe3 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseBasic(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_STANDARD>, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_STANDARD>, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-basic-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-basic-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " basic " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseBch(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankBasic32<RANK_BASIC_COMPRESSED_HEADERS>, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-bch-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-bch-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " bch " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseCf(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankCF32, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankCF32, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-cf-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-cf-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " cf " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseMpe1(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE1>, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE1>, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-mpe1-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-mpe1-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe1 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseMpe2(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE2>, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE2>, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-mpe2-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-mpe2-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe2 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}

void samSAMiFMHWTHashDenseMpe3(string q, string p, string l, string k, string loadFactor, const char *textFileName, unsigned int queriesNum, unsigned int m) {
	SamSAMiFMHWTHash<RankMPE32<RANK_MPE3>, HT_DENSE> *samSAMi = new SamSAMiFMHWTHash<RankMPE32<RANK_MPE3>, HT_DENSE>(atoi(q.c_str()), atoi(p.c_str()), atoi(l.c_str()), atoi(k.c_str()), atof(loadFactor.c_str()));
    string indexFileNameString = "SamSAMiFMHWT-hash-dense-mpe3-" + (string)textFileName + "-" +  q + "-" + p + "-" + l + "-" +  k + "-" + loadFactor + ".idx";
	const char *indexFileName = indexFileNameString.c_str();

	if (fileExists(indexFileName)) {
		samSAMi->load(indexFileName);
	} else {
		samSAMi->build(textFileName);
		samSAMi->save(indexFileName);
	}

	Patterns32 *P = new Patterns32(textFileName, queriesNum, m);
	//NegativePatterns32 *P = new NegativePatterns32(textFileName, queriesNum, m);
	/*MaliciousPatterns32 *P = new MaliciousPatterns32(textFileName, m);
	queriesNum = P->getQueriesNum();
	if (queriesNum == 0) exit(1);*/
	unsigned char **patterns = P->getPatterns();
	unsigned int *indexCounts = new unsigned int[queriesNum];

	timer.startTimer();
	for (unsigned int i = 0; i < queriesNum; ++i) {
		indexCounts[i] = samSAMi->count(patterns[i], m);
	}
	timer.stopTimer();

	string resultFileName = "results/samsami/" + string(textFileName) + "_count_SamSAMiFM-hash-dense.txt";
	fstream resultFile(resultFileName.c_str(), ios::out | ios::binary | ios::app);
	double size = (double)samSAMi->getIndexSize() / (double)samSAMi->getTextSize();
	cout << "count SamSAMiFMHWT-hash-dense-mpe3-" << q << "-" << p << "-" << l << "-" << k << "-" << loadFactor << " " << textFileName << " m=" << m << " queries=" << queriesNum << " size=" << size << "n time=" << timer.getElapsedTime() << endl;
	resultFile << m << " " << queriesNum << " mpe3 " << q << " " << p << " " << l << " " << k << " " << loadFactor << " " << size << " " << timer.getElapsedTime();

	unsigned int differences = P->getErrorCountsNumber(indexCounts);
	if (differences > 0) {
		cout << "DIFFERENCES: " << differences << endl;
		resultFile << " DIFFERENCES: " << differences;
	} else {
		cout << "Differences: " << differences << endl;
	}
	resultFile << endl;
	resultFile.close();

	delete[] indexCounts;
	delete samSAMi;
	delete P;
    exit(0);
}