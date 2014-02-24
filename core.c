/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include "core.h"

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

		if((core->currentJob == NULL) || (core->currentJob->status == FINISHED))
		{
			sem_wait(&core->sem); /*decrementa o semáfaro*/
			//sem_post(&core->sem); /*incrementa o semáfaro*/
		}

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


/*
	Atualiza o estado de um job.
	Warning: esta função subentende que a exclusão mútua
	é garantida por quem a invoca.
*/
void updateStatus(Job* j)
{
	float lockedRate;

	srand (time(NULL));
	lockedRate = rand();

	if((j->status == RUNNING) && (j->currentStep == j->service_time))
	{
		j->status = FINISHED;
		j->turnaroundTime = getSClock() - j->arrival_time; //tempo atual menos o tempo de chegado na fila de prontos
	}
	else if((j->status == RUNNING) && (lockedRate >= LOCKED_RATE))
	{
		j->enterWaitingQueueTime = getSClock();
		j->status = WAITING;
	}
}


/*
	Atribui um job ao core
*/
void assignToCore(Core* core, Job* job)
{
	sem_wait(&core->sem);
		core->currentJob = job;
		job->status = RUNNING;

		job->waitingTime += (getSClock() - job->enterWaitingQueueTime); //tempo de espera

	sem_post(&core->sem);
}
