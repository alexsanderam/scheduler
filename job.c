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

/*
	Atualiza o estado de um job.
*/
void updateStatus(Job* job)
{
	float waitingRate = 0;

	srand (time(NULL));
	waitingRate = (float)(rand() / (float) RAND_MAX);

	if((job->status == RUNNING) && (job->currentStep >= job->service_time))
	{
		job->status = FINISHED;
		job->waitingTime = (getSClock() - job->arrival_time - job->currentStep) + 1;
		job->turnaroundTime = job->waitingTime + job->currentStep;
	}
	//else if((job->status == RUNNING) && (waitingRate >= LOCKED_RATE))
		//job->status = WAITING;
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
		
		printf("\tTurnaround time: %d\n", job.turnaroundTime);
		printf("\tWaiting time: %d\n", job.waitingTime);
		printf("\tResponse time: %d\n", job.responseTime);		
		printf("\tRelative delay: %.2f\n", (float) job.turnaroundTime / job.service_time);
		printf("--------------------------------------\n");
}


/*
	Entrada: Caminho em que o arquivo que define o benchmark se encontra
	Saída: Uma lista de jobs contidos no arquivo
*/
List* readJobsFromFile(char* path)
{

	FILE* file;
	char readChar;

	List* jobs = NULL;
	Job* job;

	jobs = createList();
	job = createJob();

	//abre o arquivo
	file = fopen(path, "r");
	if (file == NULL)
		return NULL;

	/*ignora a primeira linha*/
	while((!feof(file)) && (readChar = fgetc(file) != '\n'));

	while(!feof(file))
	{
		/*cria um novo job*/
		job = createJob();

		//define o nome, o tempo de chegada, o tempo de serviço e a prioridade do job
		fscanf(file, "%s\t%d\t%d\t%d", job->name, &job->arrival_time, &job->service_time, &job->priority);

		//adiciona o job na lista de jobs de retorno em ordem crescente de chegada
		insertionSort(jobs, job, ((float*) &job->arrival_time), CRESCENT);

		//se não chegou no final do arquivo, lê um '\n'
		if(!feof(file))
			readChar = fgetc(file);

		if(readChar == '\0')
			readChar = fgetc(file);

	}

	//fecha o arquivo
	fclose(file);

	//retorna a lista de jobs
	return jobs;
}
