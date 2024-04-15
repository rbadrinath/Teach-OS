
EXECS= rd wr both

all: $(EXECS)

rd: fifo-reader.c common.h
	gcc -o rd fifo-reader.c

wr: fifo-writer.c common.h
	gcc -o wr fifo-writer.c

both: fifo-re-wr.c
	gcc -o both fifo-re-wr.c

#just for fun
fifo: 
	mkfifo /tmp/fifo1

clean:
	rm -f $(EXECS) a.out
