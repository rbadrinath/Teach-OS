EXECS= shm_ctl  fl rd

all: $(EXECS)

shm_ctl: shm_ctl.c common.h
	gcc -o shm_ctl shm_ctl.c -lrt

rd: reader.c common.h
	gcc -o rd reader.c -lrt

fl: filler.c common.h
	gcc -o fl filler.c -lrt

clean:
	rm -f $(EXECS) a.out
