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

	runSimulator(simulator);
}

void runSimulator(Simulator* simulator)
{
	unsigned int flag = 0;
	unsigned int timer;
	Node* iterator;
	Job* job;
	float waitingRate = 0;

	srand (time(NULL));
	waitingRate = (float)(rand() / (float) RAND_MAX);


	pthread_mutex_lock(&simulator->mux);
		flag = (simulator->quantityOfJobs > simulator->finishedQueue->size);
	pthread_mutex_unlock(&simulator->mux);


	/*inicializa o 'clock' do simulador*/
	startSClock();
	
	while(flag)
	{
		pthread_mutex_lock(&simulator->mux);

			timer = getSClock();

			/*da lista de novos para a lista de prontos*/
			iteratorStart(simulator->createdsQueue);
			while ((iterator = iteratorNext(simulator->createdsQueue)) != NULL)
			{
				job = (Job*) iterator->value;
			
				if(job->arrival_time >= timer)
				{
					 /*adiciona na lista de prontos em ordem não crescente de prioridades*/
					insertionSort(simulator->alreadyQueue, job, ((float*) &job->priority), DECRESCENT);
					removeByValue(simulator->createdsQueue, job); /*remove da lista de criados*/
				}
			}

			/*da lista de bloqueados para a lista de prontos*/
			iteratorStart(simulator->waitingQueue);
			while ((iterator = iteratorNext(simulator->waitingQueue)) != NULL)
			{
				job = (Job*) iterator->value;
			
				if(waitingRate <= LOCKED_RATE)
				{
					job->status = ALREADY;
					 /*adiciona na lista de prontos em ordem não crescente de prioridades*/
					insertionSort(simulator->alreadyQueue, job, ((float*) &job->priority), DECRESCENT);
					removeByValue(simulator->waitingQueue, job); /*remove da lista de criados*/
				}
			}


			/*delay do clock*/
			usleep(DELAY_SCLOCK);

			//if(simulator->alreadyQueue->size > 0)
			scheduling(simulator->cores, simulator->alreadyQueue, simulator->waitingQueue, simulator->finishedQueue);
	
			flag = (simulator->quantityOfJobs > simulator->finishedQueue->size);

			//printf("\n simulator->alreadyQueue->size: %d", simulator->alreadyQueue->size);
			//printf("\n simulator->waitingQueue->size: %d", simulator->waitingQueue->size);
			//printf("\n simulator->finishedQueue->size: %d", simulator->finishedQueue->size);
			//printf("\n simulator->createdsQueue->size: %d", simulator->createdsQueue->size);
		pthread_mutex_unlock(&simulator->mux);
	}
}


void closeSimulator(Simulator* simulator)
{
	Node* iterator;
	Core* core;

	//deveria matar os jobs também...
	clearAll(simulator->waitingQueue);
	clearAll(simulator->finishedQueue);
	clearAll(simulator->alreadyQueue);

	while ((iterator = iteratorNext(simulator->cores)) != NULL)
	{
		core = (Core*) iterator->value;
		sem_close(&core->sem);
   	}

	clearAll(simulator->cores);
	
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
