/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include "scheduler.h"


void* scheduling(List* cores, List* alreadyQueue, List* waitingQueue, List* finishedQueue)
{
	Core* core;
	Node* iterator;	
	float avg;

	avg = avgCriterion(alreadyQueue);

	//if(alreadyQueue->size == 0)
		//return 0;

	iteratorStart(cores);
	while((iterator = iteratorNext(cores)) != NULL)
	{
		core = (Core*) iterator->value;

			if(alreadyQueue->size > 0)
			{
				/*garante acesso exclusivo*/
				pthread_mutex_lock(&core->mux);

				if(core->currentJob == NULL)
				{
					makeBasicScheduling(core, alreadyQueue);
					wakeUpCore(core);
				}
				else if(core->currentJob->status == WAITING)
				{
					makeSchedulingDueStatusJob(core, alreadyQueue, waitingQueue, finishedQueue);
					wakeUpCore(core);
				}
				else if((CURRENT_QUANTUM(core->currentJob->currentStep) >= QUANTUM) || ((SHEDULER_DECISION(core->currentJob->currentStep, avg)) == 0))
					makeSchedulingGap(core, alreadyQueue);

				/*libera o acesso ao core*/
				pthread_mutex_unlock(&core->mux);
			}
			

			if(core->currentJob->status == FINISHED)
			{
				/*garante acesso exclusivo*/
				pthread_mutex_lock(&core->mux);

				makeSchedulingDueStatusJob(core, alreadyQueue, waitingQueue, finishedQueue);
				wakeUpCore(core);

				/*libera o acesso ao core*/
				pthread_mutex_unlock(&core->mux);
			}

	}

	return 0;
}


void makeBasicScheduling(Core* core, List* alreadyQueue)
{
	Job* job;

	/*obtém o job que mais atendeu o critério de escalonamento*/
	job = getGreatestCriterionJob(alreadyQueue);

	if(job != NULL)
	{
		assignToCore(core, job);
		printf("\n<Atribuiu>[%d] o job: %s - %d", core->id, job->name, getSClock());
	}
}


void makeSchedulingDueStatusJob(Core* core, List* alreadyQueue, List* waitingQueue, List* finishedQueue)
{
	if(core->currentJob->status == WAITING)
		add(waitingQueue, core->currentJob);
	else if(core->currentJob->status == FINISHED)
		add(finishedQueue, core->currentJob);

	if(core->currentJob->status == WAITING)
		printf("\n[WAITING]: o job:: %s", core->currentJob->name);
	else
		printf("\n[FINISHED]: o job:: %s", core->currentJob->name);

	makeBasicScheduling(core, alreadyQueue);
}

void makeSchedulingGap(Core* core, List* alreadyQueue)
{
	printf(">>>");
	//add(alreadyQueue, core->currentJob);
	makeBasicScheduling(core, alreadyQueue);
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

		remainingTime = job->service_time - job->currentStep;
		jobQuantum = CURRENT_QUANTUM(job->currentStep);
		
		if((criterion = SHEDULER_CRITERION(job->priority, remainingTime, jobQuantum)) >= maxCriterion)
		{
			maxCriterion = criterion;
			theJob = job;
		}
	}

	/*remove da lista de prontos o job selecionado*/
	if(theJob != NULL)
		removeByValue(alreadyQueue, theJob);

	return theJob;
}

float avgCriterion(List* alreadyJobs)
{
	Node* iterator;
	unsigned int jobQuantum = 0;
	unsigned int remainingTime = 0;
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
		
		remainingTime = job->service_time - job->currentStep;
		jobQuantum = CURRENT_QUANTUM(job->currentStep);
		
		avg += SHEDULER_CRITERION(job->priority, remainingTime, jobQuantum);
	}

	avg /= alreadyJobs->size;

	return avg;

}
