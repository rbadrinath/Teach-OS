#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "configs.h"
#include "safe_ops.h"
#include "mysemops.h"

void put(int val, struct mystruct * s){
	s->val[s->free]=val;
	s->free=(s->free+1)%MAX;
}
int get(struct mystruct * s){
	int x=s->val[s->used];
	s->used=(s->used+1)%MAX;
	return x;
}

void safely_put(int val, struct mystruct * s, int semid){
	lock_down(semid);
	put(val, s);
	free_up(semid);
}

int safely_get(struct mystruct * s, int semid){
	lock_down(semid);
	int x = get(s);
	free_up(semid);
	return x;
}

