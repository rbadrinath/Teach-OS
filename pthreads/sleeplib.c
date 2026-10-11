// This is just some code - to remind us of a quick way to sleep
// for a small amount of time

// SLEEPTIME is a constant used to bound the time
#define SLEEPTIME 10000

struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=random()%SLEEPTIME;
	return ts;
}

// Usage:
/*
	struct timespec ts=random_time();
	nanosleep(&ts,NULL);
*/
