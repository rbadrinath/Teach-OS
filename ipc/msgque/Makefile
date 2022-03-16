
EXECS= rd wr  ctl

all: $(EXECS)

ctl: msg_ctl.c common.h
	gcc -o ctl msg_ctl.c

rd: msg_reader.c common.h
	gcc -o rd msg_reader.c

wr: msg_writer.c common.h
	gcc -o wr msg_writer.c

clean:
	rm -f $(EXECS)
