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

	#define NEW 0
	#define RUNNING 1
	#define WAITING 2
	#define ALREADY 3
	#define FINISHED 4

    typedef struct {

		int id;
        char name[10];
        int arrival_time;
		int service_time;
        int priority;

		unsigned int status;
		float turnaroundTime;
		float waitingTime;
		float responseTime;
		//float CPUTime;
		unsigned int currentStep;

		float enterWaitingTime; //último instante que o job entrou para a fila de espera*/

		void (*function)();

    } Job;

	Job* createJob();
	Job* createJobWithFunction(void*);
	void killJob(Job*);
	unsigned int nextJobID();
	void printJob(Job);


#endif
