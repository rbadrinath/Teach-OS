#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
 char * x ;
 printf("Here ---------------------> \n");
 printf("Iam\n");
 x = malloc(32767);
 x[10]='k';
}
int fun(){
	printf("Bye\n");
}
