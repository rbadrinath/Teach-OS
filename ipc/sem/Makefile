
EXECS= sem_ctl shm_ctl pr cn

all: $(EXECS)

shm_ctl: shm_ctl.c safe_ops.c mysemops.h configs.h
	gcc -o shm_ctl shm_ctl.c

sem_ctl: sem_ctl.c mysemops.c  mysemops.h configs.h
	gcc -o sem_ctl sem_ctl.c mysemops.c

pr: shm_producer.c mysemops.c safe_ops.c mysemops.h safe_ops.h configs.h
	gcc -o pr shm_producer.c mysemops.c safe_ops.c

cn: shm_consumer.c mysemops.c safe_ops.c mysemops.h safe_ops.h configs.h
	gcc -o cn shm_consumer.c mysemops.c safe_ops.c

clean:
	rm -f $(EXECS)
