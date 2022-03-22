#!/bin/bash
# setup the shared memory and initialize the free and used pointers
./shm_ctl -n 8888 
./shm_ctl -i 8888

# Make sure the three semaphores 777[765] exist and are initialize
./sem_ctl -n 7777; ./sem_ctl -i 7777 1 # (mutex)
./sem_ctl -n 7776; ./sem_ctl -i 7776 2 # (empty slots)
./sem_ctl -n 7775; ./sem_ctl -i 7775 0 # (filled slots)

