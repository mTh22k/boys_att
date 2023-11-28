#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

// Função para criar uma nova fila
fila * cria_fila (){
    fila* filaAux;

    // Aloca memória para a estrutura fila
    if(!(filaAux = (fila*) malloc(sizeof(fila)))) 
        return NULL;            
    
    // Inicializa ponteiros e tamanho da fila
    filaAux->ini = NULL;
    filaAux->fim = NULL;
    filaAux->tamanho= 0;

    return filaAux;
}

// Função para destruir uma fila, liberando memória alocada
fila * destroi_fila (fila *f) {	
    int x = 0;
    int y;
    int pont = 0;
    
    // Verifica se a fila não está vazia
    if(vazia_fila(f) == 0) {
        x = tamanho_fila(f);
        
        // Remove todos os elementos da fila
        for(y = 0; y < x; y++) 
            retira_fila(f, &pont);
    }
    
    // Libera memória da própria fila
    free(f);
       
    f = NULL;

    return f;
}

// Função para verificar se a fila está vazia
int vazia_fila (fila *f) {
    if(f->ini == NULL) 
        return 1;
    
    return 0;
}

// Função para obter o tamanho atual da fila
int tamanho_fila (fila *f) {
    return f->tamanho;
}

// Função para inserir um elemento no final da fila
int insere_fila (fila *f, int elemento) {
    nodo_fila_f  *item = NULL;
    
    // Aloca memória para um novo nodo
    if(!(item = (nodo_fila_f*) malloc(sizeof(nodo_fila_f)))) 
        return 0;            
        
    // Configura o elemento associado ao nodo
    item->elem = elemento;
    item->prox = NULL;
        
    // Verifica se a fila está vazia
    if(vazia_fila(f) == 1) {
        f->ini = item;
        f->fim = item;
    } else {
        f->fim->prox = item;
        f->fim = item;
    }
    
    // Incrementa o tamanho da fila
    f->tamanho++;
    
    return 1;
}

// Função para retirar o primeiro elemento da fila
int retira_fila (fila *f, int *elemento) {
    nodo_fila_f  *item = NULL;
    
    // Verifica se a fila não está vazia
    if(vazia_fila(f) == 0) {
        // Remove o primeiro nodo da fila
        item = f->ini;
        f->ini = item->prox;
        *elemento = item->elem;
        f->tamanho--;
        free(item);
    
        return 1;
    } else 
        return 0;
}

// Função para imprimir os elementos da fila
void imprime_fila (fila *f) {
    nodo_fila_f *aux = f->ini;

    printf("[ ");
    while(aux){
        printf("%d ", aux->elem);
        aux = aux->prox;
    } 
    printf("] \n");
}