/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo e Ygor de Mello Canalli.
Data (última atualização): 18/09/2013
*/

    #ifndef LIST_STRUCT_H
    #define LIST_STRUCT_H    
    
    #include<stdio.h>
    #include<stdlib.h>

	#define CRESCENT 0
	#define DECRESCENT 1

    typedef struct _node Node;
  
    struct _node {
        struct _node* next;
        void (*value);
		float* ordering;
    };

    typedef struct {
        Node* head;
        Node* tail;
		Node* iterator;
        int size;
    } List;

    List* createList();
    void freeList(List*);
    int isEmpty(List);

    void iteratorStart(List*);
	Node* iteratorNext(List*);

    void putByIndex(List*, int, void(*));
    void putByPredecessor(List*, Node*, void(*));
    void add(List*, void(*));

	void insertionSort(List*, void (*), float*, int);

    Node* get(List, int);
    Node* find(List, void(*));
	Node* findPredecessorBiggestThen(List*, float);
	Node* findPredecessorSmallestThen(List*, float);

    void removeByIndex(List*, int);
    void removeByValue(List*, void(*));
    void removeByPredecessor(List*, Node*);
	void removeKNodesFromTail(List*, unsigned int);

    void clear(List*);

#endif
