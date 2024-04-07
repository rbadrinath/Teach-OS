
EXECS= cl srv conc long_cl udp_s  udp_c  unx_s unx_c

all: $(EXECS)

cl: tcp_client.c commons.h
	gcc -o cl tcp_client.c   

long_cl: tcp_long_client.c commons.h
	gcc -o long_cl tcp_long_client.c

srv: tcp_server.c commons.h
	gcc -o srv tcp_server.c   

conc: conc_server.c commons.h
	gcc -o conc conc_server.c   


udp_s: udp_server.c commons.h
	gcc -o udp_s udp_server.c

udp_c: udp_client.c commons.h
	gcc -o udp_c udp_client.c


unx_s: udp_server.c commons.h
	gcc -o unx_s udp_server.c

unx_c: udp_client.c commons.h
	gcc -o unx_c udp_client.c

clean:
	rm -f $(EXECS)
