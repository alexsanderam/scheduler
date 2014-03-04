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

unsigned int WP = 2; 	//peso do critério prioridade na decisão do escalonador
unsigned int WSRT = 3;	//peso do critério SRT na decisão do escalonador
unsigned int WQ = 5;	//peso do critério quantum na decisão do escalonador

unsigned int QUANTUM = 4; //tamanho do quantum


void* scheduling(List* cores, List* alreadyQueue, List* waitingQueue, List* finishedQueue, int policy)
{
	Core* core;
	Node* iterator;	
	float avg;

	float criterion = 0;
	int remainingTime = 0;
	int jobQuantum = 0;


	avg = avgCriterion(alreadyQueue);

	iteratorStart(cores);
	while((iterator = iteratorNext(cores)) != NULL)
	{
		core = (Core*) iterator->value;


		if(alreadyQueue->size > 0)
		{
			if(core->currentJob == NULL)
				makeBasicScheduling(core, alreadyQueue, policy);

			else if((core->currentJob->status == WAITING) || (core->currentJob->status == FINISHED))
				makeSchedulingDueStatusJob(core, alreadyQueue, waitingQueue, finishedQueue, policy);

			else if((core->currentJob->currentStep > 0) && (CURRENT_QUANTUM(core->currentJob->currentStep) == 0))
				makeSchedulingGap(core, alreadyQueue, policy);
			
			else if(policy ==  MY_POLICY)
			{
				remainingTime = core->currentJob->service_time - core->currentJob->currentStep;
				jobQuantum = CURRENT_QUANTUM(core->currentJob->currentStep);
				criterion = SHEDULER_CRITERION(core->currentJob->priority, remainingTime, jobQuantum);

				printf("\nP: %d - WP: %d\tSRT: %d - WSRT: %d\tQ: %d - WQ: %d", core->currentJob->priority, WP, remainingTime, WSRT, jobQuantum, WQ);
				printf("\tcriterion: %.2f \tavg: %.2f", criterion, avg);

				if((SHEDULER_DECISION(criterion, avg)) == 0)
					makeSchedulingGap(core, alreadyQueue, policy);
			}
		}
		else if((core->currentJob != NULL) && ((core->currentJob->status == WAITING) || (core->currentJob->status == FINISHED)))
		{
			if(core->currentJob->status == WAITING)
				add(waitingQueue, core->currentJob);
			else if(core->currentJob->status == FINISHED)
				add(finishedQueue, core->currentJob);

			core->currentJob = NULL;
		}
	}	

	return 0;
}


void makeBasicScheduling(Core* core, List* alreadyQueue, int policy)
{
	Job* job;

	switch (policy)
	{
		case ROUND_ROB: 
			job = (Job*) get(*alreadyQueue, 0)->value;
			removeByIndex(alreadyQueue, 0);
			break;

		default:
			/*obtém o job que mais atendeu o critério de escalonamento*/
			job = getGreatestCriterionJob(alreadyQueue);
			break;
	
	}

	//if(job != NULL)
	assignToCore(core, job);
}


void makeSchedulingDueStatusJob(Core* core, List* alreadyQueue, List* waitingQueue, List* finishedQueue, int policy)
{
	if(core->currentJob->status == WAITING)
		add(waitingQueue, core->currentJob);
	else if(core->currentJob->status == FINISHED)
		add(finishedQueue, core->currentJob);

	makeBasicScheduling(core, alreadyQueue, policy);
}

void makeSchedulingGap(Core* core, List* alreadyQueue, int policy)
{
	core->currentJob->status = ALREADY;
	add(alreadyQueue, core->currentJob);
	makeBasicScheduling(core, alreadyQueue, policy);
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
		return 0;

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

float N_WP()
{	
	return WP * (1.0 / (P_MAX - P_MIN));
}


float N_WSRT()
{	
	return WSRT * (1.0 / (SRT_MAX - SRT_MIN));
}

float N_WQ()
{	
	return WQ * (1.0 / QUANTUM);
}

int digit(int value)
{
	int i = 1;

	while((value / i) > 0)
		i *= 10;

	return i / 10;
}

float SHEDULER_CRITERION(int P, int SRT, int Q)
{
	float NWP = N_WP();
	float NWSRT = N_WSRT();
	float NWQ = N_WQ();

	return (NWP*P + NWSRT*(1.0/SRT)*digit(SRT_MAX) + NWQ*(1.0/(Q+1))*digit(QUANTUM)) / (NWP + NWSRT + NWQ);
}
