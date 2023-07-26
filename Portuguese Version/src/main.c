#include <stdio.h>
#include <stdlib.h>
#include "Arvore_B.c"

int main() {
    reg x; // Declara uma variável 'x' do tipo 'reg' (registro da Árvore B)
    Tipo_Pagina* Arvore; // Declara um ponteiro 'Arvore' do tipo 'Tipo_Pagina' que aponta para a raiz da Árvore B
    short ctrl = TRUE; // Declara e inicializa uma variável 'ctrl' do tipo 'short' com valor 'TRUE' (1) para controlar o loop

    inicializa(&Arvore); // Inicializa a Árvore B, definindo a raiz como NULL

    // Laço principal da aplicação
    do {
        // Exibe as opções do menu
        printf("----------//----------\n");
        printf("== Selecione uma opção ==\n");
        printf("1. Inserir um registro\n");
        printf("2. Remover um registro\n");
        printf("3. Pesquisar um registro\n");
        printf("4. Imprimir toda a árvore\n");
        printf("5. Fechar a aplicação\n");
        printf("----------//----------\n");
        printf("Escolha: ");
        int escolha;
        scanf("%d", &escolha);
        fflush(stdin);
        printf("\n");
        
        switch (escolha) {
        case 1:
            // Inserção de chaves
            printf("Digite as chaves de entrada. Digite '-1' para parar a inserção de registros\n");
            printf("Registro: ");
            scanf("%d%*[^\n]", &x);
            getchar();
            while (x != -1) { 
                insere(x, &Arvore); // Insere a chave 'x' na Árvore B
                imprime(Arvore); // Imprime a árvore após a inserção
                printf("\nRegistro: ");
                scanf("%d%*[^\n]", &x);
                getchar();
            }
            teste(Arvore); // Verifica a consistência da Árvore B após as inserções
            break;

        case 2:
            // Remoção de chaves
            printf("Digite as chaves de saída. Digite '-1' para parar a remoção de registros\n");
            printf("Registro: ");
            scanf("%d%*[^\n]", &x);
            getchar();
            while (x != -1) { 
                retira(x, &Arvore); // Remove a chave 'x' da Árvore B
                imprime(Arvore); // Imprime a árvore após a remoção
                printf("Registro: ");
                scanf("%d%*[^\n]", &x);
                getchar();
            }
            teste(Arvore); // Verifica a consistência da Árvore B após as remoções
            break;

        case 3:
            // Pesquisa de uma chave
            int procura;
            printf("Pesquisar um registro\n");
            printf("Registro: ");
            scanf("%d%*[^\n]", &procura);
            pesquisa(procura, Arvore); // Pesquisa a chave 'procura' na Árvore B
            break;

        case 4:
            // Impressão da árvore inteira
            imprime(Arvore); // Imprime toda a árvore
            break;

        case 5:
            // Fechando a aplicação
            imprime(Arvore); // Imprime a árvore antes de liberar a memória
            libera(Arvore); // Libera a memória alocada para a Árvore B
            printf("Saindo...\n");
            ctrl = FALSE; // Define 'ctrl' como FALSE (0) para sair do loop
            break;

        default:
            // Escolha inválida
            printf("Escolha inválida, tente novamente\n\n");
            continue; // Retorna ao início do loop
        }

    } while (ctrl); // Repete o loop enquanto 'ctrl' for TRUE (1)

    return 0;
}
