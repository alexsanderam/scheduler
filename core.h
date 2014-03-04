/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Gomes da Silva Sies e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014

dL'essentiel est invisible pour les yeux
*/
    #ifndef CORE_H
    #define CORE_H 

	#include "job.h"
    
    #include <stdio.h>
    #include <stdlib.h>
	#include <unistd.h>
	#include "my_clock.h"


    typedef struct 
	{
		unsigned int id;
		Job* currentJob;

    } Core;

	Core* createCore();
	void runCore(Core*);
	void assignToCore(Core*, Job*);

	
#endif
