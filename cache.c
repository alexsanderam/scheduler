/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014
*/


#include "cache.h"

//==========================================Cache==========================================

/*
	Saída: um nova cache
*/
Cache* createCache(int capacity, float rateCacheMiss)
{
	Cache* cache = (Cache*) malloc (sizeof(Cache));
	
	cache->capacity = capacity;
	cache->rateCacheMiss = rateCacheMiss;

	return cache;
}
