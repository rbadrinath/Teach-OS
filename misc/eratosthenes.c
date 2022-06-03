// computing the primes upto a certain limit
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
/*
           struct timeval {
               time_t      tv_sec;      seconds 
               suseconds_t tv_usec;     microseconds 
           };
*/
struct {
	struct timeval start;
	struct timeval end;
} times;
void set_time_start(){
	struct rusage usage;
	int r = getrusage(RUSAGE_SELF , &usage);
	times.start=usage.ru_utime;
}
void set_time_end(){
	struct rusage usage;
	int r = getrusage(RUSAGE_SELF , &usage);
	times.end=usage.ru_utime;
}
long get_time_usage(){
	return times.end.tv_sec*1000000+times.end.tv_usec
		- times.start.tv_sec*1000000+-times.start.tv_usec;
}


#define METHOD 2
#define LIMITN 100000		// Look for primes under this limit
#define ASIZE  20000		// This will hold all the primes
int main()
{
   
    struct {
        long int rem;
        long int prime;
        } factor[ASIZE];
    int idx=0, n, isprime, j;
    set_time_start();
    for(n=2;n<LIMITN;n++) {	// Interesting: n is never used explicitly!
        isprime=1;
        for( j=0; j<idx;j++){
#if METHOD == 1
	    // First idea is dont use the "%" operator
            factor[j].rem++;
            if (factor[j].rem==factor[j].prime) {
                factor[j].rem=0;
                isprime=0;
            }
#else
	    // Another idea is to use a cryptic single line
	    if ( (factor[j].rem=(factor[j].rem+1)%factor[j].prime) == 0 )
		    isprime=0;
#endif
	  
        }
        if(isprime){
            printf("%3d %d\n",idx+1,n);
            factor[idx].rem=0, factor[idx].prime=n;
            idx++;
        }
    }
    set_time_end();
    long t = get_time_usage();
    printf("time - %ld usec\n",t);
}
