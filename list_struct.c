/*
Universidade Federal Rural do Rio de Janeiro.
Instituto Multidisciplinar.
Departamento de Tecnologia e Linguagens.
Curso de Ciência da Computaćão.

Autores: Alexsander Andrade de Melo e Ygor de Mello Canalli.
Data (última atualização): 18/09/2013
*/


#include "list_struct.h"

//==========================================Lista Encadeada==========================================

/*
	Saída: uma Lista vazia
*/
List* createList()
{
	List* list = (List*) malloc (sizeof(List));
	if (list == NULL){
		printf ("\nErro ao alocar memoria para lista.");
		return NULL;
	}
	list->head = list->tail = NULL;
	list->iterator = NULL;
	list->size = 0;

	return list;
}


/*
	Entrada: Lista
	Saída: Se Lista é vazia ou não
*/
int isEmpty(List list)
{
	return (list.size == 0);
	//return ((&list == NULL) || (list.size == 0));
}


/*
	Entrada: Lista, ponteiro para Node predecessor, ponteiro void para o valor
	Efeito: Adiciona um novo Node na Lista com o valor passado, cujo predecessor é Node
*/
void putByPredecessor(List* list, Node* predecessor, void(* value))
{
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->value = value;

	//Verifica se a lista está vazia
	if (!isEmpty(*list))
	{
		/*Se o Node predecessor não é NULL então adiciona o novo elemento, caso contrário,
		desaloca newNode*/		
		if (predecessor != NULL)
		{	
			newNode->next = predecessor->next;
			predecessor->next = newNode;
			
			//Atualiza a cauda caso o Node predecessor for o último elemento da lista
			if(predecessor == list->tail)
				list->tail = newNode;				

			list->size++;
		}
		else
			free(newNode);
	} 
	else 
	{
		/*Como a lista esta vazia, o head e o tail da lista se tornam o newNode*/
		list->head = list->tail = newNode;
		newNode->next = NULL;
		list->size = 1;
	}
}


/*
	Entrada: Lista, ponteiro void para o valor, real da chave de ordenação, e tipo da ordenação: CRESCENTE ou DECRESCENTE
	Efeito: Adiciona ordenadamente um novo Node na Lista com o valor passado
*/
void insertionSort(List* list, void (*value), float* ordering, int type)
{
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->value = value;
	newNode->ordering = ordering;

	Node* predecessor;

	//Verifica se a lista está vazia
	if (!isEmpty(*list))
	{
		
		//Se o tipo de ordenação é decrescente, o predecessor é o elemento maior que a chave de ordenação passada
		if (type == DECRESCENT)
		{
			predecessor = findPredecessorBiggestThen(list, *ordering);
		}
		//Se o tipo de ordenação é crescente, o predecessor é o elemento menor que a chave de ordenação passada
		else
		{
			predecessor = findPredecessorSmallestThen(list, *ordering);
		}
		
		
		//Se o Node predecessor não é NULL então adiciona o novo elemento depois do predecessor
		if (predecessor != NULL)
		{	
			newNode->next = predecessor->next;
			predecessor->next = newNode;
			
			//Atualiza a cauda caso o Node predecessor for o último elemento da lista
			if(predecessor == list->tail)
				list->tail = newNode;				
		}
		//se retorna nulo, insira na cabeça
		else
		{
			newNode->next = list->head;			
			list->head = newNode;
		}
	
		list->size++;
	} 
	else 
	{
		/*Como a lista esta vazia, o head e o tail da lista se tornam o newNode*/
		list->head = list->tail = newNode;
		newNode->next = NULL;
		list->size = 1;
	}
}


/*
	Entrada: Lista, ponteiro void para o valor, real da chave de ordenação
	Saída: Node predecessor, com chave imediatamente maior que a passada. Caso a saíde seja nula, significa que não há precessor: o elemento deve ser inserido na cabeça
*/
Node* findPredecessorBiggestThen(List* list, float ordering)
{
	Node* iterator;
	iteratorStart(list);

	//se a chave passada é maior que a chave da cabeça, retorne nulo, para sinalizar a substituição da cabeça
	if (*list->head->ordering <= ordering)
		return NULL;

	//itere sobre a lista enquanto o próximo relemento do iterador não é nulo
	while ((iterator = iteratorNext(list))->next != NULL)
	{

		//se a lista possui algum elemento sem chave de ordenação, retorne nulo para inserção na cabeça
		if (iterator->next->ordering == NULL)
			return NULL;

		//se o próximo elemento da lista possui chave de ordenação menor que a passada, o elemento atual é o predecessor
		if (*iterator->next->ordering <= ordering)
			return iterator;


	}
	
	//se o próximo elemento do iterador for nulo, o predecessor é a cauda
	return list->tail;
}


