/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/


#include "simulator.h"

Simulator* newSimulator(char* path, unsigned int quantityOfCores)
{
	int i = 0;
	List* cores = createList();

	for(i = 0; i < quantityOfCores; i++)
		add(cores, createCore(DEFAULT_FREQUENCY, NULL));

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

void startSimulator(Simulator* simulator)
{
	List* cores = simulator->cores;
	Core* core;
	Node* iterator;

	pthread_mutex_init(&simulator->mux, NULL); /*inicializa mutex do simulator*/

	//prepara para iterar sobre a lista de cores
	iteratorStart(cores);

	while ((iterator = iteratorNext(cores)) != NULL)
	{
		core = (Core*) iterator->value;
		startCore(core);
	}

	/*join threads -----------------------------------------------------*/
	/*iteratorStart(cores);
	while ((iterator = iteratorNext(cores)) != NULL)
	{
		core = (Core*) iterator->value;

    	pthread_join(*core->t, NULL);
   	}*/	
	/* -----------------------------------------------------------------*/

	runSimulator(simulator);
}

void runSimulator(Simulator* simulator)
{
	unsigned int flag = 0;
	Node* iterator;
	Job* job;

	pthread_mutex_lock(&simulator->mux);
		flag = (simulator->quantityOfJobs > simulator->finishedQueue->size) || (simulator->waitingQueue->size > 0) || (simulator->alreadyQueue->size > 0) || (simulator->createdsQueue->size > 0);
	pthread_mutex_unlock(&simulator->mux);
	
	while(flag)
	{
		//incrementa o clock a cada passo
		increaseClock();

		pthread_mutex_lock(&simulator->mux);

			iteratorStart(simulator->createdsQueue);
			while ((iterator = iteratorNext(simulator->createdsQueue)) != NULL)
			{
				job = (Job*) iterator->value;
			
				if(job->arrival_time >= getClock())
				{
					add(simulator->alreadyQueue, job); /*adiciona na lista de prontos*/
					removeByValue(simulator->createdsQueue, job); /*remove da lista de criados*/
				}
			}

		pthread_mutex_unlock(&simulator->mux);

		//sheduling(simulator);

		pthread_mutex_lock(&simulator->mux);
			flag = (simulator->quantityOfJobs > simulator->finishedQueue->size) || (simulator->waitingQueue->size > 0) || (simulator->alreadyQueue->size > 0) || (simulator->createdsQueue->size > 0);
		pthread_mutex_unlock(&simulator->mux);
	}
}


void closeSimulator(Simulator* simulator)
{
	//deveria matar os jobs também...

	clear(simulator->waitingQueue);
	clear(simulator->finishedQueue);
	clear(simulator->alreadyQueue);
	clear(simulator->cores);
	
}

void showBenchMarkResults(Simulator* simulator)
{
		pthread_mutex_lock(&simulator->mux);
			printJobs(simulator->finishedQueue);
		pthread_mutex_unlock(&simulator->mux);
}


void printJobs(List* jobs)
{
	Job* job;
	Node* iterator;

	iteratorStart(jobs);
	while ((iterator = iteratorNext(jobs)) != NULL)
	{
		job = (Job*) iterator->value;
		printJob(*job);
   	}
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
