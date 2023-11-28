#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila * cria_fila (){
    fila* filaAux;
   
    if(!(filaAux = (fila*) malloc(sizeof(fila)))) 
        return NULL;            
    
    filaAux->ini = NULL;
    filaAux->fim = NULL;
    filaAux->tamanho= 0;

    return filaAux;
}


fila * destroi_fila (fila *f) {	
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

int vazia_fila (fila *f) {
	if(f->ini == NULL) 
		return 1;
	
	return 0;
}

int tamanho_fila (fila *f) {
	return f->tamanho;
}

int insere_fila (fila *f, int elemento) {
	nodo_fila_f  *item = NULL;
	
	if(!(item = (nodo_fila_f*) malloc(sizeof(nodo_fila_f)))) 
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

int retira_fila (fila *f, int *elemento) {
	nodo_fila_f  *item = NULL;
	
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

void imprime_fila (fila *f) {
	nodo_fila_f *aux = f->ini;

	printf("[ ");
	while(aux){
		printf("%d ", aux->elem);
		aux = aux->prox;
	} 
	printf("] \n");
}