/*
	Entrada: Lista, ponteiro void para o valor, real da chave de ordenação
	Saída: Node predecessor, com chave imediatamente menor que a passada. Caso a saíde seja nula, significa que não há precessor: o elemento deve ser inserido na cabeça
*/
Node* findPredecessorSmallestThen(List* list, float ordering)
{
	Node* iterator;
	iteratorStart(list);

	//se a chave passada é menor que a chave da cabeça, retorne nulo, para sinalizar a substituição da cabeça
	if (*list->head->ordering >= ordering)
		return NULL;

	//itere sobre a lista enquanto o próximo relemento do iterador não é nulo
	while ((iterator = iteratorNext(list))->next != NULL)
	{

		//se a lista possui algum elemento sem chave de ordenação, retorne nulo para inserção na cabeça
		if (iterator->next->ordering == NULL)
			return NULL;

		//se o próximo elemento da lista possui chave de ordenação maior que a passada, o elemento atual é o predecessor
		if (*iterator->next->ordering >= ordering)
			return iterator;


	}
	
	//se o próximo elemento do iterador for nulo, o predecessor é a cauda
	return list->tail;
}



/*
	Entrada: Lista, ponteiro void para o valor
	Efeito: Adiciona um novo Node na Lista com o valor passado, cujo predecessor é o último elemento corrente da lista
*/
void add(List* list, void (*value))
{
	//Verifica se a lista não é nula
	if (list != NULL)
	{
		/*o novo elemento é inserido na lista tendo como predecessor o nó list->tail*/
		putByPredecessor(list, list->tail, value);
	}
}



/*
	Entrada: Lista, índice do elemento que se deseja obter
	Saída: ponteiro do Node correspondente ao índice index
*/
Node* get(List list, int index)
{

	Node* iterator;
	int i = 0;

	//Verifica se a lista está vazia
	if (!isEmpty(list))
	{
		
		//Índice desejado está fora do tamanho da lista
		if(index >= list.size)
			return NULL;		
		
		//Itera até que i seja correspondente ao índice desejado
		for(iterator = list.head; i < index; iterator = iterator->next, i++);
		
		//Node correspondente ao índice desejado localizado	
		return iterator;			
	}

	return NULL;
}


/*
	Entrada: Lista, índice da posição que deseja inserir o novo elemento
	Efeito: Insere elemento na posição desejada
*/
void putByIndex(List* list, int index, void (*value))
{
	Node* predecessor;
	
	//Encontra pelo índice index o elemento predecessor
	predecessor = get(*list, index - 1);

	//Insere o novo elemento após o elemento de posição index-1
	putByPredecessor(list, predecessor, value);
}


/*
	Entrada: Lista, valor do elemento que se deseja encontrar
	Saída: Ponteiro do elemento correspondente ao valor value
*/
Node* find(List list, void (*value))
{
	Node* iterator;

	//Verifica se a lista está vazia
	if (!isEmpty(list))
	{
		//Itera até que i que se encontre o elemento desejado ou chegue ao final da lista
		for(iterator = list.head; ((iterator != list.tail) && (iterator->value != value)); iterator = iterator->next);

				

		//Se elemento desejado foi localizado (se não chegou ao final da lista sem encontrar)
		if(iterator->value == value)	
			return iterator;			
	}

	
	return NULL;
}


/*
	Entrada: Lista, índice da posição do elemento que se deseja remover
	Efeito: Remove o elemento que se encontra na posição posterior ao predecessor
*/
void removeByPredecessor(List* list, Node* predecessor)
{
	//Verifica se a lista está vazia
	if (!isEmpty(*list))
	{
		/*Se o Node predecessor não é NULL então remove o elemento seguinte, caso este também não seja nulo*/		
		if ((predecessor != NULL) && (predecessor->next != NULL))
		{	
			printf("\nnext == null? %d \tnext->next == null? %d", predecessor->next == NULL, predecessor->next->next == NULL);

			Node* aux = predecessor->next;
			predecessor->next = predecessor->next->next;
			
			//Atualiza a cauda, caso o elemento a ser removido seja a cauda
			if(aux == list->tail)
				list->tail = predecessor;

			free(aux);
			list->size--;
		}
	} 
}


