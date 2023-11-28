#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libconjunto.h"

conjunto *cria_cjt (int max) {
	conjunto* conjuntoAux;
	int *v;
	
	if(!(conjuntoAux = (conjunto*) malloc(sizeof(conjunto))))
		return NULL;
	
	if(!(v = (int*) malloc(sizeof(int) * max)))
		return NULL;
	
	conjuntoAux->max=max;
	conjuntoAux->ptr=0;
	conjuntoAux->card=0;
	conjuntoAux->v=v;
	
	return conjuntoAux;
	
}

conjunto *destroi_cjt (conjunto *c) {
	free(c->v);
	
	free(c);
	
	return NULL;
}

int vazio_cjt (conjunto *c) {
	if(c->card==0) 
		return 1;
		
	return 0;
}

int cardinalidade_cjt (conjunto *c) {
	return c->card;
}

int insere_cjt (conjunto *c, int elemento) {
	if(pertence_cjt(c, elemento) == 1) 
		return 0;
	else if (c->card >= c->max) 
		return 0;
	
	c->card++;
	
	c->v[c->card - 1] = elemento;
	
	return 1;
}

int retira_cjt (conjunto *c, int elemento) {
	int x;
	
	if(vazio_cjt(c) == 1)
		return 0;
	
	if(pertence_cjt(c, elemento) == 0) 
		return 0;
		
	for(x=0; x < c->card && c->v[x] != elemento; x++);
	
	for(x++; x < c->card; x++) 
		c->v[x -1] = c->v[x];
		
	c->card--;
	
	return 1;
		
}

int pertence_cjt (conjunto *c, int elemento) {
	int x;
	for(x = 0; x < c->card; x++) {
		if(c->v[x] == elemento)
			return 1;
	}
	
	return 0;
}

int contido_cjt (conjunto *c1, conjunto *c2) {
	int x;
	for(x = 0; x < c1->card; x++) {
		if(pertence_cjt(c2, c1->v[x]) == 0)
			return 0;
	}
	
	return 1;
}

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


int busca(int *vetor, int inicio, int fim, int valor){

	if (inicio > fim)
		return inicio - 1;

	if (valor >= vetor[fim])
		return fim;

	return busca(vetor, inicio, fim - 1, valor);
}

void troca(int *vetor, int a, int b)
{
	int x = 0;
	x = vetor[a];
	vetor[a] = vetor[b];
	vetor[b] = x;
}

void insere(int *vetor, int inicio, int fim)
{
	int resultado = busca(vetor, inicio, fim - 1, vetor[fim]);
	int i = fim;

	while (i > resultado + 1){
		troca(vetor, i, i - 1);
		i = i - 1;
	}
}

void insertSort(int *vetor, int inicio, int fim){
	if (inicio >= fim)
		return;
	
	insertSort(vetor, inicio, fim - 1);
	insere(vetor, inicio, fim);
}

void imprime_cjt (conjunto *c) {
	int x;
	int *v;
	
	if(c->card == 0){
		printf("conjunto vazio\n");
		return;
	}
	
	if(!(v = (int*) malloc(sizeof(int) * c->card)))
		return;
	
	printf("[ ");

	for(x=0; x < c->card; x++)
		v[x] = c->v[x];
		
	insertSort(v, 0, c->card - 1);
	
	for(x = 0; x < c->card; x++) 
		printf("%d ", v[x]);
	

	printf("] ");
	printf("\n");
	
	free(v);
}


