/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/


#include "simulator.h"

Simulator* newSimulator(char* path, unsigned int quantityOfCores)
{
	int i = 0;
	List* cores = createList();

	for(i = 0; i < quantityOfCores; i++)
		add(cores, createCore());

	return newSimulatorWithCores(path, cores);
}

Simulator* newSimulatorWithCores(char* path, List* cores)
{

	Simulator* simulator = (Simulator*) malloc (sizeof(Simulator));
	simulator->waitingQueue = createList();
	simulator->finishedQueue = createList();
	simulator->alreadyQueue = createList();
	simulator->cores = cores;

	/*todos os jobs já são criados na fila de pronto*/
	simulator->createdsQueue = readJobsFromFile(path);
	simulator->quantityOfJobs = simulator->createdsQueue->size;

	return simulator;
}

void startSimulator(Simulator* simulator, int policy)
{
	Core* core;
	Job* job;
	Node* iterator;

	float waitingRate = 0;

	srand (time(NULL));
	startSClock();
	
	while(simulator->quantityOfJobs > simulator->finishedQueue->size)
	{	

		/*da lista de novos para a lista de prontos*/
		iteratorStart(simulator->createdsQueue);
		while ((iterator = iteratorNext(simulator->createdsQueue)) != NULL)
		{
			job = (Job*) iterator->value;
		
			if(job->arrival_time <= getSClock())
			{
				 /*adiciona na lista de prontos em ordem não crescente de prioridades*/
				add(simulator->alreadyQueue, job);
				//insertionSort(simulator->alreadyQueue, job, ((float*) &job->priority), DECRESCENT);
				removeByValue(simulator->createdsQueue, job); /*remove da lista de criados*/
			}
		}

		/*da lista de bloqueados para a lista de prontos*/
		iteratorStart(simulator->waitingQueue);
		while ((iterator = iteratorNext(simulator->waitingQueue)) != NULL)
		{
			waitingRate = (float)(rand() / (float) RAND_MAX);
			job = (Job*) iterator->value;
		
			if(waitingRate <= LOCKED_RATE)
			{
				job->status = ALREADY;
				 /*adiciona na lista de prontos em ordem não crescente de prioridades*/
				add(simulator->alreadyQueue, job);
				//insertionSort(simulator->alreadyQueue, job, ((float*) &job->priority), DECRESCENT);
				removeByValue(simulator->waitingQueue, job); /*remove da lista de bloqueados*/
			}
		}

		/*delay do clock*/
		//usleep(DELAY_SCLOCK);

		scheduling(simulator->cores, simulator->alreadyQueue, simulator->waitingQueue, simulator->finishedQueue, policy);

		/*Pecorre os cores*/
		iteratorStart(simulator->cores);
		while ((iterator = iteratorNext(simulator->cores)) != NULL)
		{
			core = (Core*) iterator->value;
			if(core->currentJob != NULL)
				runCore(core);
		}

		/*incrementa o clock*/
		increaseSClock();

	}
}


void closeSimulator(Simulator* simulator)
{
	clearAll(simulator->waitingQueue);
	clearAll(simulator->finishedQueue);
	clearAll(simulator->alreadyQueue);
	clearAll(simulator->cores);
	
}

void showBenchMarkResults(Simulator* simulator)
{
	printJobs(simulator->finishedQueue);
}


void printJobs(List* jobs)
{
	Job* job;
	Node* iterator;
	float avgResponseTime = 0;
	float avgRelativeDelay = 0;

	iteratorStart(jobs);
	while ((iterator = iteratorNext(jobs)) != NULL)
	{
		job = (Job*) iterator->value;
		printJob(*job);
		avgResponseTime += job->responseTime;
		avgRelativeDelay += (float) job->turnaroundTime/job->service_time;
   	}

	avgResponseTime /= jobs->size;
	avgRelativeDelay /= jobs->size;
	printf("\n\t>> Average response time: %.2f\n", avgResponseTime);
	printf("\tAverage relative delay: %.2f\n", avgRelativeDelay);
}

