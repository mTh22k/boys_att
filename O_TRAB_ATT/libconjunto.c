#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libconjunto.h"

// Função para criar um novo conjunto
conjunto *cria_cjt (int max) {
    conjunto* conjuntoAux;
    int *v;

    // Aloca memória para a estrutura conjunto
    if(!(conjuntoAux = (conjunto*) malloc(sizeof(conjunto))))
        return NULL;

    // Aloca memória para o vetor que armazenará os elementos do conjunto
    if(!(v = (int*) malloc(sizeof(int) * max)))
        return NULL;

    // Inicializa os campos da estrutura conjunto
    conjuntoAux->max = max;
    conjuntoAux->ptr = 0;
    conjuntoAux->card = 0;
    conjuntoAux->v = v;

    return conjuntoAux;
}

// Função para destruir um conjunto, liberando memória alocada
conjunto *destroi_cjt (conjunto *c) {
    // Libera memória do vetor que armazena os elementos do conjunto
    free(c->v);

    // Libera memória da própria estrutura conjunto
    free(c);

    return NULL;
}

// Função para verificar se um conjunto está vazio
int vazio_cjt (conjunto *c) {
    if(c->card == 0) 
        return 1;

    return 0;
}

// Função para obter a cardinalidade de um conjunto
int cardinalidade_cjt (conjunto *c) {
    return c->card;
}

// Função para inserir um elemento em um conjunto
int insere_cjt (conjunto *c, int elemento) {
    if(pertence_cjt(c, elemento) == 1) 
        return 0;
    else if (c->card >= c->max) 
        return 0;

    c->card++;

    c->v[c->card - 1] = elemento;

    return 1;
}

// Função para retirar um elemento de um conjunto
int retira_cjt (conjunto *c, int elemento) {
    int x;

    if(vazio_cjt(c) == 1)
        return 0;

    if(pertence_cjt(c, elemento) == 0) 
        return 0;

    for(x=0; x < c->card && c->v[x] != elemento; x++);

    for(x++; x < c->card; x++) 
        c->v[x - 1] = c->v[x];

    c->card--;

    return 1;
}

// Função para verificar se um elemento pertence a um conjunto
int pertence_cjt (conjunto *c, int elemento) {
    int x;
    for(x = 0; x < c->card; x++) {
        if(c->v[x] == elemento)
            return 1;
    }

    return 0;
}

// Função para verificar se um conjunto está contido em outro
int contido_cjt (conjunto *c1, conjunto *c2) {
    int x;
    for(x = 0; x < c1->card; x++) {
        if(pertence_cjt(c2, c1->v[x]) == 0)
            return 0;
    }

    return 1;
}

// Função para verificar se dois conjuntos são iguais
int sao_iguais_cjt (conjunto *c1, conjunto *c2) {
    int x;
    if(c1->card != c2->card) 
        return 0;

    for(x = 0; x < c1->card; x++) {
        if(pertence_cjt(c2, c1->v[x]) == 0)
            return 0;
    }

    return 1;
}

// Função para calcular a diferença entre dois conjuntos
conjunto *diferenca_cjt (conjunto *c1, conjunto *c2) {
    int x;
    conjunto *conj = cria_cjt(c1->max);

    if(conj == NULL) 
        return NULL;

    for(x = 0; x < c1->card; x++) {
        if(pertence_cjt(c2, c1->v[x]) == 0)
            insere_cjt(conj, c1->v[x]);
    }

    return conj;
}

// Função para calcular a interseção entre dois conjuntos
conjunto *interseccao_cjt (conjunto *c1, conjunto *c2) {
    int x;
    conjunto *conj = cria_cjt(c1->max);

    if(conj == NULL) 
        return NULL;

    for(x = 0; x < c1->card; x++) {
        if(pertence_cjt(c2, c1->v[x]) == 1)
            insere_cjt(conj, c1->v[x]);
    }

    return conj;
}

// Função para calcular a união entre dois conjuntos
conjunto *uniao_cjt (conjunto *c1, conjunto *c2) {
    int x, y;
    conjunto *conj = cria_cjt(c1->max + c2->max);

    if(conj == NULL) 
        return NULL;

    for(x = 0; x < c1->card; x++) 
        insere_cjt(conj, c1->v[x]);

    for(y = 0; y < c2->card; y++)
        insere_cjt(conj, c2->v[y]);

    return conj;
}

// Função de busca binária para auxiliar na ordenação do conjunto
int busca(int *vetor, int inicio, int fim, int valor){
    if (inicio > fim)
        return inicio - 1;

    if (valor >= vetor[fim])
        return fim;

    return busca(vetor, inicio, fim - 1, valor);
}

// Função para trocar dois elementos em um vetor
void troca(int *vetor, int a, int b)
{
    int x = 0;
    x = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = x;
}

// Função para inserir um elemento em um vetor ordenado
void insere(int *vetor, int inicio, int fim)
{
    int resultado = busca(vetor, inicio, fim - 1, vetor[fim]);
    int i = fim;

    while (i > resultado + 1){
        troca(vetor, i, i - 1);
        i = i - 1;
    }
}

// Função de ordenação do vetor usando o algoritmo de inserção
void insertSort(int *vetor, int inicio, int fim){
    if (inicio >= fim)
        return;
    
    insertSort(vetor, inicio, fim - 1);
    insere(vetor, inicio, fim);
}

// Função para imprimir os elementos do conjunto em ordem crescente
void imprime_cjt (conjunto *c) {
    int x;
    int *v;

    if(c->card == 0){
        printf("conjunto vazio\n");
        return;
    }

    // Aloca memória para um vetor temporário
    if(!(v = (int*) malloc(sizeof(int) * c->card)))
        return;

    printf("[ ");

    for(x=0; x < c->card; x++)
        v[x] = c->v[x];

    // Ordena o vetor temporário
    insertSort(v, 0, c->card - 1);

    // Imprime os elementos do conjunto ordenados
    for(x = 0; x < c->card; x++) 
        printf("%d ", v[x]);

    printf("] ");
    printf("\n");

    // Libera memória do vetor temporário
    free(v);
}




