#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

char * message="hello from the C world\n";

void main()
{
      syscall(SYS_write, 1, message , strlen(message));
}

