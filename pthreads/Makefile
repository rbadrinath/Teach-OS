EXECS= simple pc find_some_primes find_some_primes_2r update_balance update_balance_mutex

all: $(EXECS)

# gettid() is linux specific and gives linux LWP ID to the thread
# compiling gives a warning, that I neglect
simple: simple.c
	gcc -o simple -pthread simple.c

update_balance: update_balance.c
	gcc -o update_balance -pthread update_balance.c
	
update_balance_mutex: update_balance_mutex.c
	gcc -o update_balance_mutex -pthread update_balance_mutex.c
	
find_some_primes: find_some_primes.c
	gcc -o find_some_primes -pthread find_some_primes.c -lm

find_some_primes_2r: find_some_primes_2r.c
	gcc -o find_some_primes_2r -pthread find_some_primes_2r.c -lm

pc: prod_con.c safe_ops.c safe_ops.h
	gcc -o pc -pthread safe_ops.c prod_con.c

clean:
	rm -f $(EXECS) a.out
