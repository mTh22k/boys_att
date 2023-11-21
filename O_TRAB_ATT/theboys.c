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
#define N_HEROIS (N_HABILIDADES*5)
#define N_BASES (N_HEROIS/6)
#define N_MISSOES (T_FIM_DO_MUNDO/100)

#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define FIM 4



typedef  struct heroi {
    int id;  
    int paciencia;   
    int velocidade;  
    int experiencia;  
    conjunto_t *habilidades;
} heroi_t;

typedef  struct localizacao {
    int x;  
    int y;   
} localizacao_t;

typedef  struct missao {
    int id;
	localizacao_t localizacao;    
    conjunto_t *habilidades_nec;
} missao_t;

typedef  struct local {
    int id;  
    int lotacao_maxima;   
    conjunto_t *presentes;  
    fila_t *espera;  
    localizacao_t localizacao;
} local_t;

typedef  struct mundo {
	int tempo_atual;
	heroi_t herois[N_HEROIS];
	local_t locais[N_BASES];
	missao_t missoes[N_MISSOES] ;
	conjunto_t *habilidades;
	int n_herois;
	int n_habilidades;
	int n_locais;
	int n_missoes;
	int n_tamanho_mundo;
} mundo_t;

int alet(int min, int max) {
	return min + (rand() % (max - min + 1));
}

heroi_t cria_heroi(int id, conjunto_t *hab) {
	heroi_t heroi;
	int n;
        
    n = alet(2,5);
     
	heroi.id = id;
	heroi.experiencia = 0;
	heroi.paciencia = alet(0, 100);
	heroi.velocidade = alet(50, 5000);
	heroi.habilidades = cria_cjt(n);

	int i;
    for (i = 0; i < n; i++) {
        int habilidade = alet(0, N_HABILIDADES - 1);
        insere_cjt(heroi.habilidades, habilidade);
    }

	
	return heroi;
}

local_t cria_local(int id) {
	local_t local;
	localizacao_t localizacao;
	int max;
	
    local.id = id;
	max = alet(3,10);
        
    local.lotacao_maxima = max;
    local.presentes = cria_cjt(max);
    local.espera = cria_fila();
    localizacao.x = alet(0, N_TAMANHO_MUNDO);
    localizacao.y = alet(0, N_TAMANHO_MUNDO);
    local.localizacao = localizacao;
        
    return local;
}


mundo_t cria_mundo() {
	int x, y, z;
	mundo_t mundo;
        	  
    mundo.tempo_atual= T_INICIO; 
	mundo.n_habilidades = N_HABILIDADES;
    mundo.habilidades = cria_cjt(mundo.n_habilidades);
    mundo.n_herois = N_HEROIS;
    mundo.n_locais = N_BASES;
	mundo.n_missoes = N_MISSOES;
    mundo.n_tamanho_mundo = N_TAMANHO_MUNDO;
        
    for(x = 0; x < mundo.n_habilidades; x++){
       	if(insere_cjt(mundo.habilidades, rand() % 10) == 0)
        	x--;
   	}
        
   	for(y = 0; y < N_HEROIS; y++)
        mundo.herois[y] = cria_heroi(y, mundo.habilidades);
        
   	for(z = 0; z < N_BASES; z++) 
        mundo.locais[z] = cria_local(z);
        	
	return mundo;
}

evento_t *cria_chegada(int id_heroi, int id_local, int tempo) {
	evento_t* ev;
	
	if(!(ev = (evento_t*) malloc(sizeof(evento_t)))) 
        	return NULL;
        
	ev->tempo = tempo;
    ev->tipo = CHEGADA;
    ev->dado1 = id_heroi;
    ev->dado2 = id_local;
        
    return ev;
}

missao_t cria_missao(int id, conjunto_t* habilidades) {
    missao_t missao;
    int n;

    missao.id = id;
    n = alet(6, 10);
    
    // Inicializa o conjunto de habilidades necessárias da missão
    missao.habilidades_nec = cria_cjt(n);
    
    // Preenche o conjunto de habilidades necessárias da missão
    int i;
    for (i = 0; i < n; i++) {
        int habilidade = alet(0, N_HABILIDADES - 1);
        insere_cjt(missao.habilidades_nec, habilidade);
    }

    missao.localizacao.x = alet(0, N_TAMANHO_MUNDO);
    missao.localizacao.y = alet(0, N_TAMANHO_MUNDO);

    return missao;
}


evento_t *cria_evento_missao(int id, int tempo){
	evento_t* ev;
	
	if(!(ev = (evento_t*) malloc(sizeof(evento_t)))) 
        	return NULL;
        
	ev->tempo = tempo;
    ev->tipo = MISSAO;
    ev->dado1 = id;
        
    return ev;
}

