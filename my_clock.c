/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014*/


#include "my_clock.h"


unsigned int sclock = 0;

unsigned int getClock()
{
	unsigned int* c;

	/*assim está apenas um acesso por vez, mas como é quase atômica a operação
	deixamos assim mesmo. Mas o certo era aplicar o problema do leitor e escritor*/

	pthread_mutex_lock(&mux);
		c = &sclock;
	pthread_mutex_unlock(&mux);
	
	return *c;
}

unsigned int increaseClock()
{
	pthread_mutex_lock(&mux);
		sclock++;

		//meio segundo de delay
		usleep(DELAY);
	pthread_mutex_unlock(&mux);

	return sclock;
}
