/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include<stdio.h>
#include<stdlib.h>
#include"simulator.h"
#include<string.h>
#include<math.h>

unsigned int castToInteger(char*);
int length(char*);

int main(int argc, char** argv)
{
	Simulator* simulator;
	int policy = MY_POLICY;
	int quantityOfCores = castToInteger(argv[2]);


	if((argc < 2) || (quantityOfCores < 1))
		return 1;

	if(argc > 3)
	{
		if (strcmp(argv[3], "--roundrobin") == 0)
		{
			policy = ROUND_ROB;
			printf("\n\tRound Robin\n");
		}
		else
			printf("\n\tRound Robin\n");
	}
	else
		printf("\n\tMy Policy\n");
	
	simulator = newSimulator(argv[1], quantityOfCores);
	//printJobs(simulator->createdsQueue);
	startSimulator(simulator, policy);
	showBenchMarkResults(simulator);
	closeSimulator(simulator);

	return 0;
}


unsigned int castToInteger(char *n)
{
	unsigned int r = 0; //this is the return of function
	int l = length(n);
	int i; //i
	
	for (i = l - 1; i >= 0; i--)
	{	
		r += (n[(l - 1) - i] - 48) * pow(10, i);
	}
	
	return r;
}

int length(char* n)
{
	int i = 0;
	for (i = 0; n[i] != '\0'; i++);
	
	return i;
}
