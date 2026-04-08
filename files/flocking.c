#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/file.h>
// syntax:  int flock(int fd, int operation);
// Valid values for operation are: LOCK_EX , LOCK_SH , LOCK_UN
// return value is -1 if there is an error, 0 otherwise. See man flock(2)
void main(){
	int fd = open("xyz",O_RDONLY);
	int pid = getpid();
	for(int i=0;i<9;i++) {
		flock(fd,LOCK_EX);
		printf("%d is using the lock\n",pid);
		sleep(1);
		printf("%d Unlocking\n",pid);
		flock(fd,LOCK_UN);
		sleep(1);
	}
	printf("------ %d ends\n",pid);
}
