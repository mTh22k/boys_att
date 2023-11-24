#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define FIM 4

typedef struct heroi
{
	int id;
	int paciencia;
	int velocidade;
	int experiencia;
	conjunto_t *habilidades;
} heroi_t;

typedef struct localizacao
{
	int x;
	int y;
} localizacao_t;

typedef struct missao
{
	int id;
	localizacao_t localizacao;
	conjunto_t *habilidades_nec;
} missao_t;

typedef struct local
{
	int id;
	int lotacao_maxima;
	conjunto_t *presentes;
	fila_t *espera;
	localizacao_t localizacao;
} local_t;

typedef struct mundo
{
	int tempo_atual;
	heroi_t herois[N_HEROIS];
	local_t locais[N_BASES];
	missao_t missoes[N_MISSOES];
	int n_herois;
	int n_habilidades;
	int n_locais;
	int n_missoes;
	int n_tamanho_mundo;
} mundo_t;

int alet(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

heroi_t cria_heroi(int id)
{
	heroi_t heroi;
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

local_t cria_local(int id)
{
	local_t local;
	localizacao_t localizacao;
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

mundo_t cria_mundo()
{
	mundo_t mundo;

	mundo.tempo_atual = T_INICIO;
	mundo.n_habilidades = N_HABILIDADES;
	mundo.n_herois = N_HEROIS;
	mundo.n_locais = N_BASES;
	mundo.n_missoes = N_MISSOES;
	mundo.n_tamanho_mundo = N_TAMANHO_MUNDO;

	for (int y = 0; y < N_HEROIS; y++)
		mundo.herois[y] = cria_heroi(y);

	for (int z = 0; z < N_BASES; z++)
		mundo.locais[z] = cria_local(z);

	return mundo;
}

evento_t *cria_chegada(int id_heroi, int id_local, int tempo)
{
	evento_t *ev;

	if (!(ev = (evento_t *)malloc(sizeof(evento_t))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = CHEGADA;
	ev->dado1 = id_heroi;
	ev->dado2 = id_local;

	return ev;
}

missao_t cria_missao(int id)
{
	missao_t missao;

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

evento_t *cria_evento_missao(int id, int tempo)
{
	evento_t *ev;

	if (!(ev = (evento_t *)malloc(sizeof(evento_t))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = MISSAO;
	ev->dado1 = id;

	return ev;
}

evento_t *cria_saida(int id_heroi, int id_local, int tempo)
{
	evento_t *ev;

	if (!(ev = (evento_t *)malloc(sizeof(evento_t))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = SAIDA;
	ev->dado1 = id_heroi;
	ev->dado2 = id_local;

	return ev;
}

evento_t *cria_fim()
{
	evento_t *ev;

	if (!(ev = (evento_t *)malloc(sizeof(evento_t))))
		return NULL;

	ev->tempo = T_FIM_DO_MUNDO;
	ev->tipo = FIM;

	return ev;
}

int distancia_entre_bases(local_t *inicial, local_t *final)
{

	int xi = inicial->localizacao.x;
	int xf = final->localizacao.x;

	int yi = inicial->localizacao.y;
	int yf = final->localizacao.y;

	int resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));

	return resul;
}

int distancia_entre_base_missao(localizacao_t *inicial, localizacao_t *final)
{

	int xi = inicial->x;
	int xf = final->x;

	int yi = inicial->y;
	int yf = final->y;

	int resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));

	return resul;
}

// int calcula_velocidade(heroi_t *heroi)
// {

// 	int aux = heroi->velocidade;

// 	return aux;
// }

int escolhe_equipe(local_t locais[N_BASES], heroi_t herois[N_HEROIS], missao_t *missao, int tempo_atual, lef_t *lef)
{
	conjunto_t *destroi, *aux = cria_cjt(30 * 5);
	int escolhido = -1, id_heroi;

	/*LOOP PARA PERCORRER TODOS OS LOCAIS*/
	for (int x = 0; x < N_BASES; x++)
	{

		/* LOOP PARA PERCORRER TODOS OS HEROIS DO LOCAL*/
		for (int y = 0; y < locais[x].presentes->card; y++)
		{
			id_heroi = locais[x].presentes->v[y];
			destroi = aux;
			aux = uniao_cjt(herois[id_heroi].habilidades, destroi);
			destroi_cjt(destroi);
		}

		printf("%6d:MISSAO %2d HAB_BASE %d:", tempo_atual, missao->id, locais[x].id);
		imprime_cjt(aux);

		/* Verifica se a equipe é apta para a missão */
		if (contido_cjt(missao->habilidades_nec, aux) == 1)
		{
			/* Calcula a distância entre a base e o local da missão */
			int distancia = distancia_entre_base_missao(&locais[x].localizacao, &missao->localizacao);

			/* Se a distância é menor que a mínima registrada, atualiza a escolha */
			if (escolhido == -1 || distancia < distancia_entre_base_missao(&locais[escolhido].localizacao, &missao->localizacao))
			{

				escolhido = x;
			}
		}

		destroi_cjt(aux);
		aux = cria_cjt(30 * 5);
	}

	destroi_cjt(aux);

	return escolhido;
}

void destroi_mundo(mundo_t *mundo)
{

	for (int i = 0; i < N_HEROIS; i++)
	{
		mundo->herois[i].habilidades = destroi_cjt(mundo->herois[i].habilidades);
	}

	for (int x = 0; x < N_BASES; x++)
	{
		mundo->locais[x].presentes = destroi_cjt(mundo->locais[x].presentes);
		mundo->locais[x].espera = destroi_fila(mundo->locais[x].espera);
	}
}

int main()
{
	srand(time(NULL));

	mundo_t m = cria_mundo();
	int x, p_fila, removeu_fila = 0, missoes_geradas = N_MISSOES, missoes_realizadas = 0, vezes_agendada = 0;

	lef_t *lef = cria_lef();
	nodo_lef_t *aux, *tirado_no;

	evento_t *evento_atual = NULL;

	for (x = 0; x < N_HEROIS; x++)
	{
		int id_local = alet(0, N_BASES - 1);
		adiciona_ordem_lef(lef, cria_chegada(x, id_local, alet(0, 4320)));
	}

	for (x = 0; x < N_MISSOES; x++)
	{
		m.missoes[x] = cria_missao(x);
		int tempo_missao_inicial = alet(0, T_FIM_DO_MUNDO);

		adiciona_ordem_lef(lef, cria_evento_missao(x, tempo_missao_inicial));
	}

	adiciona_ordem_lef(lef, cria_fim());

	while (lef)
	{
		evento_atual = obtem_primeiro_lef(lef);
		m.tempo_atual = evento_atual->tempo;

		aux = lef->Primeiro->prox;
		tirado_no = lef->Primeiro;
		lef->Primeiro = aux;
		free(tirado_no);

		switch (evento_atual->tipo)
		{
		case CHEGADA:

			// EVENTO CHEGA
			if (m.locais[evento_atual->dado2].presentes->card == m.locais[evento_atual->dado2].lotacao_maxima)
			{
				int espera;
				if (vazia_fila(m.locais[evento_atual->dado2].espera) || (m.herois[evento_atual->dado1].paciencia > 10 * tamanho_fila(m.locais[evento_atual->dado2].espera)))
				{
					espera = 1; // true
				}
				else
				{
					espera = 0; // false
				}

				if (espera)
				{
					// EVENTO ESPERA
					insere_fila(m.locais[evento_atual->dado2].espera, evento_atual->dado1);

					printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) ESPERA\n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.locais[evento_atual->dado2].id,
						   m.locais[evento_atual->dado2].presentes->card,
						   m.locais[evento_atual->dado2].presentes->max);

					printf("%6d:ESPERA HEROI %2d BASE %d (%2d) \n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.locais[evento_atual->dado2].id,
						   tamanho_fila(m.locais[evento_atual->dado2].espera));
				}
				else
				{
					// EVENTO DESISTE
					adiciona_ordem_lef(lef, cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual));

					printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) DESISTE\n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.locais[evento_atual->dado2].id,
						   m.locais[evento_atual->dado2].presentes->card,
						   m.locais[evento_atual->dado2].presentes->max);

					printf("%6d:DESIST HEROI %2d BASE %d \n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.locais[evento_atual->dado2].id);
				}
			}
			else
			{
				// EVENTO ENTRA
				insere_cjt(m.locais[evento_atual->dado2].presentes, evento_atual->dado1);

				// Calcula o tempo de permanência na base (TPB)
				int TPB = 15 + m.herois[evento_atual->dado1].paciencia * alet(1, 20);

				// Cria e insere na LEF o evento SAI
				adiciona_ordem_lef(lef, cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual + TPB));

				printf("%6d:ENTRA HEROI %2d BASE %d (%2d / %2d) SAI %d\n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   m.locais[evento_atual->dado2].id,
					   m.locais[evento_atual->dado2].presentes->card,
					   m.locais[evento_atual->dado2].presentes->max,
					   m.tempo_atual + TPB);
			}
			break;
		case SAIDA:
			// EVENTO SAI
			if (vazia_fila(m.locais[evento_atual->dado2].espera) == 0)
			{
				retira_fila(m.locais[evento_atual->dado2].espera, &p_fila);
				insere_cjt(m.locais[evento_atual->dado2].presentes, p_fila);

				adiciona_inicio_lef(
					lef,
					cria_chegada(p_fila, evento_atual->dado2, m.tempo_atual));

				printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), FILA ",
					   m.tempo_atual,

					   m.locais[evento_atual->dado2].id,
					   m.locais[evento_atual->dado2].presentes->card,
					   m.locais[evento_atual->dado2].presentes->max);

				imprime_fila(m.locais[evento_atual->dado2].espera);

				printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), ADMITE %2d\n",
					   m.tempo_atual,

					   m.locais[evento_atual->dado2].id,
					   m.locais[evento_atual->dado2].presentes->card,
					   m.locais[evento_atual->dado2].presentes->max,
					   p_fila);

				removeu_fila = 1;
			}

			// EVENTO VIAJA
			int id_local_dest = rand() % N_BASES;
			int v = m.herois[evento_atual->dado1].velocidade;
			int d = distancia_entre_bases(&m.locais[evento_atual->dado2], &m.locais[id_local_dest]);
			int tdl = d / v;

			retira_cjt(m.locais[evento_atual->dado2].presentes, m.herois[evento_atual->dado1].id);
			adiciona_ordem_lef(
				lef,
				cria_chegada(evento_atual->dado1, id_local_dest, m.tempo_atual + tdl / 15));

			if (removeu_fila == 0)
			{

				printf("%6d:SAI HEROI %2d BASE %d (%2d / %2d) \n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   m.locais[evento_atual->dado2].id,
					   m.locais[evento_atual->dado2].presentes->card,
					   m.locais[evento_atual->dado2].presentes->max);

				printf("%6d:VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   id_local_dest,
					   m.locais[evento_atual->dado2].id,
					   d,
					   m.herois[evento_atual->dado1].velocidade,
					   m.tempo_atual + tdl);
			}

			removeu_fila = 0;
			break;
		case MISSAO:
			printf("%6d:MISSAO %2d HAB_REQ ", m.tempo_atual, evento_atual->dado1);
			imprime_cjt(m.missoes[evento_atual->dado1].habilidades_nec);
			int local_missao = escolhe_equipe(m.locais, m.herois, &m.missoes[evento_atual->dado1], m.tempo_atual, lef);

			if (local_missao > -1)
			{
				missoes_realizadas++;
				printf("%6d:MISSAO %2d CUMPRIDA BASE %d HEROIS: ", m.tempo_atual, evento_atual->dado1, local_missao);
				imprime_cjt(m.locais[local_missao].presentes);

				for (x = 0; x < m.locais[local_missao].presentes->card; x++)
				{
					int id_heroi_equipe = m.locais[local_missao].presentes->v[x];
					m.herois[id_heroi_equipe].experiencia++;
				}
			}
			else
			{

				printf("%6d:MISSAO %2d IMPOSSIVEL (adiada para %d)\n", m.tempo_atual, evento_atual->dado1, m.tempo_atual + 24 * 60);
				adiciona_ordem_lef(lef, cria_evento_missao(evento_atual->dado1, m.tempo_atual + 24 * 60));

				vezes_agendada++;
			}

			break;
		case FIM:
			printf("%d:FIM\n", m.tempo_atual);
			printf("%d", vezes_agendada);
			for (x = 0; x < N_HEROIS; x++)
			{
				printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", m.herois[x].id, m.herois[x].paciencia, m.herois[x].velocidade, m.herois[x].experiencia);
				imprime_cjt(m.herois[x].habilidades);
			}

			double porcentagem_cumpridas = ((double)missoes_realizadas / missoes_geradas) * 100;
			double media_agendamentos = ((double)vezes_agendada / missoes_geradas);

			printf("%d: %d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", m.tempo_atual, missoes_realizadas, missoes_geradas, porcentagem_cumpridas, media_agendamentos);

			destroi_mundo(&m);

			for (x = 0; x < N_MISSOES; x++)
			{
				destroi_cjt(m.missoes[x].habilidades_nec);
			}

			lef = destroi_lef(lef);
			break;
		}

		free(evento_atual);
	}

	return 0;
}
