#define MAX 10

struct mystruct {
	int val[MAX];
	int free;
	int used;
};

void put(int val, struct mystruct * s);
int get(struct mystruct * s);
void safely_put(int val, struct mystruct * s, int semid);
int safely_get(struct mystruct * s, int semid);

