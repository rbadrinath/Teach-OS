#!/bin/bash
# remove the shared memory
./shm_ctl -r 8888

# remove the three semaphores 777[765]
./sem_ctl -r 7777
./sem_ctl -r 7776
./sem_ctl -r 7775

