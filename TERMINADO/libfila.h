// Definição de um nó da fila, que contém um elemento e um ponteiro para o próximo nó.
struct nodo_fila {
    int elem;
    struct nodo_fila *prox;
};

// Tipo definido para simplificar a declaração de variáveis do tipo nodo_fila.
typedef struct nodo_fila nodo_fila_f;

// Definição da estrutura fila, que mantém ponteiros para o início e o fim da fila, bem como o tamanho da fila.
struct fila {
    nodo_fila_f *ini;
    nodo_fila_f *fim;
    int tamanho; 
};

// Tipo definido para simplificar a declaração de variáveis do tipo fila.
typedef struct fila fila;

// Função para criar e inicializar uma fila vazia.
fila *cria_fila();

// Função para destruir a fila, liberando todos os recursos alocados.
fila *destroi_fila(fila *f);

// Função para verificar se a fila está vazia.
int vazia_fila(fila *f);

// Função para obter o tamanho atual da fila.
int tamanho_fila(fila *f);

// Função para inserir um elemento no final da fila.
int insere_fila(fila *f, int elemento);

// Função para retirar um elemento do início da fila, armazenando-o na variável passada por referência.
int retira_fila(fila *f, int *elemento);

// Função para imprimir os elementos da fila.
void imprime_fila(fila *f);
