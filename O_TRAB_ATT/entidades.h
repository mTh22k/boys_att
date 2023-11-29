#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 500
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

typedef struct heroi
{
	int id;
	int paciencia;
	int velocidade;
	int experiencia;
	conjunto *habilidades;
} Heroi;

typedef struct localizacao
{
	int x;
	int y;
} Localizacao;

typedef struct missao
{
	int id;
	Localizacao localizacao;
	conjunto *habilidades_nec;
} Missao;

typedef struct base
{
	int id;
	int lotacao_maxima;
	conjunto *presentes;
	fila *espera;
	Localizacao localizacao;
} Base;

typedef struct mundo
{
	int tempo_atual;
	Heroi herois[N_HEROIS];
	Base bases[N_BASES];
	Missao missoes[N_MISSOES];
	int n_herois;
	int n_habilidades;
	int n_bases;
	int n_missoes;
	int n_tamanho_mundo;
} Mundo;

// Função para gerar números aleatórios em um intervalo
int alet(int min, int max);

// Função para inicializar um herói com características aleatórias
Heroi cria_heroi(int id);

// Função para inicialiazar uma base com características aleatórias
Base cria_base(int id);

// Função para criar o mundo inicial
Mundo cria_mundo();

// Função para criar uma missão
Missao cria_missao(int id);

// Função para destruir a estrutura do mundo
void destroi_mundo(Mundo *mundo);