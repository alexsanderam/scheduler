/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014
*/
    #ifndef MY_CLOCK_H
    #define MY_CLOCK_H

	#include <unistd.h>
	#include <pthread.h>
	#include <sys/types.h>
	#include <sys/syscall.h>

	#define DELAY 50000 /*microsegundos*/

	pthread_mutex_t mux; /*mutex*/

	unsigned int getClock();
	unsigned int increaseClock();



#endif
