#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"

// Função para gerar números aleatórios em um intervalo
int alet(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

Heroi cria_heroi(int id)
{
	Heroi heroi;
	int n;

	n = alet(1, 3);

	heroi.id = id;
	heroi.experiencia = 0;
	heroi.paciencia = alet(0, 100);
	heroi.velocidade = alet(50, 5000);
	heroi.habilidades = cria_cjt(n);

	for (int i = 0; i < n; i++)
	{
		int habilidade = alet(0, N_HABILIDADES - 1);
		insere_cjt(heroi.habilidades, habilidade);
	}

	return heroi;
}

// Função para inicialiazar uma base com características aleatórias
Base cria_base(int id)
{
	Base local;
	Localizacao localizacao;
	int max;

	local.id = id;
	max = alet(3, 10);

	local.lotacao_maxima = max;
	local.presentes = cria_cjt(max);
	local.espera = cria_fila();
	localizacao.x = alet(0, N_TAMANHO_MUNDO);
	localizacao.y = alet(0, N_TAMANHO_MUNDO);
	local.localizacao = localizacao;

	return local;
}

// Função para criar o mundo inicial
Mundo cria_mundo()
{
	Mundo mundo;

	mundo.tempo_atual = T_INICIO;
	mundo.n_habilidades = N_HABILIDADES;
	mundo.n_herois = N_HEROIS;
	mundo.n_bases = N_BASES;
	mundo.n_missoes = N_MISSOES;
	mundo.n_tamanho_mundo = N_TAMANHO_MUNDO;

	for (int y = 0; y < N_HEROIS; y++)
		mundo.herois[y] = cria_heroi(y);

	for (int z = 0; z < N_BASES; z++)
		mundo.bases[z] = cria_base(z);

	return mundo;
}

// Função para criar uma missão
Missao cria_missao(int id)
{
	Missao missao;

	missao.id = id;
	int n = alet(6, 10);

	missao.habilidades_nec = cria_cjt(n);

	for (int i = 0; i < n; i++)
	{
		int habilidade = alet(0, N_HABILIDADES - 1);
		insere_cjt(missao.habilidades_nec, habilidade);
	}

	missao.localizacao.x = alet(0, N_TAMANHO_MUNDO);
	missao.localizacao.y = alet(0, N_TAMANHO_MUNDO);

	return missao;
}

// Função para destruir a estrutura do mundo
void destroi_mundo(Mundo *mundo)
{

	for (int i = 0; i < N_HEROIS; i++)
	{
		mundo->herois[i].habilidades = destroi_cjt(mundo->herois[i].habilidades);
	}

	for (int x = 0; x < N_BASES; x++)
	{
		mundo->bases[x].presentes = destroi_cjt(mundo->bases[x].presentes);
		mundo->bases[x].espera = destroi_fila(mundo->bases[x].espera);
	}
}