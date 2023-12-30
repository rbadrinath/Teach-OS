/*
 * A program to generate some exceptions
 */
#include <stdio.h>
#define DIVBYZERO 1
#define BADMEM 2
#define BADACCESS 3
const int x=0;
void main(){
	int type=3;
	switch( type ) {
		case DIVBYZERO:
			printf("div by zero: %d\n",type/(type-type));
		case BADMEM:
			printf("bad mem: %d\n",*(int *)0x7fffffff);
		case BADACCESS:
			scanf("%d",&x);
		default:
			printf("Unknown, exit\n");
	}
}
