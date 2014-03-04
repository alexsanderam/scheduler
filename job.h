/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef JOB_H
    #define JOB_H 
    
    #include<stdio.h>
    #include<stdlib.h>
	#include <time.h>
	#include "my_clock.h"
	#include "list_struct.h"

	#define NEW 0
	#define RUNNING 1
	#define WAITING 2
	#define ALREADY 3
	#define FINISHED 4

	#define LOCKED_RATE 0.8

    typedef struct {

		int id;
        char name[10];
        unsigned int arrival_time;
		unsigned int service_time;
        unsigned int priority;

		unsigned int status;
		unsigned int turnaroundTime;
		unsigned int waitingTime;
		unsigned int responseTime;
		unsigned int currentStep;

		void (*function)();

    } Job;

	Job* createJob();
	Job* createJobWithFunction(void*);
	void killJob(Job*);
	void updateStatus(Job*);
	unsigned int nextJobID();
	void printJob(Job);
	List* readJobsFromFile(char*);


#endif
