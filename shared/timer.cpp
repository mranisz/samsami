#include <cstdio>
#include "timer.h"

void CStopWatch::startTimer() {
	gettimeofday(&(timer.start),NULL);
}

void CStopWatch::stopTimer() {
	gettimeofday(&(timer.stop),NULL);
}

double CStopWatch::getElapsedTime() {
	timeval res;
	timersub(&(timer.stop),&(timer.start),&res);
	return res.tv_sec + res.tv_usec/1000000.0; // 10^6 uSec per second
}
