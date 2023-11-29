#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "eventos.h"

#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define FIM 4

int main()
{
	srand(time(NULL));

	Mundo m = cria_mundo();
	int x, missoes_geradas = N_MISSOES, missoes_realizadas = 0, vezes_agendada = 0;

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

			evento_chega(m, evento_atual, lef);

			break;
		case SAIDA:

			evento_sai(m, evento_atual, lef);

			break;
		case MISSAO:

			evento_missao(m, evento_atual, lef, &missoes_realizadas, &vezes_agendada);

			break;
		case FIM:

			evento_fim(m, evento_atual, lef, &missoes_realizadas, &vezes_agendada, missoes_geradas);

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
