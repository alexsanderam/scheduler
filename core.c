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
Core* createCore()
{
	Core* core = (Core*) malloc (sizeof(Core));
	core->currentJob = NULL;
	core->id = nextID++;

	return core;
}


void runCore(Core* core)
{		
	core->currentJob->currentStep++;

	if(core->currentJob->responseTime == 0)
		core->currentJob->responseTime = getSClock();

	updateStatus(core->currentJob);
}



/*
	Atribui um job ao core
*/
void assignToCore(Core* core, Job* job)
{
		job->status = RUNNING;
		core->currentJob = job;
}