evento_t *cria_saida(int id_heroi, int id_local, int tempo) {
	evento_t* ev;
	
	if(!(ev = (evento_t*) malloc(sizeof(evento_t)))) 
        	return NULL;
        	
    ev->tempo = tempo;
    ev->tipo = SAIDA;
    ev->dado1 = id_heroi;
    ev->dado2 = id_local;
    
    return ev;
}

evento_t *cria_fim(){
	evento_t* ev;
	
	if(!(ev = (evento_t*) malloc(sizeof(evento_t)))) 
        return NULL;
        
    ev->tempo = T_FIM_DO_MUNDO;
	ev->tipo = FIM;
        
    return ev;
}



int distancia_entre_bases(local_t *inicial, local_t *final) {
	int xi, xf, yi, yf, resul;
	
	xi = inicial->localizacao.x;
	xf = final->localizacao.x;
	
	yi = inicial->localizacao.y;
	yf = final->localizacao.y;
	
	resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));
	
	return resul;
}


int distancia_entre_base_missao(localizacao_t *inicial, localizacao_t *final) {
	int xi, xf, yi, yf, resul;
	
	xi = inicial->x;
	xf = final->x;
	
	yi = inicial->y;
	yf = final->y;
	
	resul = sqrt(pow((xf - xi), 2) + pow((yf - yi), 2));
	
	return resul;
}

int calcula_velocidade(heroi_t* heroi) {
	int aux;
	
	aux = heroi->velocidade;
	
	
	
	return aux;
} 

int escolhe_equipe(local_t locais[N_BASES], heroi_t herois[N_HEROIS], missao_t* missao, int tempo_atual, lef_t* lef) {
    conjunto_t* destroi, *aux = cria_cjt(30*5);
    int x, y, escolhido = -1, id_heroi;

    /*LOOP PARA PERCORRER TODOS OS LOCAIS*/
    for(x = 0; x < N_BASES; x++) {

        /* LOOP PARA PERCORRER TODOS OS HEROIS DO LOCAL*/
        for(y = 0; y < locais[x].presentes->card; y++){
            id_heroi = locais[x].presentes->v[y];
            destroi = aux;
            aux = uniao_cjt(herois[id_heroi].habilidades, destroi);
            destroi_cjt(destroi);
        }

        printf("%6d:MISSAO %2d HAB_EQL %d:", tempo_atual, missao->id, locais[x].id);
        imprime_cjt(aux);

        /* Verifica se a equipe é apta para a missão */
        if(contido_cjt(missao->habilidades_nec, aux) == 1) {
            /* Calcula a distância entre a base e o local da missão */
           int distancia = distancia_entre_base_missao(&locais[x].localizacao, &missao->localizacao);


            /* Se a distância é menor que a mínima registrada, atualiza a escolha */
           if (escolhido == -1 || distancia < distancia_entre_base_missao(&locais[escolhido].localizacao, &missao->localizacao)) {

                escolhido = x;
            }
        }

        

        destroi_cjt(aux);
        aux = cria_cjt(30*5);
    } 
	
	destroi_cjt(aux);

	if (escolhido == -1) {
        printf("%6d:MISSAO %2d IMPOSSIVEL (adiada para %d)\n", tempo_atual, missao->id, tempo_atual + 24 * 60);
        adiciona_ordem_lef(lef, cria_evento_missao(missao->id, tempo_atual + 24 * 60));
    }
	
	return escolhido;
}

/*FUNÇÃO PARA ESCOLHER EQUIPE PARA MISSÃO*/
// int escolhe_equipe(local_t locais[N_BASES], heroi_t herois[N_HEROIS], missao_t* missao, int tempo_atual) {
// 	conjunto_t* destroi, *aux = cria_cjt(30*5);
// 	int x, y, escolhido = -1, id_heroi;
	
// 	/*LOOP PARA PERCORRER TODOS OS LOCAIS*/
// 	for(x = 0; x < N_BASES; x++) {
		
// 		/* LOOP PARA PERCORRER TODOS OS HEROIS DO LOCAL*/
// 		for(y = 0; y < locais[x].presentes->card; y++){
// 			id_heroi = locais[x].presentes->v[y];
// 			destroi = aux;
// 			aux = uniao_cjt(herois[id_heroi].habilidades, destroi);
// 			destroi_cjt(destroi);
// 		}
		
// 		// printf("%6d:MISSAO %2d HAB_EQL %d:", tempo_atual, missao->id, locais[x].id);
// 		// imprime_cjt(aux);
		
// 		if(contido_cjt(missao->habilidades_nec, aux) == 1) {
// 			if(escolhido == -1) 
// 				escolhido = x;
// 			else if(locais[x].presentes->card < locais[escolhido].presentes->card) 
// 				escolhido = x;
// 		}
		
// 		destroi_cjt(aux);
// 		aux = cria_cjt(30*5);
	
// 	}
	
