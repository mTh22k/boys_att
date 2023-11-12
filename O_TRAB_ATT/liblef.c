#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

lef_t * cria_lef () {
	lef_t* lef;

	if(!(lef = (lef_t*) malloc(sizeof(lef_t)))) 
        return NULL;
    
    lef->Primeiro = NULL;
    
    return lef;   
}

lef_t * destroi_lef (lef_t *l) {
	nodo_lef_t *no = l->Primeiro, *aux;
	
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

int adiciona_inicio_lef (lef_t *l, evento_t *evento){
    nodo_lef_t* nodo;
	
    if(!(nodo = (nodo_lef_t*) malloc(sizeof(nodo_lef_t)))) 
        return 0;
    
    nodo->evento=evento;
    nodo->prox=l->Primeiro;
    l->Primeiro=nodo;
    return 1;
}

int adiciona_ordem_lef  (lef_t *l, evento_t *evento) {
   nodo_lef_t *nodo, *aux;
	
   if(!(nodo = (nodo_lef_t*) malloc(sizeof(nodo_lef_t)))) 
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

evento_t * obtem_primeiro_lef (lef_t *l) {
	return l->Primeiro->evento;
}
