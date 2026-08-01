#include <stdio.h>
long  gcd(long ax, long cx){

   while( cx!= 0 ) {
	int rem=ax%cx;
	ax=cx;
	cx=rem;
   }
   return ax;
}
