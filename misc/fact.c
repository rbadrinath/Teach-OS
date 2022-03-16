#include <stdio.h>
int fact(int n){
	int x;
	if (n <= 1 )
		return 1;
	else
		return n*fact(n-1);
}
int test(int y){
	int x=1;
	int n = x + y; 
	return n;
}
int main(){
	int v=5;
	int f=test(v);
	int z=3;
}
