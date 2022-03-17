EXECS= simple pc

help:
	echo USAGE:  make all    or    make clean

all: 
	utils/mymake *
	


clean:
	utils/mymake clean *
