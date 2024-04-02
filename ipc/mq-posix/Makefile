EXECS= mq_ctl

all: $(EXECS)

mq_ctl: mq_ctl.c
	gcc -o mq_ctl mq_ctl.c -lrt

clean:
	rm -f $(EXECS) a.out
