
EXECS= cl srv conc long_cl

all: $(EXECS)

cl: tcp_client.c commons.h
	gcc -o cl tcp_client.c   

long_cl: tcp_long_client.c commons.h
	gcc -o long_cl tcp_long_client.c

srv: tcp_server.c commons.h
	gcc -o srv tcp_server.c   

conc: conc_server.c commons.h
	gcc -o conc conc_server.c   

clean:
	rm -f $(EXECS)
