#include <stdio.h>
long gcd(long a, long b);

// we use ax and bx below simply to visually help map to registers
long  gcd(long ax, long cx){

   while( cx!= 0 ) {
	int rem=ax%cx;   // only interested in remiainder
	ax=cx;
	cx=rem;
   }
   return ax;
}
