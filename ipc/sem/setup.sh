#!/bin/bash
# Make sure 777[765] exist and are initialize
./sem_ctl -n 7777
./sem_ctl -n 7776
./sem_ctl -n 7775

./sem_ctl -i 7777 1
./sem_ctl -i 7776 3
./sem_ctl -i 7775 0

