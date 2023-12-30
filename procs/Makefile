CC=gcc
CFLAGS= -g
DEPS = 
OBJ = 
EXEC = loopy  shell  simple_exec simple_fork  simple_getpid zomB reparent priority_ops
OBJS = loopy.o  shell.o  simple_exec.o  simple_fork.o simple_getpid.o \
       zomB.o reparent.o priority_ops.o

#OBJS = 

##$(EXEC):

all-execs: $(EXEC)

# all-objs: $(OBJS)

# all: all-objs all-execs
all: all-execs


clean:
	rm -f $(OBJ) a.out $(EXEC)

help:
	echo all-execs/ all-objs /all /clean-all
