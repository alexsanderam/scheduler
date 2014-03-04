/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef SIMULATOR_H
    #define SIMULATOR_H

	#include "my_clock.h"
	#include "list_struct.h"
	#include "scheduler.h"
	#include "core.h"
    
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <time.h>
	#include "scheduler.h"


    typedef struct {

		List* createdsQueue;	/*lista de jobs criados*/	
		List* alreadyQueue;	/*lista de jobs com o estado pronto*/	
		List* waitingQueue; /*lista de jobs com o estado bloqueado*/
		List* finishedQueue; /*lista de jobs com o estado finalizado*/

		unsigned int quantityOfJobs; /*quantidade de jobs total*/

		List* cores;	/*lista de cores*/

    } Simulator;


	Simulator* newSimulatorWithCores(char*, List*);
	Simulator* newSimulator(char*, unsigned int);
	void startSimulator(Simulator*, int);
	void showBenchMarkResults(Simulator*);
	void closeSimulator(Simulator*);
	void printJobs(List*);
	

#endif
