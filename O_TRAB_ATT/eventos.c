#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "eventos.h"

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

void evento_espera(Mundo m, evento *evento_atual)
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

void evento_desiste(Mundo m, evento *evento_atual, lef *lef)
{
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

void evento_entra(Mundo m, evento *evento_atual, lef *lef)
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

void evento_chega(Mundo m, evento *evento_atual, lef *lef)
{
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
            evento_espera(m, evento_atual);
        }
        else
        {
            evento_desiste(m, evento_atual, lef);
        }
    }
    else
    {
        evento_entra(m, evento_atual, lef);
    }
}

void evento_missao(Mundo m, evento *evento_atual, lef *lef, int *missoes_realizadas, int *vezes_agendada)
{
    printf("%6d:MISSAO %2d HAB REQ ", m.tempo_atual, evento_atual->dado1);
    imprime_cjt(m.missoes[evento_atual->dado1].habilidades_nec);

    int local_missao = escolhe_equipe(m.bases, m.herois, &m.missoes[evento_atual->dado1], m.tempo_atual, lef);

    if (local_missao > -1)
    {
        // MISSÃO CUMPRIDA
        (*missoes_realizadas)++; // Incrementa o número de missões cumpridas

        printf("%6d:MISSAO %2d CUMPRIDA BASE %d HEROIS: ", m.tempo_atual, evento_atual->dado1, local_missao);
        imprime_cjt(m.bases[local_missao].presentes);

        // Incrementa a experiência dos heróis envolvidos na missão cumprida
        for (int x = 0; x < m.bases[local_missao].presentes->card; x++)
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

        (*vezes_agendada)++; // Incrementa o número de vezes que a missão foi agendada
    }
}

void evento_viaja(Mundo m, evento *evento_atual, lef *lef, int *removeu_fila)
{
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
    if (*removeu_fila == 0)
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

    *removeu_fila = 0;
}

void evento_avisa(Mundo m, evento *evento_atual, lef *lef, int *removeu_fila, int *p_fila)
{
    // Retira o herói da fila de espera e coloca na base
    retira_fila(m.bases[evento_atual->dado2].espera, p_fila);
    insere_cjt(m.bases[evento_atual->dado2].presentes, *p_fila);

    // Adiciona um evento de chegada na LEF para o herói que saiu da fila
    add_inicio_lef(
        lef,
        cria_chegada(*p_fila, evento_atual->dado2, m.tempo_atual));

    printf("%6d:AVISA PORTEIRO BASE %d (%2d / %2d), FILA ",
           m.tempo_atual,
           m.bases[evento_atual->dado2].id,
           m.bases[evento_atual->dado2].presentes->card,
           m.bases[evento_atual->dado2].presentes->max);

    imprime_fila(m.bases[evento_atual->dado2].espera);

    printf("%6d:AVISA PORTEIRO BASE %d ADMITE %2d\n",
           m.tempo_atual,
           m.bases[evento_atual->dado2].id,
           *p_fila);

    *removeu_fila = 1;
}

void evento_sai(Mundo m, evento *evento_atual, lef *lef)
{
    // EVENTO SAI
    int removeu_fila = 0, p_fila;
    // Verifica se há heróis na fila de espera
    if (vazia_fila(m.bases[evento_atual->dado2].espera) == 0)
    {
        // Passa o endereço de p_fila para a função evento_avisa
        evento_avisa(m, evento_atual, lef, &removeu_fila, &p_fila);
    }

    evento_viaja(m, evento_atual, lef, &removeu_fila);
}

void evento_fim(Mundo m, evento *evento_atual, lef *lef, int *missoes_realizadas, int *vezes_agendada, int missoes_geradas)
{
    // EVENTO FIM
    printf("%d:FIM\n", m.tempo_atual);

    for (int x = 0; x < N_HEROIS; x++)
    {
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", m.herois[x].id, m.herois[x].paciencia, m.herois[x].velocidade, m.herois[x].experiencia);
        imprime_cjt(m.herois[x].habilidades);
    }

    // Inicialização das variáveis
    double porcentagem_cumpridas = 0.0;
    double media_agendamentos = 0.0;

    // Verifica se missoes_geradas é diferente de zero para evitar divisão por zero
    if (missoes_geradas != 0)
    {
        // Cálculos corrigidos para usar double
        porcentagem_cumpridas = ((double)(*missoes_realizadas) / missoes_geradas) * 100;
        media_agendamentos = ((double)(*vezes_agendada) / missoes_geradas);
    }

    printf("%d: %d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", m.tempo_atual, *missoes_realizadas, missoes_geradas, porcentagem_cumpridas, media_agendamentos);
}
