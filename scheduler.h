/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef SHEDULER_H
    #define SHEDULER_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include "list_struct.h"
	#include "core.h"


	#define WP 1 	//peso do critério prioridade na decisão do escalonador
	#define WSRT 1	//peso do critério SRT na decisão do escalonador
	#define WQ 1	//peso do critério quantum na decisão do escalonador

	#define QUANTUM 5 //tamanho do quantum

	#define SHEDULER_CRITERION(P, SRT, Q) (WP*P + WSRT*SRT - WQ*Q) / 3
	#define SHEDULER_DECISION(CRITERION, AVG) CRITERION >= AVG
	#define CURRENT_QUANTUM(CURRENT_STEP) CURRENT_STEP % QUANTUM

	void scheduling(List*, List*, List*, List*);
	Job* getGreatestCriterionJob(List*);
	float avgCriterion(List*);

#endif
