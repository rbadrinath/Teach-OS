
EXECS= rd wr 

all: $(EXECS)

rd: fifo-reader.c common.h
	gcc -o rd fifo-reader.c

wr: fifo-writer.c common.h
	gcc -o wr fifo-writer.c

#just for fun
fifo: 
	mkfifo /tmp/fifo1

clean:
	rm -f $(EXECS)
