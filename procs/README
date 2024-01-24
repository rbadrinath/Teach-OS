System Calls and exceptions
===========================
syscallC.c		Making a syscall from C calls  WRITE(2)
syscall.s		Making a syscall from assembly calls WRITE(2) & EXIT(2)
exceptions.c		Experiment with a few different exceptions,
				run with strace


See time spent in Kernel and User mode
======================================
loopy.c			Creating processes that loop with or without sleep 
                          to see how they consume cpu. 
			  Use ./loopy sleepy to use sleep(2) to consume 
								little time.
			  Use ./loopy busy   to consume all time in user space.
			  Use ./loopy kernel to force it to consume kernel time.
			Additionally use it with the time(1) command to see usage
			You can also use top and ps -l to see time it uses
			Also good to check CPU allocation within 
					an autogroup *like created by a TTY.

Getting and setting priorty and scheduling policy
======================================================
priority_ops.c
sched_info.c
sched_prioritychange.c
sched_policychange.c		: use -h  or --help to see what the options are


Using fork(2), exec(2) and wait(2)
==================================
simple_getpid.c		Just a call to get the pid
simple_exec.c		A first intro to exec()
simple_fork.c		A first intro to fork()
shell.c			A very simple shell - combining the above ideas
zomB.c			To create a zombie process
fork_and_termC.c	To see what happens when child is terminated 
				(but not yet reaped)
fork_and_termP.c	To see what happens when parent is terminated
fork_experiment.c	To see some experiment with IO buffers and addresses
reparent.c		To create a process which gets re-parented - 
				- similar to fork_and_termP.c


Experimenting with fd dup(2)
============================
duper.c			Experimenting with the dup(fd) to see what happens
