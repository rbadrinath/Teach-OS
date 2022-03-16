CC=gcc
CFLAGS= -g
DEPS = 
OBJ = 
EXEC = file_as_a_lock  loopy  shell  simple_exec simple_fork  test2_link  test_link printpid zomB reparent touch_bad_memory
OBJS = file_as_a_lock.o  loopy.o  shell.o  simple_exec.o  simple_fork.o \
       test2_link.o  test_link.o printpid.o zomB.o reinit.o

#OBJS = 

##$(EXEC):

all-execs: $(EXEC)

# all-objs: $(OBJS)

# all: all-objs all-execs
all: all-execs


clean:
	rm -f *.o a.out $(EXEC)

help:
	echo all-execs/ all-objs /all /clean-all
