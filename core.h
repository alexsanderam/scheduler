/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef CORE_H
    #define CORE_H 

	#include "my_clock.h"
	#include "cache.h"
	#include "job.h"
    
    #include <stdio.h>
    #include <stdlib.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <sys/types.h>
	#include <sys/syscall.h>
	#include <time.h>

	#define LOCKED_RATE 0.8
	#define DEFAULT_FREQUENCY 1


    typedef struct 
	{
		float frequency;
		Job* currentJob;

		Cache* cache;

		pthread_t* t; /*thread*/
		pthread_mutex_t mux; /*mutex*/

    } Core;

	Core* createCore(float, Cache*);
	void startCore(Core*);
	void* runCore(void*);
	void updateStatus(Job*);
	void assignToCore(Core*, Job*);

	
#endif
