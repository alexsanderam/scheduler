all:
	clear
	gcc -g main.c simulator.c core.c job.c list_struct.c my_clock.c scheduler.c -o simulator -Wall -lm -pthread
