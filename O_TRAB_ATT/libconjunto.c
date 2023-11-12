#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libconjunto.h"

conjunto_t *cria_cjt (int max) {
	conjunto_t* conjunto;
	int *v;
	
	if(!(conjunto = (conjunto_t*) malloc(sizeof(conjunto_t))))
		return NULL;
	
	if(!(v = (int*) malloc(sizeof(int) * max)))
		return NULL;
	
	conjunto->max=max;
	conjunto->ptr=0;
	conjunto->card=0;
	conjunto->v=v;
	
	return conjunto;
	
}

conjunto_t *destroi_cjt (conjunto_t *c) {
	free(c->v);
	
	free(c);
	
	return NULL;
}

int vazio_cjt (conjunto_t *c) {
	if(c->card==0) 
		return 1;
		
	return 0;
}

int cardinalidade_cjt (conjunto_t *c) {
	return c->card;
}

int insere_cjt (conjunto_t *c, int elemento) {
	if(pertence_cjt(c, elemento) == 1) 
		return 0;
	else if (c->card >= c->max) 
		return 0;
	
	c->card++;
	
	c->v[c->card - 1] = elemento;
	
	return 1;
}

int retira_cjt (conjunto_t *c, int elemento) {
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

int pertence_cjt (conjunto_t *c, int elemento) {
	int x;
	for(x = 0; x < c->card; x++) {
		if(c->v[x] == elemento)
			return 1;
	}
	
	return 0;
}

int contido_cjt (conjunto_t *c1, conjunto_t *c2) {
	int x;
	for(x = 0; x < c1->card; x++) {
		if(pertence_cjt(c2, c1->v[x]) == 0)
			return 0;
	}
	
	return 1;
}

int sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2) {
	int x;
	if(c1->card != c2->card) 
		return 0;
		
	for(x = 0; x < c1->card; x++) {
		if(pertence_cjt(c2, c1->v[x]) == 0)
			return 0;
	}
	
	return 1;
}

conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2) {
	int x;
	conjunto_t *conj = cria_cjt(c1->max);
	
	if(conj == NULL) 
		return NULL;
	
	for(x = 0; x < c1->card; x++) {
		if(pertence_cjt(c2, c1->v[x]) == 0)
			insere_cjt(conj, c1->v[x]);
	}
	
	return conj;
}

conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2) {
	int x;
	conjunto_t *conj = cria_cjt(c1->max);
	
	if(conj == NULL) 
		return NULL;
	
	for(x = 0; x < c1->card; x++) {
		if(pertence_cjt(c2, c1->v[x]) == 1)
			insere_cjt(conj, c1->v[x]);
	}
	
	return conj;
}


conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2) {
	int x, y;
	conjunto_t *conj = cria_cjt(c1->max + c2->max);
	
	if(conj == NULL) 
		return NULL;
	
	for(x = 0; x < c1->card; x++) 
		insere_cjt(conj, c1->v[x]);
	
	for(y = 0; y < c2->card; y++)
		insere_cjt(conj, c2->v[y]);
	
	return conj;
}

conjunto_t *copia_cjt (conjunto_t *c){
	int x;
	conjunto_t *conj = cria_cjt(c->max);
	
	if(conj == NULL) 
		return NULL;	
		
	conj->ptr=c->ptr;
	conj->card=c->card;
	
	for(x=0; x < c->card; x++)
		conj->v[x] = c->v[x];
	
	return conj;
}

conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n) {
	conjunto_t *conj;
	int x, elem;
	
	if(n >= c->card) 
		return copia_cjt(c);
	
	conj = cria_cjt(n);	
	
	for(x=0; x < n; x++){
		elem = rand() % (c->card - 1);
		
		if(insere_cjt(conj, c->v[elem]) == 0) 
			x--;
	}
	
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

void imprime_cjt (conjunto_t *c) {
	int x;
	int *v;
	
	if(c->card == 0){
		printf("conjunto vazio\n");
		return;
	}
	
	if(!(v = (int*) malloc(sizeof(int) * c->card)))
		return;
	
	for(x=0; x < c->card; x++)
		v[x] = c->v[x];
		
	insertSort(v, 0, c->card - 1);
	
	for(x = 0; x < c->card; x++) 
		printf("%d ", v[x]);
	
	printf("\n");
	
	free(v);
}

void inicia_iterador_cjt (conjunto_t *c) {
	c->ptr = 0;
}

int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterador) {
	if(c->ptr > c->card - 1) 
		return 0;
	else {
		*ret_iterador = c->v[c->ptr];
		c->ptr++;
		return 1;
	}
}

int retira_um_elemento_cjt (conjunto_t *c){
	int ret;
	int indice = rand() % c->card;
	
	ret = c->v[indice];
	
	for(indice++; indice < c->card; indice++) 
		c->v[indice -1] = c->v[indice];
		
	c->card--;
	
	return ret;
	
}

