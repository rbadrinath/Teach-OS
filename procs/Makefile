CC=gcc
CFLAGS= -g
DEPS = 
OBJ = 
EXEC = loopy  shell  simple_exec simple_fork  simple_getpid zomB reparent priority_ops syscallC
OBJS = loopy.o  shell.o  simple_exec.o  simple_fork.o simple_getpid.o \
       zomB.o reparent.o priority_ops.o syscallC.o syscall.o

#OBJS = 

##$(EXEC):

all-execs: $(EXEC) assembly

# all-objs: $(OBJS)

# all: all-objs all-execs
all: all-execs 

assembly:
	gcc -c syscall.s
	ld -o syscall syscall.o

clean:
	rm -f $(OBJS) a.out $(EXEC) syscall 

help:
	echo all-execs/ all-objs /all /clean-all
