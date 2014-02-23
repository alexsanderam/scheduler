/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014
*/


#include "job.h"

//==========================================Job==========================================

/*
	Saída: um novo job sem função definida
*/
Job* createJob()
{
	return createJobWithFunction(NULL);
}


/*
	Saída: um novo job com função definida
*/
Job* createJobWithFunction(void* function)
{
	Job* job = (Job*) malloc (sizeof(Job));
	
	job->id = nextJobID();
	
	job->status = NEW;
	job->turnaroundTime = 0;
	job->waitingTime = 0;
	job->responseTime = 0;
	job->currentStep = 0;

	job->function = function;

	return job;
}


/*
	Desaloca o trabalho da memória
*/
void killJob(Job* job)
{
	return free(job);
}

/* 
	Saída: próximo id para um novo job
*/
unsigned int nextJobID()
{
	static int id = 0;

	return id++;
}


void printJob(Job job)
{
		printf("\n-----------------Job------------------\n");
		printf("\tName: %s\n", job.name);
		printf("\tID: %d\n", job.id);
		printf("\tArrival time: %d\n", job.arrival_time);
		printf("\tService time: %d\n", job.service_time);
		printf("\tPriority: %d\n", job.priority);

		printf("\tStatus: ");
		if(job.status == NEW)
			printf("NEW\n");
		else if (job.status == RUNNING)
			printf("RUNNING\n");
		else if (job.status == WAITING)
			printf("WAITING\n");
		else if (job.status == FINISHED)
			printf("FINISHED\n");
		else if (job.status == ALREADY)
			printf("ALREADY\n");
		
		printf("\tTurnaround time: %.2f\n", job.turnaroundTime);
		printf("\tWaiting time: %.2f\n", job.waitingTime);
		printf("\tResponse time: %.2f\n", job.responseTime);		
		printf("--------------------------------------\n");
}
