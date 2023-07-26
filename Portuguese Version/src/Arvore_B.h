// Constantes
#define TRUE 1
#define FALSE 0
#define M 2 // Número mínimo de chaves em um nó
#define MM (M * 2) // Número máximo de chaves em um nó

// Estrutura de dados que representa um nó da árvore B
typedef int reg; // Supondo que as chaves são do tipo inteiro

typedef struct Pagina* Ponteiro; // Define um novo tipo 'Ponteiro' como um pseudônimo para um ponteiro para struct 'Pagina'
typedef struct Pagina {
    short quantia; // Número de chaves no nó
    reg reg[MM]; // Array para armazenar as chaves no nó
    Ponteiro descendente[MM + 1]; // Array de ponteiros para os nós filhos
} Tipo_Pagina;




// Funções de inserção
void insere(reg Registro, Ponteiro *Pagina);

// Funções de remoção
void organiza(Ponteiro Pagina, Ponteiro Pagina_Pai, int Pos_Pai, short* Diminuiu);

void antecessor(Ponteiro Pagina, int Index, Ponteiro Pagina_Pai, short* Diminuiu);

void retira(reg Delete, Ponteiro* Pagina);

// Funções auxiliares
void inicializa(Ponteiro* Arvore);

void pesquisa(reg Reg, Ponteiro Pagina);

void teste(Ponteiro Arvore);

void imprime(Ponteiro Arvore);

void libera(Ponteiro Arvore);
