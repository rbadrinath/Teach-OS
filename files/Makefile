CC=gcc
EXEC = check-fileinfo flush usingdup test_link test2_link \
       file_as_a_lock readfile pipecmd redirect repeated_printf flocking

all: $(EXEC)

clean:
	rm -f a.out *.o $(EXEC)

clean-objs:
	rm -f *.o

help:
	echo all-execs/ all-objs /all /clean-all
