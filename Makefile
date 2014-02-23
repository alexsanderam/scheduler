all:
	clear
	gcc main.c simulator.c core.c cache.c job.c list_struct.c my_clock.c -o simulator -Wall -lm -pthread
