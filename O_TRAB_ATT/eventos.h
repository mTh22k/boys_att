#include "entidades.h"

// Função para criar um evento de chegada de herói
evento *cria_chegada(int id_heroi, int id_local, int tempo);

// Função para criar um evento de início de missão
evento *cria_evento_missao(int id, int tempo);

// Função para criar um evento de saída de herói
evento *cria_saida(int id_heroi, int id_local, int tempo);

// Função para criar um evento de fim do mundo
evento *cria_fim();

// Função para calcular a distância entre duas bases
int distancia_entre_bases(Base *inicial, Base *final);

// Função para calcular a distância entre a base e a localização da missão
int distancia_entre_base_missao(Localizacao *inicial, Localizacao *final);

// Função para escolher uma equipe de heróis para uma missão
int escolhe_equipe(Base bases[N_BASES], Heroi herois[N_HEROIS], Missao *missao, int tempo_atual, lef *lef);

// Função que simula o evento de um herói chegar a uma base
// Inclui a inserção do herói na fila de espera e exibe informações
void evento_espera(Mundo m, evento *evento_atual);

// Função que simula o evento de um herói desistir de uma missão
// Inclui a criação de um evento de saída na lista de eventos futuros
void evento_desiste(Mundo m, evento *evento_atual, lef *lef);

// Função que simula o evento de um herói entrar em uma base
// Inclui a inserção do herói na base e a criação de um evento de saída
void evento_entra(Mundo m, evento *evento_atual, lef *lef);

// Função que simula o evento de um herói chegar a uma base para uma missão
// Inclui a verificação de lotação da base e a chamada de outras funções conforme necessário
void evento_chega(Mundo m, evento *evento_atual, lef *lef);

// Função que simula o evento de uma missão ocorrer
// Inclui a escolha de uma equipe para realizar a missão e o tratamento de sucesso ou falha
void evento_missao(Mundo m, evento *evento_atual, lef *lef, int *missoes_realizadas, int *vezes_agendada);

// Função que simula o evento de um herói viajar entre bases
// Inclui o cálculo da distância, remoção da base de origem e criação de um evento de chegada
void evento_viaja(Mundo m, evento *evento_atual, lef *lef, int *removeu_fila);

// Função que simula o evento de um herói ser avisado para entrar em uma base
// Inclui a remoção do herói da fila de espera e a exibição de informações
void evento_avisa(Mundo m, evento *evento_atual, lef *lef, int *removeu_fila, int *p_fila);

// Função que simula o evento de um herói sair de uma base e viajar
// Inclui a remoção do herói da base e a criação de um evento de chegada em outra base
void evento_sai(Mundo m, evento *evento_atual, lef *lef);

// Função que simula o evento de fim do mundo
// Inclui a exibição de estatísticas finais e a destruição do mundo e da lista de eventos futuros
void evento_fim(Mundo m, evento *evento_atual, lef *lef, int *missoes_realizadas, int *vezes_agendada, int missoes_geradas);