#include <stdio.h>
#include <string.h>

// create a number with the characters  'A'(0x41), 'B', 'C', 'D'
unsigned int n=0x41424344;
char least_significant_char=(char)0x44;

// If Least Significant appears first, it is little endian
// If Most significant appears first, it is big endian
//
int main(int argc, char * argv[]){
	printf("Originally least_significant is %c, here are the extracted characters:\n",least_significant_char);
	printf(" %d ", (int)sizeof(unsigned int));
	char f,c;
	f = c = * (char *) &n ; 	  putchar( c );    putchar(' ');
	c = * ((char *) &n  + 1); putchar( c );    putchar(' ');
	c = * ((char *) &n  + 2); putchar( c );    putchar(' ');
	c = * ((char *) &n  + 3); putchar( c );    putchar('\n');

	if (argc > 1 && strcmp(argv[1],"-s")==0){
		if (f == least_significant_char ) 
		   printf("Least significant appears first, so it is little endian\n");
		else
		   printf("Most significant appears first, so it is big-endian\n");
	}
}
