
EXECS= tcp_c tcp_s tcp_concs tcp_long_cl   \
       udp_s udp_c udp_echo_s \
       unx_s unx_c 

all: $(EXECS)

tcp_c: tcp_client.c commons.h
	gcc -o tcp_c tcp_client.c   

tcp_long_cl: tcp_long_client.c commons.h
	gcc -o tcp_long_cl tcp_long_client.c

tcp_s: tcp_server.c commons.h
	gcc -o tcp_s tcp_server.c   

tcp_concs: conc_server.c commons.h
	gcc -o tcp_concs conc_server.c   


udp_s: udp_server.c commons.h
	gcc -o udp_s udp_server.c

udp_c: udp_client.c commons.h
	gcc -o udp_c udp_client.c

udp_echo_s: udp_echo_server.c commons.h
	gcc -o udp_echo_s udp_echo_server.c

unx_s: udp_server.c commons.h
	gcc -o unx_s udp_server.c

unx_c: udp_client.c commons.h
	gcc -o unx_c udp_client.c

clean:
	rm -f $(EXECS)
