/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo, Renan Sies Gomes e Ygor de Mello Canalli.
Data (última atualização): 22/02/2014
*/
    #ifndef CACHE_H
    #define CACHE_H 
    
    #include<stdio.h>
    #include<stdlib.h>

    typedef struct {

		int capacity;
		float rateCacheMiss;

    } Cache;


	Cache* createCache(int, float);


#endif
