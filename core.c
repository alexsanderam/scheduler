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

	return core;
}


/*
	Inicializa o novo core
*/
void startCore(Core* core)
{
	core->t = (pthread_t*) malloc (sizeof(pthread_t));
	
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

		/*bloqueia o acesso ao core*/
		pthread_mutex_lock(&core->mux);
		j = core->currentJob;

		if((j != NULL) && (j->status != FINISHED))
		{
				if(j->responseTime == 0)
					j->responseTime = getClock(); //tempo de resposta

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
		j->turnaroundTime = getClock() - j->arrival_time; //tempo atual menos o tempo de chegado na fila de prontos
	}
	else if((j->status == RUNNING) && (lockedRate >= LOCKED_RATE))
	{
		j->enterWaitingQueueTime = getClock();
		j->status = WAITING;
	}
}


/*
	Atribui um job ao core
*/
void assignToCore(Core* core, Job* job)
{
	pthread_mutex_lock(&core->mux);
		core->currentJob = job;
		job->status = RUNNING;

		job->waitingTime += (getClock() - job->enterWaitingQueueTime); //tempo de espera

	pthread_mutex_unlock(&core->mux);
}