// 	destroi_cjt(aux);
	
// 	return escolhido;
// }




void destroi_mundo(mundo_t *mundo) {
	int x;
	
	for(x = 0; x < N_HEROIS; x++) {
		mundo->herois[x].habilidades = destroi_cjt(mundo->herois[x].habilidades);
	}
	
	for(x = 0; x < N_BASES; x++) {
		mundo->locais[x].presentes = destroi_cjt(mundo->locais[x].presentes);
		mundo->locais[x].espera = destroi_fila(mundo->locais[x].espera);
	}
	
	mundo->habilidades = destroi_cjt(mundo->habilidades);
}

int main() {
	srand(time(NULL));

	mundo_t m = cria_mundo();
	int id_local, x, tempo_missao, tempo_missao_inicial, tpl, p_fila, id_local_dest, v, d, tdl, local_missao, id_heroi_equipe, removeu_fila = 0, missoes_geradas = 0, missoes_realizadas = 0, total_agendamentos = 0, vezes_agendada = 0;

	lef_t* lef = cria_lef();
	nodo_lef_t* aux, *tirado_no;

	evento_t* evento_atual = NULL;
	
	for(x = 0; x < N_HEROIS;x++) {
		id_local = alet(0,N_BASES-1);
		adiciona_ordem_lef(lef, cria_chegada(x, id_local, alet(0,4320)) );
	}
	
	for(x = 0; x < N_MISSOES; x++) {
		m.missoes[x] = cria_missao(x, m.habilidades);
		tempo_missao_inicial = alet(0,T_FIM_DO_MUNDO);

		adiciona_ordem_lef(lef, cria_evento_missao(x, tempo_missao_inicial));
	}
	
	adiciona_ordem_lef(lef, cria_fim());
	
	while(lef) {
		evento_atual = obtem_primeiro_lef(lef);
		m.tempo_atual = evento_atual->tempo;
			
		aux = lef->Primeiro->prox;	
		tirado_no = lef->Primeiro;		
		lef->Primeiro = aux;	
		free(tirado_no);
			
		switch(evento_atual->tipo){
			case CHEGADA:
				
				if(m.locais[evento_atual->dado2].presentes->card == m.locais[evento_atual->dado2].lotacao_maxima) {
					if(m.herois[evento_atual->dado1].paciencia/4 - tamanho_fila(m.locais[evento_atual->dado2].espera) > 0){	
				 		insere_fila (m.locais[evento_atual->dado2].espera, evento_atual->dado1);
				 		
				 		printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) ESPERA\n", 
				 			m.tempo_atual,
				 			m.herois[evento_atual->dado1].id,
				 			m.locais[evento_atual->dado2].id,
				 			m.locais[evento_atual->dado2].presentes->card,
				 			m.locais[evento_atual->dado2].presentes->max
				 		);

						printf("%6d:ESPERA HEROI %2d BASE %d (%2d) \n", 
				 			m.tempo_atual,
				 			m.herois[evento_atual->dado1].id,
				 			m.locais[evento_atual->dado2].id,
							tamanho_fila(m.locais[evento_atual->dado2].espera)
				 		);


					} else {
						adiciona_ordem_lef(
							lef, 
							cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual));
				
						printf("%6d:CHEGA HEROI %2d BASE %d (%2d / %2d) DESISTE\n", 
				 			m.tempo_atual,
				 			m.herois[evento_atual->dado1].id,
				 			m.locais[evento_atual->dado2].id,
				 			m.locais[evento_atual->dado2].presentes->card,
				 			m.locais[evento_atual->dado2].presentes->max
				 		);

							printf("%6d:DESIST HEROI %2d BASE %d \n", 
				 			m.tempo_atual,
				 			m.herois[evento_atual->dado1].id,
				 			m.locais[evento_atual->dado2].id
				 		);
					}			
				} else {
				
					insere_cjt (m.locais[evento_atual->dado2].presentes, evento_atual->dado1);
					tpl = m.herois[evento_atual->dado1].paciencia / 10 + alet(-2, 6);
						
					if(tpl < 1) 
						tpl = 1;
							
					adiciona_ordem_lef(
						lef,
						cria_saida(evento_atual->dado1, evento_atual->dado2, m.tempo_atual + tpl)
					);

					int horario_saida = m.tempo_atual + tpl;
					
					printf("%6d:ENTRA HEROI %2d BASE %d (%2d / %2d) SAI %d\n", 
				 			m.tempo_atual,
				 			m.herois[evento_atual->dado1].id,
				 			m.locais[evento_atual->dado2].id,
				 			m.locais[evento_atual->dado2].presentes->card,
				 			m.locais[evento_atual->dado2].presentes->max,
							horario_saida 
				 		);
					
				}
							
				break;
			case SAIDA:
				if(vazia_fila(m.locais[evento_atual->dado2].espera) == 0){
					retira_fila(m.locais[evento_atual->dado2].espera, &p_fila);
					insere_cjt (m.locais[evento_atual->dado2].presentes, p_fila);
					
					adiciona_inicio_lef(
						lef, 
						cria_chegada(p_fila, evento_atual->dado2, m.tempo_atual)
					);

					printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), FILA ", 
				 			m.tempo_atual,
				 			
				 			m.locais[evento_atual->dado2].id,
				 			m.locais[evento_atual->dado2].presentes->card,
				 			m.locais[evento_atual->dado2].presentes->max
				 	);
					
					imprime_fila(m.locais[evento_atual->dado2].espera);
					
					
					printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), ADMITE %2d\n", 
				 			m.tempo_atual,
				 			
				 			m.locais[evento_atual->dado2].id,
				 			m.locais[evento_atual->dado2].presentes->card,
				 			m.locais[evento_atual->dado2].presentes->max,
				 			p_fila
				 	);
				 	
				 	removeu_fila = 1;
				}
				
				id_local_dest = rand() % N_BASES ;
				v = calcula_velocidade(&m.herois[evento_atual->dado1]);
				d = distancia_entre_bases(&m.locais[evento_atual->dado2], &m.locais[id_local_dest]);
				tdl = d / v;
				
				//FALTA PRINTAR ABAIXO

				//Significado: no instante 46101 o herói 30 inicia uma viagem da base 2 à base 6, com distância 6922 m, velocidade 4763 m/min e chegada prevista no instante 46102.

				retira_cjt (m.locais[evento_atual->dado2].presentes, m.herois[evento_atual->dado1].id);
				adiciona_ordem_lef(
					lef, 
					cria_chegada(evento_atual->dado1, id_local_dest, m.tempo_atual + tdl / 15)
				);
				
				if(removeu_fila == 0) {

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
							m.tempo_atual + tdl
					);

				}

				
					
				removeu_fila = 0;
				break;
			case MISSAO:
				missoes_geradas++;
				printf("%6d:MISSAO %2d HAB_REQ ", m.tempo_atual, evento_atual->dado1);
				imprime_cjt(m.missoes[evento_atual->dado1].habilidades_nec);
				local_missao = escolhe_equipe(m.locais, m.herois, &m.missoes[evento_atual->dado1], m.tempo_atual,lef);
				
				if(local_missao > -1) {
					missoes_realizadas++;
					printf("%6d:MISSAO %2d CUMPRIDA BASE %d HEROIS: ", m.tempo_atual, evento_atual->dado1, local_missao);
					imprime_cjt(m.locais[local_missao].presentes);
	
					for(x = 0; x < m.locais[local_missao].presentes->card; x++) {
						id_heroi_equipe = m.locais[local_missao].presentes->v[x];
						m.herois[id_heroi_equipe].experiencia++;
					}
									
				} else {
					vezes_agendada++;
					tempo_missao = alet(m.tempo_atual,T_FIM_DO_MUNDO);
					/*VERIFICAR SE MISSÃO FOI CRIADA NO FIM DO MUNDO SE SIM A ADICIONA PARA ALÉM DO FIM DO MUNDO*/
					if(tempo_missao == T_FIM_DO_MUNDO) 
						tempo_missao = alet(tempo_missao, T_FIM_DO_MUNDO + 100);
					
					adiciona_ordem_lef(lef, cria_evento_missao(evento_atual->dado1, tempo_missao));
					printf("%6d:MISSAO %2d IMPOSSIVEL\n", m.tempo_atual, evento_atual->dado1);
					
				}
				total_agendamentos += vezes_agendada;		
				break;
			case FIM:
				printf("%d:FIM\n", m.tempo_atual);
				for(x = 0; x < N_HEROIS; x++){
					printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", m.herois[x].id, m.herois[x].paciencia,m.herois[x].velocidade ,m.herois[x].experiencia);
					imprime_cjt(m.herois[x].habilidades);
				}

				double porcentagem_cumpridas = ((double)missoes_realizadas / missoes_geradas) * 100;
				double media_agendamentos = ((double)total_agendamentos / missoes_geradas);

				printf("%d: %d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",m.tempo_atual, missoes_realizadas, missoes_geradas, porcentagem_cumpridas, media_agendamentos);
				
				destroi_mundo(&m);
				
				for(x = 0; x < N_MISSOES; x++) {
					destroi_cjt(m.missoes[x].habilidades_nec);
				}

				//FALTA PRINTAR O QUE TA ABAIXO

				// Foram cumpridas 5242 das 5256 missões geradas (99,73% de sucesso); cada missão foi agendada em média 2,09 vezes até ser cumprida.

				lef = destroi_lef(lef);
				break;
		}
		
		free(evento_atual);

	}
			
	return 0;
}
