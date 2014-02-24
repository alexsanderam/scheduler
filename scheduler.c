/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include "scheduler.h"

//pthread_t threadSchedulerCore[cores->size];

void scheduling(List* cores, List* alreadyQueue, List* waitingQueue, List* finishedQueue)
{

	Core* core;
	Node* iterator;
	Job* job;

	float avg = avgCriterion(alreadyQueue);
		
	if(alreadyQueue->size != 0)
	{
		iteratorStart(cores);
		while((iterator = iteratorNext(cores)) != NULL)
		{
				core = (Core*) iterator->value;

				/*garante acesso exclusivo*/
				pthread_mutex_lock(&core->mux);

				/*casos que realizam a troca de job no core*/
				if(core->currentJob == NULL)
				{
					/*obtém o job que mais atendeu o critério de escalonamento*/
					job = getGreatestCriterionJob(alreadyQueue);
					job->status = RUNNING;
					core->currentJob = job;

					sem_post(&core->sem);

					/*remove da lista de prontos o job selecionado*/
					removeByValue(alreadyQueue, core->currentJob);
				}
				else if((CURRENT_QUANTUM(core->currentJob->currentStep) >= QUANTUM) || ((SHEDULER_DECISION(core->currentJob->currentStep, avg)) == 0))
				{
					/*obtém o job que mais atendeu o critério de escalonamento*/
					job = getGreatestCriterionJob(alreadyQueue);

					/*adiciona na lista de destino o job atual*/
					if(core->currentJob->status == FINISHED)
					{
						add(finishedQueue, core->currentJob);
						sem_post(&core->sem);
					}

					else if(core->currentJob->status == WAITING)
						add(waitingQueue, core->currentJob);

					else
						add(alreadyQueue, core->currentJob);	
	
					job->status = RUNNING;
					core->currentJob = job;
		
					/*remove da lista de prontos o job selecionado*/
					removeByValue(alreadyQueue, core->currentJob);
				}


				/*desbloqueia o acesso ao core*/
				pthread_mutex_unlock(&core->mux);
		}
	}
}


Job* getGreatestCriterionJob(List* alreadyQueue)
{
	Job* job;
	Job* theJob = NULL; /*the greatest*/
	Node* iterator;
	float maxCriterion = 0;
	float criterion = 0;
	unsigned int jobQuantum;
	unsigned int remainingTime;

	iteratorStart(alreadyQueue);
	while((iterator = iteratorNext(alreadyQueue)) != NULL)
	{				
		job = (Job*) iterator->value;

		remainingTime = job->currentStep - job->service_time;
		jobQuantum = CURRENT_QUANTUM(job->currentStep);
		
		if((criterion = SHEDULER_CRITERION(job->priority, remainingTime, jobQuantum)) > maxCriterion)
		{
			maxCriterion = criterion;
			theJob = job;
		}
	}

	return theJob;
}

float avgCriterion(List* alreadyJobs)
{
	Node* iterator;
	unsigned int jobQuantum;
	unsigned int remainingTime;
	float avg = 0; 

	Job* job;


	if(alreadyJobs->size == 0)
	{
		return 0;
	}

	iteratorStart(alreadyJobs);
	while ((iterator = iteratorNext(alreadyJobs)) != NULL)
	{
		job = (Job*) iterator->value;
		
		remainingTime = job->currentStep - job->service_time;
		jobQuantum = CURRENT_QUANTUM(job->currentStep);
		
		avg += SHEDULER_CRITERION(job->priority, remainingTime, jobQuantum);
	}

	avg /= alreadyJobs->size;

	return avg;

}
