/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef SHEDULER_H
    #define SHEDULER_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include "list_struct.h"
	#include "core.h"


	#define P_MIN 0 //prioridade mínima
	#define P_MAX 10 //prioridade máxima

	#define SRT_MIN 0 //tempo restante mínimo
	#define SRT_MAX 20 //tempo restante máximo

	#define SHEDULER_DECISION(CRITERION, AVG) CRITERION >= AVG
	#define CURRENT_QUANTUM(CURRENT_STEP) CURRENT_STEP % QUANTUM

	#define ROUND_ROB 0
	#define MY_POLICY 1

	void* scheduling(List*, List*, List*, List*, int);
	void makeBasicScheduling(Core*, List*, int);
	void makeSchedulingDueStatusJob(Core*, List*, List*, List*, int);
	void makeSchedulingGap(Core*, List*, int);
	Job* getGreatestCriterionJob(List*);
	float avgCriterion(List*);

	float N_WP();
	float N_WSRT();
	float N_WQ();
	int digit(int value);

	float SHEDULER_CRITERION(int, int, int);

#endif
