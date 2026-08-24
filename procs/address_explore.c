#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>

// Exploring sections

// into .rodata
const int gc=500;

// into .data
char gstring[]="Hello how are you today?";
int gi1=1;

// into .bss
int gi0=0;
int gu;

// into .text
void testaddress(int a, int b, int c){
	int i,j,k;
	printf("function locals: %p %p %p\n",&i,&j,&k);
	printf("function params: %p %p %p\n",&a,&b,&c);
	return;
}

// into TLS .tdata or .tbss depending on the initialization
//thread_local int globalx=5;

void main(int argc, char *argv[]){
	int i,j,k;
	printf("Experiment:\n");
	printf("  For each of these test output against output of readelf -s %s \n",argv[0]);
	printf("main at     main:  %p\n",main);   
	printf("Global Cons    gc: %p\n",&gc);
	printf("Globalv Ini-1 gi1: %p\n",&gi1);
	printf("Globalv Zer-0 gi0: %p\n",&gi0);
	printf("Globalv Un-In gu:  %p\n",&gu);
	printf("small malloc at :  %p\n",malloc(100));
	//getchar();
	printf("Big malloc at:     %p\n",malloc(256*1024));
	printf("main locals: %p %p %p\n",&i,&j,&k);
	// printf("GlobalT :%p\n",&globalx);
	testaddress(i,j,k);
	sleep(300);
}
