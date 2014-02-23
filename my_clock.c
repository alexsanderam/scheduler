/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014*/


#include "my_clock.h"


unsigned int sclock;
pthread_t* t;

void startSClock()
{
	sclock = 0;
	t = (pthread_t*) malloc (sizeof(pthread_t));
	pthread_create(t, NULL, runSClock, NULL);
	free(t);
}

void* runSClock()
{
		while(1)
		{
			//delay
			usleep(DELAY_SCLOCK);
			//incrementa o clock
			increaseClock();
		}

		return 0;
}

unsigned int getSClock()
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
	pthread_mutex_unlock(&mux);

	return sclock;
}
