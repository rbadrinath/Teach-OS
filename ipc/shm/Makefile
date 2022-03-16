EXECS= shm_ctl  fl rd

all: $(EXECS)

shm_ctl: shm_ctl.c common.h
	gcc -o shm_ctl shm_ctl.c

rd: reader.c common.h
	gcc -o rd reader.c

fl: filler.c common.h
	gcc -o fl filler.c

clean:
	rm -f $(EXECS)
