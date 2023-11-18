#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t * cria_fila (){
    fila_t* fila;
   
    if(!(fila = (fila_t*) malloc(sizeof(fila_t)))) 
        return NULL;            
    
    fila->ini = NULL;
    fila->fim = NULL;
    fila->tamanho= 0;

    return fila;
}


fila_t * destroi_fila (fila_t *f) {	
	int x = 0;
	int y;
	int pont = 0;
	
	if(vazia_fila(f) == 0) {
		x = tamanho_fila(f);
		
		for(y = 0; y < x; y++) 
			retira_fila(f, &pont);
	}
	
    free(f);
       
    f = NULL;

    return f;
}

int vazia_fila (fila_t *f) {
	if(f->ini == NULL) 
		return 1;
	
	return 0;
}

int tamanho_fila (fila_t *f) {
	return f->tamanho;
}

int insere_fila (fila_t *f, int elemento) {
	nodo_f_t  *item = NULL;
	
	if(!(item = (nodo_f_t*) malloc(sizeof(nodo_f_t)))) 
        return 0;            
    		
    item->elem = elemento;
    item->prox = NULL;
    	
    if(vazia_fila(f) == 1) {
    	f->ini = item;
    	f->fim = item;
    } else {
    	f->fim->prox = item;
    	f->fim = item;
    }
	
	f->tamanho++;
	
	return 1;
}

int retira_fila (fila_t *f, int *elemento) {
	nodo_f_t  *item = NULL;
	
	if(vazia_fila(f) == 0) {
		item = f->ini;
		f->ini = item->prox;
		*elemento = item->elem;
		f->tamanho--;
		free(item);
	
		return 1;
	} else 
		return 0;
	
}

void imprime_fila (fila_t *f) {
	nodo_f_t *aux = f->ini;

	printf("[ ");
	while(aux){
		printf("%d ", aux->elem);
		aux = aux->prox;
	} 
	printf("] \n");
}
