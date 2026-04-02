/*
 * Simplistic implementation of a distributed Bellman-Ford Algorithm
 * The worded "Distributed" is important. Otherwise we can implement it in the standard text book
 * method of initializing with respect to a specific source and repeated relaxation on all edes of the graph.
 * 
 * Here we are repeating relaxation at a node with respect to its neigbours only
 * However, we are repeatedly seeking updated informaiton from the nodes - "diffusion" idea - of their estimates
 *    of distance to each other node.
 *
 * In the output each row represents a nodes estimate of distances and the outnode D(N) to reach that destination
 * So the set of nodes to traverse to reach from node i to node j will be in the j-th column 
 * starting at the i-th row and following the outnodes(rows) mentioned in the (N) form in that column
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define SLEEPTIME 10000
#define INF 10000

// Declare all glabal shared stuff
pthread_mutex_t d_mutex;

// shared d w and next
#define MAXN 10
int d[MAXN][MAXN];  // initially INF except d[i][i]=0;
int next[MAXN][MAXN]; // initially -1 expet next[i][i]=i;
int w[MAXN][MAXN]; // initially read in
int size; // initially read in

// this is an additional one to simulate limited communication with nbrs
int changed[MAXN][MAXN]; // changed[i][j]=1 iff i wants to tell j that something changed.. must use a lock!

void init_matrices(){
	scanf("%d",&size);
	if(size >MAXN ) {
		printf("Matrix too large, please recongifure MAXN... exiting**\n");
		exit(1);
	}
	for(int i=0;i<size;i++)
		for( int j=0;j<size;j++) {
			if( scanf("%d",&w[i][j]) < 0 ) { // assume w[i][j]=0 if i==j and INF if not adjacent
				printf("Error reading w, exiting**\n");
				exit(1);
			}
			// assume 
			next[i][j]= j;
			d[i][j]= w[i][j];
			if( w[i][j]!= INF ) 
				changed[i][j]=1; // initially everyone wants to give nbrs initial info 
			else
				changed[i][j]=0;
		}
}

void print_matrices(){
	printf("dist(nxt):");
        for(int dest=0;dest <size; dest++)
		printf(" %3d   ",dest);
	printf("\n");	
	for(int i=0;i<size;i++) {
		printf("      S:%d",i);
		for( int j=0;j<size;j++) {
			printf(" %3d(%d)",d[i][j],next[i][j]);
		}
		printf("\n");
	}
}

struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=random()%SLEEPTIME;
	return ts;
}

void * thread_node(void * n){
	int t = (int) n;
	int count=0;
	printf("%d Started\n",t);
	while(count < 20){
		// lock at the beginning of the attempt
		int somechange=0;
		pthread_mutex_lock(&d_mutex);
		for(int nbr=0; nbr <size; nbr++) {
			// skip if nbr is yourself
			if (nbr==t)
				continue;
			// !! Important: Skip if this is a non-nbr of t - simulating only nbrs can talk
			if( w[t][nbr]==INF) 
				continue;
			if( changed[nbr][t]== 0 ) {  // if nbr tells me nothing has changed
			        continue; 	
			}
			for( int dest=0; dest <size; dest++) {
			    if(d[t][dest] > w[t][nbr]+d[nbr][dest]){
				    d[t][dest]= w[t][nbr]+d[nbr][dest];
				    next[t][dest]=nbr;
				    somechange=1;
			    }
			}
			// clearup to indicate to yourself that the change has been diffused in
			changed[nbr][t]=0;
		}// end of all nbr check
		if( somechange ) { // something changed at t so tell all my neighbours only
			for(int node=0;node<size;node++)
				if (node!=t && w[node][t]!=INF)
					changed[t][node]=1;
		} else 
			printf("%d No change at %d\n",t,count);
		// unlock at the end of the attempt
		pthread_mutex_unlock(&d_mutex);

		// sleep a little while
		struct timespec ts=random_time();
		nanosleep(&ts,NULL);
		count++;
	}
	printf("%d Done\n",t);
	return NULL ;
}

int main(int argc, char * argv[]){
	// initialize randomnumber generator
	srand(time(NULL));
	// initialize the mutex and the cvs
	pthread_mutex_init(&d_mutex,NULL);
	
	init_matrices();

	// Create the node threads
	pthread_t node_tid[MAXN];
	int r;

	for(int i=0;i < size; i++) {
		printf("Creating thread %d\n",i);
		r = pthread_create(&node_tid[i],NULL,thread_node,(void *)i);
		if ( r != 0 ) {
			perror("Thread Creation failed, exiting***\n");
			exit(1);
		}
	}

	for(int i=0;i < size;i++)
		r = pthread_join(node_tid[i],NULL);

	print_matrices();
}
