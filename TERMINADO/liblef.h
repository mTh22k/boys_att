// Definição de um evento, que contém informações como tempo, tipo e dados associados.
typedef struct evento {
    int tempo; 
    int tipo;   
    int dado1;  
    int dado2;  
} evento;

// Definição de um nó da lista encadeada, que armazena um evento e aponta para o próximo nó.
typedef struct nodo_lef {
    evento *evento;
    struct nodo_lef *prox;
} nodo_lef;

// Definição da Lista de Eventos Futuros (LEF), que mantém um ponteiro para o primeiro nó.
typedef struct lef {
    nodo_lef *Primeiro;
} lef;

// Função para criar e inicializar uma LEF vazia.
lef *cria_lef();

// Função para destruir a LEF, liberando todos os recursos alocados.
lef *destroi_lef(lef *l);

// Função para adicionar um novo evento no início da LEF.
int add_inicio_lef(lef *l, evento *evento);

// Função para adicionar um novo evento na LEF mantendo a ordem crescente de tempo.
int add_ordem_lef(lef *l, evento *evento);

// Função para obter o primeiro evento da LEF, sem removê-lo.
evento *obtem_primeiro_lef(lef *l);
