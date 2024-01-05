CC=gcc
CFLAGS= -g
DEPS = 
OBJ = 
EXEC = loopy  shell  simple_exec simple_fork  simple_getpid \
       fork_and_termC fork_and_termP zomB reparent priority_ops \
       syscallC exceptions fork_experiment
OBJS = loopy.o  shell.o  simple_exec.o  simple_fork.o simple_getpid.o \
       fork_and_termC.o fork_and_termP.o zomB.o reparent.o priority_ops.o \
       syscallC.o syscall.o exceptions.o fork_experiment.o

all-execs: $(EXEC) assembly

all: all-execs 

assembly: 
	gcc -c syscall.s
	ld -o syscall syscall.o

clean:
	rm -f $(OBJS) a.out $(EXEC) syscall 

help:
	echo all-execs/ all-objs /all /clean-all
