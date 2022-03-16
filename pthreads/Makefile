EXECS= simple pc

all: $(EXECS)

# gettid() is linux specific and gives linux LWP ID to the thread
# compiling gives a warning, that I neglect
simple: simple.c
	gcc -o simple -pthread simple.c


pc: prod_con.c safe_ops.c safe_ops.h
	gcc -o pc -pthread safe_ops.c prod_con.c

clean:
	rm -f $(EXECS)
