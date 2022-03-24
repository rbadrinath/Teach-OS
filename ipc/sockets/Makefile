
EXECS= cl srv conc

all: $(EXECS)

cl: tcp_client.c commons.h
	gcc -o cl tcp_client.c   

srv: tcp_client.c commons.h
	gcc -o srv tcp_server.c   

conc: conc_server.c commons.h
	gcc -o conc conc_server.c   

clean:
	rm -f $(EXECS)
