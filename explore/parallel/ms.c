#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define RD 0
#define WR 1
int funglobal=0x30;
int mergesort_base_solution(int *a, int size){
	// size == 1 or 0
	if(size<=1)
		return 1;
		
	if (size==2) {
		if ( a[0] > a[1] ) {
			int temp=a[0];
			a[0]=a[1];
			a[1]=temp;
		}
		return 1;
	}
	// size is 3 or more
	return 0;
}

void merge(int *a, int midn, int size){
	int a1[midn],a2[size-midn];
	// split a into a1, a2
	for(int i=0;i<midn;i++)
		a1[i]=a[i];
	for(int i=0;i<size-midn;i++)
		a2[i]=a[i+midn];
	// merge a1, a2 back into a
	int i=0, j=0; // i and j start from the beginning of a1 and a2
	int k=0;
	while( (i <midn ) && (j < size-midn) ) {
		if( a1[i] <= a2[j] )
			a[k++]=a1[i++];
		else
			a[k++]=a2[j++];
	}	
	while (i<midn)
		a[k++]=a1[i++];
	while (j<size-midn)
		a[k++]=a2[j++];
}

void mergesort(int *a, int size){ //a[0]..a[size-1] is size elements
	if ( mergesort_base_solution(a,size) )
		return;
	
	int mid = size/2;	// note size is 3 or more

/* (1) parallel part starts ...
	int mypipe[2];
	if ( pipe(mypipe) ) {		// check the return value!!
		perror("Error opening pipe\n");
		exit(0);
	}
	int cpid;
	if ((cpid=fork())==0){
		//have child do this part a[0]..a[mid-1]
		// close read end
		close(mypipe[RD]);
		// sort a[0]..a[mid-1]
*/
		mergesort(a,mid);
/* (2) parallel part starts ...
		// send back the result to the parent
		printf("DEBUG: Child writing to pipe %ld\n",sizeof(int)*mid);
		int k = write(mypipe[WR], a, sizeof(int)*mid);
		printf("DEBUG: Child written to pipe %d\n",k);
		// close write end and exit
		close(mypipe[WR]);
		exit(0);
	}
	// parent activity
	// close the write end
	close(mypipe[WR]);
*/
	// sort a[mid]..a[size-1]
	mergesort(&a[mid],size-mid); // a[mid]...a[size-1] is size-mid elements
/* (3) parallel part starts ...
	// read results from the child for the first part
	int nbytes=0;
	while (1) {

		printf("DEBUG: Parent reding pipe\n");
		int k = read(mypipe[RD],((char *)a) + nbytes,sizeof(int)*mid);
		if(k < 0 ) {
			perror("Error reading pipe\n");
			exit(1);
		}
		printf("DEBUG: Parent read from pipe %d\n",k);
		if (k==0) break;
        	nbytes+=k;
	}
	close(mypipe[RD]);
	waitpid(cpid,NULL,0);
*/
	merge(a,mid,size); // After this the two sorted sides are merged
}

int main(){
	int a[100];
	int n;
	printf("My pid is %d\n",(int ) getpid());
	printf("%d\n Give me the number of items to sort, followed by the list of times: ",funglobal);
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		scanf("%d",&a[i]);
	mergesort(a,n);
	for(int i=0;i<n;i++)
		printf("%d ",a[i]);
	printf("\n");
}
