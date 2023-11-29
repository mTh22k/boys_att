#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

// Função para criar uma nova Lista de Eventos Futuros (LEF)
lef* cria_lef() {
    lef* lefAux;

    // Aloca memória para a estrutura LEF
    if (!(lefAux = (lef*)malloc(sizeof(lef))))
        return NULL;

    // Inicializa o ponteiro do primeiro nodo como NULL
    lefAux->Primeiro = NULL;

    return lefAux;
}

// Função para destruir uma Lista de Eventos Futuros (LEF), liberando memória alocada
lef * destroi_lef (lef *l) {
	nodo_lef *no = l->Primeiro, *aux;
	
	// Itera sobre cada nodo na lista
	while(no) {
		aux = no;
		no = aux->prox;
		free(aux->evento);  // Libera memória associada ao evento
		free(aux);         // Libera memória do nodo
	}
	
	// Libera memória da própria lista
	free(l);
	l = NULL;
	
	return l;
}

// Função para adicionar um evento no início da Lista de Eventos Futuros (LEF)
int add_inicio_lef (lef *l, evento *evento){
    nodo_lef* nodo;
	
    // Aloca memória para um novo nodo
    if(!(nodo = (nodo_lef*) malloc(sizeof(nodo_lef)))) 
        return 0;
    
    // Configura o evento associado ao nodo
    nodo->evento = evento;
    
    // Atualiza os ponteiros para inserir o novo nodo no início da lista
    nodo->prox = l->Primeiro;
    l->Primeiro = nodo;
    
    return 1;
}

// Função para adicionar um evento mantendo a ordem crescente de tempo na LEF
int add_ordem_lef  (lef *l, evento *evento) {
   nodo_lef *nodo, *aux;
	
   // Aloca memória para um novo nodo
   if(!(nodo = (nodo_lef*) malloc(sizeof(nodo_lef)))) 
        return 0;
        
    // Configura o evento associado ao nodo
    nodo->evento = evento;
    
    // Verifica se a lista está vazia ou se o novo evento deve ser inserido no início
    if(l->Primeiro == NULL){
    	nodo->prox = NULL;
    	l->Primeiro = nodo;
    } else if (nodo->evento->tempo < l->Primeiro->evento->tempo) {
    	// Caso o novo evento tenha tempo menor que o primeiro evento na lista
    	nodo->prox = l->Primeiro;
    	l->Primeiro = nodo;
    } else {
    	aux = l->Primeiro;
    	
    	// Encontra a posição correta para inserção mantendo a ordem crescente de tempo
    	while(aux->prox && nodo->evento->tempo > aux->prox->evento->tempo) 
    		aux = aux->prox;
    	
    	nodo->prox = aux->prox;
    	aux->prox = nodo;
    }
    
    return 1;
}

// Função para obter o evento associado ao primeiro nodo na LEF
evento * obtem_primeiro_lef (lef *l) {
	return l->Primeiro->evento;
}
