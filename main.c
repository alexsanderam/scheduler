/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/

#include<stdio.h>
#include<stdlib.h>
#include "simulator.h"

int main(int argc, char** argv)
{
	Simulator* simulator;

	if(argc == 1)
		return 1;

	simulator = newSimulator(argv[1], 4);
	printJobs(simulator->createdsQueue);
	startSimulator(simulator);
	showBenchMarkResults(simulator);
	closeSimulator(simulator);

	return 0;
}
