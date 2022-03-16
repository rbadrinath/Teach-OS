// Needed for acquiring token via ftok:
#define MYPROJECTPATH "/tmp/blah"
#define MYPROJECTID 6764

// Message structure definition:
#define MSGSIZE 100
#define TYPEA 1
struct msgbuf {
	long mtype;  
	char mtext[MSGSIZE];
};

