#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

lef* cria_lef() {
    lef* lefAux;

    if (!(lefAux = (lef*)malloc(sizeof(lef))))
        return NULL;

    lefAux->Primeiro = NULL;

    return lefAux;
}


lef * destroi_lef (lef *l) {
	nodo_lef *no = l->Primeiro, *aux;
	
	int i = 0;
	while(no) {
		aux=no;
		no = aux->prox;
		free(aux->evento);
		free(aux);
		i++;
	}
	
	free(l);
	l = NULL;
	
	return l;
}

int add_inicio_lef (lef *l, evento *evento){
    nodo_lef* nodo;
	
    if(!(nodo = (nodo_lef*) malloc(sizeof(nodo_lef)))) 
        return 0;
    
    nodo->evento=evento;
    nodo->prox=l->Primeiro;
    l->Primeiro=nodo;
    return 1;
}

int add_ordem_lef  (lef *l, evento *evento) {
   nodo_lef *nodo, *aux;
	
   if(!(nodo = (nodo_lef*) malloc(sizeof(nodo_lef)))) 
        return 0;
        
    nodo->evento=evento;
    
    if(l->Primeiro == NULL){
    	nodo->prox = NULL;
    	l->Primeiro = nodo;
    } else if (nodo->evento->tempo < l->Primeiro->evento->tempo) {
    	nodo->prox = l->Primeiro;
    	l->Primeiro = nodo;
    } else {
    	aux = l->Primeiro;
    	
    	while(aux->prox && nodo->evento->tempo > aux->prox->evento->tempo) 
    		aux = aux->prox;
    	
    	nodo->prox = aux->prox;
    	aux->prox = nodo;
    }
    
    return 1;
}

evento * obtem_primeiro_lef (lef *l) {
	return l->Primeiro->evento;
}
