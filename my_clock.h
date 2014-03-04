/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014
*/
    #ifndef MY_CLOCK_H
    #define MY_CLOCK_H

	#include <stdio.h>
	#include <stdlib.h>

	#define DELAY_SCLOCK 50000 /*microsegundos*/

	void startSClock();
	unsigned int getSClock();
	unsigned int increaseSClock();


#endif
