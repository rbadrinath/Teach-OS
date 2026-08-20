#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
// The above is needed for syscall numbers in symbolic form  like SYS_XXXX
// See man 2 syscall

char * message="hello from the C world\n";
#define WR_FD 1

// This is a way to make a syscall using the syscall number from C
void main()
{
      syscall(SYS_write, WR_FD, message , strlen(message));
}

