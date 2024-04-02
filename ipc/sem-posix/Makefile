EXECS= pc sem_ctl

all: $(EXECS)


pc: pc.c 
	gcc -o pc pc.c -pthread 

sem_ctl: sem_ctl.c 
	gcc -o sem_ctl sem_ctl.c -pthread 


clean:
	rm -f $(EXECS) a.out
