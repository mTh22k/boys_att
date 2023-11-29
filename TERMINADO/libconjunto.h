// Definição da estrutura conjunto, que armazena informações como capacidade máxima, cardinalidade, ponteiro e vetor de elementos.
struct conjunto {
    int max;   
    int card;   
    int ptr;    
    int *v;     
};

// Tipo definido para simplificar a declaração de variáveis do tipo conjunto.
typedef struct conjunto conjunto;

// Função para criar e inicializar um conjunto com capacidade máxima especificada.
conjunto *cria_cjt(int max);

// Função para destruir o conjunto, liberando todos os recursos alocados.
conjunto *destroi_cjt(conjunto *c);

// Função para verificar se o conjunto está vazio.
int vazio_cjt(conjunto *c);

// Função para obter a cardinalidade atual do conjunto.
int cardinalidade_cjt(conjunto *c);

// Função para inserir um elemento no conjunto.
int insere_cjt(conjunto *c, int elemento);

// Função para retirar um elemento do conjunto.
int retira_cjt(conjunto *c, int elemento);

// Função para verificar se um elemento pertence ao conjunto.
int pertence_cjt(conjunto *c, int elemento);

// Função para verificar se um conjunto está contido em outro.
int contido_cjt(conjunto *c1, conjunto *c2);

// Função para verificar se dois conjuntos são iguais.
int sao_iguais_cjt(conjunto *c1, conjunto *c2);

// Função para calcular a diferença entre dois conjuntos.
conjunto *diferenca_cjt(conjunto *c1, conjunto *c2);

// Função para calcular a interseção entre dois conjuntos.
conjunto *interseccao_cjt(conjunto *c1, conjunto *c2);

// Função para calcular a união entre dois conjuntos.
conjunto *uniao_cjt(conjunto *c1, conjunto *c2);

// Função para imprimir os elementos do conjunto.
void imprime_cjt(conjunto *c);
