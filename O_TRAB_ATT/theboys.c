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

// DEFINICAO DAS STRUCTS

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
int alet(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

// Função para inicializar um herói com características aleatórias
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

// Função para criar um evento de chegada de herói
evento *cria_chegada(int id_heroi, int id_local, int tempo)
{
	evento *ev;

	if (!(ev = (evento *)malloc(sizeof(evento))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = CHEGADA;
	ev->dado1 = id_heroi;
	ev->dado2 = id_local;

	return ev;
}

// Função para criar um evento de início de missão
evento *cria_evento_missao(int id, int tempo)
{
	evento *ev;

	if (!(ev = (evento *)malloc(sizeof(evento))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = MISSAO;
	ev->dado1 = id;

	return ev;
}

// Função para criar um evento de saída de herói
evento *cria_saida(int id_heroi, int id_local, int tempo)
{
	evento *ev;

	if (!(ev = (evento *)malloc(sizeof(evento))))
		return NULL;

	ev->tempo = tempo;
	ev->tipo = SAIDA;
	ev->dado1 = id_heroi;
	ev->dado2 = id_local;

	return ev;
}

// Função para criar um evento de fim do mundo
evento *cria_fim()
{
	evento *ev;

	if (!(ev = (evento *)malloc(sizeof(evento))))
		return NULL;

	ev->tempo = T_FIM_DO_MUNDO;
	ev->tipo = FIM;

	return ev;
}

// Função para calcular a distância entre duas bases
int distancia_entre_bases(Base *inicial, Base *final)
{

	int xi = inicial->localizacao.x;
	int xf = final->localizacao.x;

	int yi = inicial->localizacao.y;
	int yf = final->localizacao.y;

	int resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));

	return resul;
}

// Função para calcular a distância entre a base e a localização da missão
int distancia_entre_base_missao(Localizacao *inicial, Localizacao *final)
{

	int xi = inicial->x;
	int xf = final->x;

	int yi = inicial->y;
	int yf = final->y;

	int resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));

	return resul;
}

// Função para escolher uma equipe de heróis para uma missão
int escolhe_equipe(Base bases[N_BASES], Heroi herois[N_HEROIS], Missao *missao, int tempo_atual, lef *lef)
{
	conjunto *destroi, *aux = cria_cjt(30 * 5);
	int escolhido = -1, id_heroi;

	// Itera sobre todas as bases para escolher a melhor equipe
	for (int x = 0; x < N_BASES; x++)
	{

		// Combina as habilidades dos heróis presentes na base
		for (int y = 0; y < bases[x].presentes->card; y++)
		{
			id_heroi = bases[x].presentes->v[y];
			destroi = aux;
			aux = uniao_cjt(herois[id_heroi].habilidades, destroi);
			destroi_cjt(destroi);
		}

		printf("%6d:MISSAO %2d HAB BASE %d:", tempo_atual, missao->id, bases[x].id);
		imprime_cjt(aux);

		// Verifica se a equipe é apta para a missão 
		if (contido_cjt(missao->habilidades_nec, aux) == 1)
		{
			// Calcula a distância entre a base e o local da missão
			int distancia = distancia_entre_base_missao(&bases[x].localizacao, &missao->localizacao);

			// Se a distância é menor que a mínima registrada, atualiza a escolha
			if (escolhido == -1 || distancia < distancia_entre_base_missao(&bases[escolhido].localizacao, &missao->localizacao))
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

int main()
{
	srand(time(NULL));

	Mundo m = cria_mundo();
	int x, p_fila, removeu_fila = 0, missoes_geradas = N_MISSOES, missoes_realizadas = 0, vezes_agendada = 0;

	// Criação da lista de eventos futuros
	lef *lef = cria_lef();
	nodo_lef *aux, *tirado_no;

	evento *evento_atual = NULL;

	// Preenchimento inicial da lista de eventos com chegadas de heróis
	for (x = 0; x < N_HEROIS; x++)
	{
		int id_local = alet(0, N_BASES - 1);
		add_ordem_lef(lef, cria_chegada(x, id_local, alet(0, 4320)));
	}

	// Preenchimento inicial da lista de eventos com missões
	for (x = 0; x < N_MISSOES; x++)
	{
		m.missoes[x] = cria_missao(x);
		int tempo_missao_inicial = alet(0, T_FIM_DO_MUNDO);

		add_ordem_lef(lef, cria_evento_missao(x, tempo_missao_inicial));
	}

	// Adição de evento de fim do mundo à lista de eventos
	add_ordem_lef(lef, cria_fim());

	while (lef)
	{
		// Obtenção do próximo evento da lista de eventos futuros
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
			if (m.bases[evento_atual->dado2].presentes->card == m.bases[evento_atual->dado2].lotacao_maxima)
			{
				int espera;

				// Verifica se a base está cheia
				if (vazia_fila(m.bases[evento_atual->dado2].espera) || (m.herois[evento_atual->dado1].paciencia > 10 * tamanho_fila(m.bases[evento_atual->dado2].espera)))
				{
					espera = 1;
				}
				else
				{
					espera = 0; 
				}

				if (espera)
				{
					// EVENTO ESPERA
					insere_fila(m.bases[evento_atual->dado2].espera, evento_atual->dado1);

					printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) ESPERA\n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.bases[evento_atual->dado2].id,
						   m.bases[evento_atual->dado2].presentes->card,
						   m.bases[evento_atual->dado2].presentes->max);

					printf("%6d:ESPERA HEROI %2d BASE %d (%2d) \n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.bases[evento_atual->dado2].id,
						   tamanho_fila(m.bases[evento_atual->dado2].espera));
				}
				else
				{
					// EVENTO DESISTE
					add_ordem_lef(lef, cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual));

					printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) DESISTE\n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.bases[evento_atual->dado2].id,
						   m.bases[evento_atual->dado2].presentes->card,
						   m.bases[evento_atual->dado2].presentes->max);

					printf("%6d:DESIST HEROI %2d BASE %d \n",
						   m.tempo_atual,
						   m.herois[evento_atual->dado1].id,
						   m.bases[evento_atual->dado2].id);
				}
			}
			else
			{
				// EVENTO ENTRA
				insere_cjt(m.bases[evento_atual->dado2].presentes, evento_atual->dado1);

				// Calcula o tempo de permanência na base 
				int TPB = 15 + m.herois[evento_atual->dado1].paciencia * alet(1, 20);

				// Cria e insere na LEF o evento SAI
				add_ordem_lef(lef, cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual + TPB));

				printf("%6d:ENTRA HEROI %2d BASE %d (%2d / %2d) SAI %d\n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   m.bases[evento_atual->dado2].id,
					   m.bases[evento_atual->dado2].presentes->card,
					   m.bases[evento_atual->dado2].presentes->max,
					   m.tempo_atual + TPB);
			}
			break;
		case SAIDA:
			// EVENTO SAI

			// Verifica se há heróis na fila de espera
			if (vazia_fila(m.bases[evento_atual->dado2].espera) == 0)
			{
				// Retira o herói da fila de espera e coloca na base
				retira_fila(m.bases[evento_atual->dado2].espera, &p_fila);
				insere_cjt(m.bases[evento_atual->dado2].presentes, p_fila);

				 // Adiciona um evento de chegada na LEF para o herói que saiu da fila
				add_inicio_lef(
					lef,
					cria_chegada(p_fila, evento_atual->dado2, m.tempo_atual));

				printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), FILA ",
					   m.tempo_atual,

					   m.bases[evento_atual->dado2].id,
					   m.bases[evento_atual->dado2].presentes->card,
					   m.bases[evento_atual->dado2].presentes->max);

				imprime_fila(m.bases[evento_atual->dado2].espera);

				printf("%6d:AVISA PORTEIRO BASE %d ADMITE %2d\n",
					   m.tempo_atual,

					   m.bases[evento_atual->dado2].id,
					   p_fila);

				removeu_fila = 1;
			}

			// EVENTO VIAJA
			int id_local_dest = rand() % N_BASES;
			int v = m.herois[evento_atual->dado1].velocidade;
			int d = distancia_entre_bases(&m.bases[evento_atual->dado2], &m.bases[id_local_dest]);
			int tdl = d / v;

			// Remove o herói da base de origem
			retira_cjt(m.bases[evento_atual->dado2].presentes, m.herois[evento_atual->dado1].id);

			// Adiciona um evento de chegada na LEF para o herói que viajou
			add_ordem_lef(
				lef,
				cria_chegada(evento_atual->dado1, id_local_dest, m.tempo_atual + tdl / 15));

			// Se não removeu da fila, imprime eventos de saída e viagem
			if (removeu_fila == 0)
			{

				printf("%6d:SAI HEROI %2d BASE %d (%2d / %2d) \n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   m.bases[evento_atual->dado2].id,
					   m.bases[evento_atual->dado2].presentes->card,
					   m.bases[evento_atual->dado2].presentes->max);

				printf("%6d:VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
					   m.tempo_atual,
					   m.herois[evento_atual->dado1].id,
					   id_local_dest,
					   m.bases[evento_atual->dado2].id,
					   d,
					   m.herois[evento_atual->dado1].velocidade,
					   m.tempo_atual + tdl);
			}

			removeu_fila = 0;
			break;
		case MISSAO:
			printf("%6d:MISSAO %2d HAB REQ ", m.tempo_atual, evento_atual->dado1);
			imprime_cjt(m.missoes[evento_atual->dado1].habilidades_nec);

			// Escolhe uma equipe para realizar a missão
			int local_missao = escolhe_equipe(m.bases, m.herois, &m.missoes[evento_atual->dado1], m.tempo_atual, lef);

			if (local_missao > -1)
			{
				// MISSÃO CUMPRIDA

				missoes_realizadas++;
				printf("%6d:MISSAO %2d CUMPRIDA BASE %d HEROIS: ", m.tempo_atual, evento_atual->dado1, local_missao);
				imprime_cjt(m.bases[local_missao].presentes);

				// Incrementa a experiência dos heróis envolvidos na missão cumprida
				for (x = 0; x < m.bases[local_missao].presentes->card; x++)
				{
					int id_heroi_equipe = m.bases[local_missao].presentes->v[x];
					m.herois[id_heroi_equipe].experiencia++;
				}
			}
			else
			{

				 // Imprime informações sobre a impossibilidade da missão e agenda um novo evento de missão
				printf("%6d:MISSAO %2d IMPOSSIVEL\n", m.tempo_atual, evento_atual->dado1);
				add_ordem_lef(lef, cria_evento_missao(evento_atual->dado1, m.tempo_atual + 24 * 60));

				vezes_agendada++;
			}

			break;
		case FIM:

			// EVENTO FIM
			printf("%d:FIM\n", m.tempo_atual);

			for (x = 0; x < N_HEROIS; x++)
			{
				printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", m.herois[x].id, m.herois[x].paciencia, m.herois[x].velocidade, m.herois[x].experiencia);
				imprime_cjt(m.herois[x].habilidades);
			}

			// Calcula estatísticas finais
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
