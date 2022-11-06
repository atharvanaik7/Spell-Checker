#include <iostream>
#include <ctime>
#include <cstddef>
#include "TimeInterval.h"


TimeInterval::TimeInterval() {

}

void TimeInterval::start() {
	time(&start_time);
}

void TimeInterval::stop() {
	time(&end_time);
}

float TimeInterval::getInterval() {
	float t = (float)(end_time - start_time);
	return t;
}