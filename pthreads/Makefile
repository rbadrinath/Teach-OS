EXECS= simple pc find_some_primes update_balance

all: $(EXECS)

# gettid() is linux specific and gives linux LWP ID to the thread
# compiling gives a warning, that I neglect
simple: simple.c
	gcc -o simple -pthread simple.c

update_balance: update_balance.c
	gcc -o update_balance -pthread update_balance.c
	

find_some_primes: find_some_primes.c
	gcc -o find_some_primes -pthread find_some_primes.c -lm


pc: prod_con.c safe_ops.c safe_ops.h
	gcc -o pc -pthread safe_ops.c prod_con.c

clean:
	rm -f $(EXECS) a.out
