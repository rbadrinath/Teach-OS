#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/file.h>
// syntax:  int flock(int fd, int operation);
// Valid values for operation are: LOCK_EX , LOCK_SH , LOCK_UN
// return value is -1 if there is an error, 0 otherwise. See man flock(2)
void main(){
	int fd = open("xyz",O_RDONLY);
	flock(fd,LOCK_EX);
	int pid = getpid();
	for(int i=0;i<9;i++) {
		printf("%d is using the lock\n",pid);
		for(int j=0;j<INT_MAX;j++)
			;
	}
	printf("------ %d ends\n",pid);
}
