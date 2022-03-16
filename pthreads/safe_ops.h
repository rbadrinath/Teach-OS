#define MAX 10

struct mystruct {
	pthread_mutex_t mutex;
	int val[MAX];
	int free;
	int used;
};

#define ISEMPTY(s) (s.free==s.used)
#define ISFULL(s) ((s.free+1)%MAX==s.used)

void put(int val, struct mystruct * s);
int get(struct mystruct * s);
void safely_put(int val, struct mystruct * s, int semid);
int safely_get(struct mystruct * s, int semid);