/*
	Entrada: Lista, índice da posição do elemento que se deseja remover
	Efeito: Remove o elemento que se encontra na posição index
*/
void removeByIndex(List* list, int index)
{
	Node* predecessor;

	if ((list->size == 1) && (index == 0))
		clear(list);
	else if(index == 0)
	{
		Node* aux = list->head;
		list->head = list->head->next;
			
		free(aux);
		list->size--;
	}
	else
	{
		//Encontra pelo índice index o elemento predecessor
		predecessor = get(*list, index - 1);

		//Remove elemento seguinte ao elemento da posição index-1
		removeByPredecessor(list, predecessor);
	}
}



/*
	Entrada: Lista, valor do elemento que se deseja remover
	Efeito: Remove o elemento correspondente ao valor value
*/
void removeByValue(List* list, void (*value))
{
	Node* predecessor;

	//Verifica se a lista está vazia
	if (!isEmpty(*list))
	{		
		//Se a lista só possui um elemento	
		if(list->size == 1)
		{
			//Se a cabeça da lista possui o valor desejado, retorne seu ponteiro, caso contrairo, retorne NULL
			if (list->head->value == value)
				clear(list);
		}
		//Se o valor está na cabeça, mas a lista tem mais elementos
		else if(list->head->value == value)
		{
			Node* aux = list->head;
			list->head = list->head->next;
			
			free(aux);
			list->size--;
		}
		//Se a lista possui mais que um elemento
		else
		{
			//Itera até que i que se encontre o elemento desejado ou chegue ao final da lista
			for(predecessor = list->head; ((predecessor->next != list->tail) && (predecessor->next->value != value)); predecessor = predecessor->next);

			//Se elemento desejado foi localizado (se não chegou ao final da lista sem encontrar)
			if(predecessor->next->value == value);
				removeByPredecessor(list, predecessor);	
		}
	}
}


/*
	Entrada: Lista que se deseja remover todos os elementos (desalocando-os)
	Efeito: Remove todos elementos da lista, desalocando-os
*/
void clear(List* list)
{
	Node *iterator, *aux;
	iterator = list->tail;

	//Itera enquanto o iterador iterator é diferente de nulo, ou seja, até que chegue ao final da lista
	while (iterator != NULL)
	{
		aux = iterator;
		iterator = iterator->next;
		free (aux);
	}

	list->size = 0;
	list->head = list->tail = NULL;
}


void clearAll(List* list)
{
	Node *iterator, *aux;
	iterator = list->tail;

	//Itera enquanto o iterador iterator é diferente de nulo, ou seja, até que chegue ao final da lista
	while (iterator != NULL)
	{
		aux = iterator;
		iterator = iterator->next;
		free (aux->value);
		free (aux);
	}

	list->size = 0;
	list->head = list->tail = NULL;
}

/*
	Entrada: List da qual se deseja inicializar o iterador 
	Efeito: Inicializa o iterator com a cabeça da lista
*/
void iteratorStart(List* list)
{
	list->iterator = list->head;
}


/*
	Entrada: Lista sobre a qual se deseja iterar
	Saída: Valor atual do iterador
	Efeito: Atualiza o iterador para iterator->next
*/
Node* iteratorNext(List* list)
{

	if(list->iterator != NULL)
	{
		Node* aux = list->iterator;
		list->iterator = list->iterator->next;

		return aux;
	}
	
	return NULL;
}

void removeKNodesFromTail(List* list, unsigned int k)
{
	List* auxList = createList();
	Node* auxNode;
	int endPosition = 0 ;

	//Se a quantidade de itens a remover for maior ou igual
	if (k >= list->size)
		clear(list);
	else
	{
		//Calcula a nova posição da cauda
		endPosition = list->size - 1 - k;

		//localiza o nó que será a nova cauda da lista
		auxNode = get(*list, endPosition);

		//atualiza a cabeça da lista auxiliar como o proximo elemento da cauda da lista principal	
		auxList->head = auxNode->next;

		//atualiza a cauda da lista auxiliar como a cauda da lista principal
		auxList->tail = list->tail;

		//atualiza a posição da cauda da lista principal
		list->tail = auxNode;
		list->tail->next = NULL;

		//atualiza o tamanho da lista
		list->size = list->size-k;

		//remove todos os elementos da lista auxililar e depois a desaloca
		clear(auxList);
		free(auxList);
		
	}
}
