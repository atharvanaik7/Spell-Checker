#ifndef _TIMEINTERVAL_H_
#define _TIMEINTERVAL_H_

struct timeval {
	long tv_sec;
	long tv_usec;
};

class TimeInterval {
public:
	time_t start_time;
	time_t end_time;

	TimeInterval();
	void start();
	void stop();
	float getInterval();
};


#endif
