#include <stdio.h>
#include "hello_world.h"

int main(){
#ifndef NAME
#define NAME "Ram"
#endif

	printf("Hello dear %s\n",NAME);
}
