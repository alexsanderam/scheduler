/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include "core.h"

unsigned int nextID = 0;

/*
	Entrada: frequência e cache do core a ser criado
	Saída: ponteiro de um novo core
*/
Core* createCore(float frequency, Cache* cache)
{
	Core* core = (Core*) malloc (sizeof(Core));
	
	core->frequency = frequency;
	core->cache = cache;
	core->currentJob = NULL;
	core->id = nextID++;

	return core;
}


/*
	Inicializa o novo core
*/
void startCore(Core* core)
{
	core->t = (pthread_t*) malloc (sizeof(pthread_t));
	sem_init(&core->sem, 0, 0); /*Inicializa o semáfaro com valor 0 para threads*/
	pthread_mutex_init(&core->mux, NULL); /*inicializa mutex do core*/
	pthread_create(core->t, NULL, runCore, (void*) core);
}


/*
	'Executa' o core
*/
void* runCore(void* c)
{
	Core* core = (Core*) c;
	Job* j;

	while(1)
	{

		/*delay do clock de acordo com a frequência do core*/
		usleep(((float) 1/(core->frequency)) * DELAY_SCLOCK);

		if((core->currentJob == NULL) || (core->currentJob->status == FINISHED) || (core->currentJob->status == WAITING))
			sem_wait(&core->sem); /*decrementa o semáfaro*/

		/*garante acesso exclusivo*/
		pthread_mutex_lock(&core->mux);
		j = core->currentJob;

		if(j->responseTime == 0)
			j->responseTime = getSClock(); //tempo de resposta

		if (j->function != NULL)
		{
			/*desbloqueia o acesso ao core*/
			pthread_mutex_unlock(&core->mux);
			j->function();
		}
		else
		{
			j->currentStep++;
			updateStatus(j);

			/*desbloqueia o acesso ao core*/
			pthread_mutex_unlock(&core->mux);
		}		

	}

	return 0;
}


void wakeUpCore(Core* core)
{
	sem_post(&core->sem);
}


/*
	Atualiza o estado de um job.
	Warning: esta função subentende que a exclusão mútua
	é garantida por quem a invoca.
*/
void updateStatus(Job* j)
{
	float waitingRate = 0;

	srand (time(NULL));
	waitingRate = (float)(rand() / (float) RAND_MAX);

	if((j->status == RUNNING) && (j->currentStep == j->service_time))
	{
		j->status = FINISHED;
		j->turnaroundTime = j->waitingTime + j->currentStep; //tempo atual menos o tempo de chegado na fila de prontos
	}
	else if((j->status == RUNNING) && (waitingRate >= LOCKED_RATE))
		j->status = WAITING;
}


/*
	Atribui um job ao core
	Warning: esta função subentende que a exclusão mútua
*/
void assignToCore(Core* core, Job* job)
{
		core->currentJob = job;
		job->status = RUNNING;
		job->waitingTime += (getSClock() - job->enterWaitingTime); //tempo de espera
}
