#include "Arvore_B.h"

// Função para inicializar a árvore B
void inicializa(Ponteiro* Arvore) {
    *Arvore = NULL; // Define o ponteiro raiz da árvore B como NULL, indicando uma árvore vazia
}

// Função para inserir um registro em uma página da árvore B
void insere_pagina(Ponteiro Pagina, reg Registro, Ponteiro Irmão_Direita) {
    short posicão_nao_encontrada;
    int quantia_aux;
    quantia_aux = Pagina->quantia;

    // Verifica se a página possui algum registro
    if (quantia_aux > 0) {
        posicão_nao_encontrada = TRUE; // Registros existem, define a flag para encontrar a posição correta
    }
    else {
        posicão_nao_encontrada = FALSE; // Nenhum registro, insere no início da página
    }

    // Procura pela posição correta para inserir o registro
    while (posicão_nao_encontrada == TRUE) {
        // Verifica se o registro deve ser inserido no final da página
        if (Registro >= Pagina->reg[quantia_aux-1]) {
            posicão_nao_encontrada = FALSE; // Registro encontrou sua posição, sai do loop
            break;
        }

        // Desloca os registros e ponteiros para a direita para abrir espaço para o novo registro
        Pagina->reg[quantia_aux] = Pagina->reg[quantia_aux-1];
        Pagina->descendente[quantia_aux+1] = Pagina->descendente[quantia_aux];
        quantia_aux--;

        // Verifica se chegou ao final da página
        if (quantia_aux < 1) {
            posicão_nao_encontrada = FALSE; // Fim da página, sai do loop
        }
        else {
            posicão_nao_encontrada = TRUE; // Continua procurando a posição correta
        }
    }

    // Insere o registro e atualiza o ponteiro para o irmão à direita
    Pagina->reg[quantia_aux] = Registro;
    Pagina->descendente[quantia_aux+1] = Irmão_Direita;

    // Incrementa o número de registros na página
    Pagina->quantia++;
}

// Função para inserir um registro na árvore B
void insere_primitiva(reg Registro, Ponteiro Pagina, short* Cresceu, reg* Registro_Retorno, Ponteiro* Pagina_Retorno) {
    long j;
    long index = 1;
    Ponteiro temp_pagina;

    // Verifica se a página atual (nó) é NULL
    if (Pagina == NULL) {
        *Cresceu = TRUE; // Árvore está vazia, define a flag para inserir o registro
        (*Registro_Retorno) = Registro; // Define o registro a ser inserido
        (*Pagina_Retorno) = NULL; // A página pai (nó) não é afetada
        return;
    }

    // Procura pela posição correta para inserir o registro na página atual
    while (index < Pagina->quantia && Registro > Pagina->reg[index-1]) {
        index ++;
    }

    // Verifica se o registro a ser inserido já está presente na página
    if (Registro == Pagina->reg[index - 1]) {
        printf("ERRO: Registro já existe na árvore\n");
        *Cresceu = FALSE; // O registro já está na árvore, define a flag para indicar que nenhuma inserção é necessária
        return;
    }

    // Se o registro é menor que o registro atual, ajusta o índice
    if (Registro < Pagina->reg[index-1]) {
        index --;
    }

    // Chama recursivamente a função insere_primitiva para encontrar a página correta para inserir o registro
    insere_primitiva(Registro, Pagina->descendente[index], Cresceu, Registro_Retorno, Pagina_Retorno);

    // Se a flag de adição é falsa, significa que o registro já está na árvore, então nenhuma inserção é necessária
    if (!*Cresceu) {
        return;
    }

    // Verifica se há espaço suficiente na página atual para inserir o registro
    if (Pagina->quantia < MM) {
        insere_pagina(Pagina, *Registro_Retorno, *Pagina_Retorno); // Insere o registro na página atual
        *Cresceu = FALSE; // Define a flag de adição como falsa, pois o registro foi inserido
        return;
    }

    // Se não há espaço suficiente na página atual, cria uma nova página e realiza a divisão
    temp_pagina = (Ponteiro)malloc(sizeof(Tipo_Pagina));
    temp_pagina->quantia = 0;
    temp_pagina->descendente[0] = NULL;

    // Realiza a divisão com base no índice de inserção
    if (index < M + 1) {
        insere_pagina(temp_pagina, Pagina->reg[MM-1], Pagina->descendente[MM]); // Insere o registro na página temporária
        Pagina->quantia --;
        insere_pagina(Pagina, *Registro_Retorno, *Pagina_Retorno); // Insere o novo registro na página atual
    }

    else {
        insere_pagina(temp_pagina, *Registro_Retorno, *Pagina_Retorno); // Insere o registro na página temporária
    }

    // Move os registros e ponteiros restantes para a página temporária
    for (j = M + 2; j <= MM; j++) {
        insere_pagina(temp_pagina, Pagina->reg[j-1], Pagina->descendente[j]);
    }

    Pagina->quantia = M; // Atualiza o número de registros na página atual
    temp_pagina->descendente[0] = Pagina->descendente[M+1]; // Atualiza o ponteiro para o irmão à direita da página temporária
    *Registro_Retorno = Pagina->reg[M]; // Retorna o registro do meio da página atual
    *Pagina_Retorno = temp_pagina; // Retorna a página temporária como o novo irmão à direita
}

// Função para inserir um registro na árvore B
void insere(reg Registro, Ponteiro* Pagina) {
    short Cresceu;
    reg registro_retorno;
    Tipo_Pagina* pagina_retorno, *temp_pagina;

    // Chama a função insertion_primal para encontrar a posição correta para o registro
    insere_primitiva(Registro, *Pagina, &Cresceu, &registro_retorno, &pagina_retorno);

    // Se a flag de adição é verdadeira, significa que o registro precisa ser inserido na árvore
    if (Cresceu) {
        // Cria uma nova página para armazenar o registro e os ponteiros
        temp_pagina = (Tipo_Pagina*)malloc(sizeof(Tipo_Pagina));
        temp_pagina->quantia = 1;
        temp_pagina->reg[0] = registro_retorno;
        temp_pagina->descendente[1] = pagina_retorno;
        temp_pagina->descendente[0] = *Pagina;

        // Define a nova página como a raiz da árvore
        *Pagina = temp_pagina;
    }
}

// Função para pesquisar um registro na árvore B
void pesquisa(reg Reg, Ponteiro Pagina) {
    long index = 1;

    // Verifica se a página atual (nó) é NULL ou se a árvore B está vazia
    if (Pagina == NULL) {
        printf("ERRO: Página vazia ou registro não encontrado\n"); // Página está vazia ou o registro não foi encontrado
        return;
    }

    // Procura a posição correta do registro na página atual
    while (index < Pagina->quantia && Reg > Pagina->reg[index - 1])
        index++;

    // Verifica se o registro é encontrado na página atual
    if (Reg == Pagina->reg[index-1]) {
        Reg = Pagina->reg[index-1]; // Atualiza o parâmetro passado com o registro encontrado
        printf("O registro está na árvore\n"); // Registro encontrado, imprime a mensagem
        return;
    }

    // Se o registro é menor que o registro atual, pesquisa na subárvore esquerda
    if (Reg < Pagina->reg[index-1])
        pesquisa(Reg, Pagina->descendente[index-1]);
    else // Se o registro é maior que o registro atual, pesquisa na subárvore direita
        pesquisa(Reg, Pagina->descendente[index]);
}

// Função para reconstruir a árvore B após a remoção de um registro
void organiza(Ponteiro Pagina, Ponteiro Pagina_Pai, int Pos_Pai, short* Diminuiu) {
    Tipo_Pagina* Aux;
    long DispAux, j;

    // Verifica se há um irmão à direita da página atual
    if (Pos_Pai < Pagina_Pai->quantia) {
        Aux = Pagina_Pai->descendente[Pos_Pai + 1];
        DispAux = (Aux->quantia - M + 1) / 2;

        // Move o separador (chave) da página pai para a página atual
        Pagina->reg[Pagina->quantia] = Pagina_Pai->reg[Pos_Pai];
        Pagina->descendente[Pagina->quantia + 1] = Aux->descendente[0];
        Pagina->quantia++;

        // Verifica se há espaço suficiente na página atual para os registros do irmão
        if (DispAux > 0) {
            // Transfere os registros do irmão para a página atual
            for (j = 1; j < DispAux; j++) {
                insere_pagina(Pagina, Aux->reg[j - 1], Aux->descendente[j]);
            }
            // Atualiza o separador na página pai
            Pagina_Pai->reg[Pos_Pai] = Aux->reg[DispAux - 1];
            // Ajusta os registros e ponteiros do irmão
            Aux->quantia -= DispAux;
            for (j = 0; j < Aux->quantia; j++) {
                Aux->reg[j] = Aux->reg[j + DispAux];
            }
            for (j = 0; j <= Aux->quantia; j++) {
                Aux->descendente[j] = Aux->descendente[j + DispAux];
            }
            *Diminuiu = FALSE; // Não é necessário subtrair da página pai
        } else {
            // Não há espaço suficiente na página atual para os registros do irmão
            // Transfere alguns registros do irmão para a página atual
            for (j = 1; j <= M; j++) {
                insere_pagina(Pagina, Aux->reg[j - 1], Aux->descendente[j]);
            }
            free(Aux); // Libera o espaço do irmão
            // Ajusta a página pai após a remoção do separador
            for (j = Pos_Pai + 1; j < Pagina_Pai->quantia; j++) {
                Pagina_Pai->reg[j - 1] = Pagina_Pai->reg[j];
                Pagina_Pai->descendente[j] = Pagina_Pai->descendente[j + 1];
            }

            Pagina_Pai->quantia--;
            if (Pagina_Pai->quantia >= M) {
                *Diminuiu = FALSE; // Não é necessário subtrair da página pai
            }
        }
    } else {
        // A página atual não possui irmão à direita, então verifica o irmão à esquerda
        Aux = Pagina_Pai->descendente[Pos_Pai - 1];
        DispAux = (Aux->quantia - M + 1) / 2;
        
        // Move o separador (chave) da página pai para a página atual
        for (j = Pagina->quantia; j >= 1; j--) {
            Pagina->reg[j] = Pagina->reg[j - 1];
        }
        Pagina->reg[0] = Pagina_Pai->reg[Pos_Pai - 1];
        for (j = Pagina->quantia; j >= 0; j--) {
            Pagina->descendente[j + 1] = Pagina->descendente[j];
        }
        Pagina->quantia++;

        // Verifica se há espaço suficiente na página atual para os registros do irmão
        if (DispAux > 0) {
            // Transfere os registros do irmão para a página atual
            for (j = 1; j < DispAux; j++) {
                insere_pagina(Pagina, Aux->reg[Aux->quantia - j], Aux->descendente[Aux->quantia - j + 1]);
            }
            Pagina->descendente[0] = Aux->descendente[Aux->quantia - DispAux + 1];
            Pagina_Pai->reg[Pos_Pai - 1] = Aux->reg[Aux->quantia - DispAux];
            Aux->quantia -= DispAux;
            *Diminuiu = FALSE; // Não é necessário subtrair da página pai
        } else {
            // Não há espaço suficiente na página atual para os registros do irmão
            // Transfere alguns registros da página atual para o irmão
            for (j = 1; j <= M; j++) {
                insere_pagina(Aux, Pagina->reg[j - 1], Pagina->descendente[j]);
            }
            free(Pagina); // Libera o espaço da página atual
            Pagina_Pai->quantia--;
            if (Pagina_Pai->quantia >= M) {
                *Diminuiu = FALSE; // Não é necessário subtrair da página pai
            }
        }
    }
}

// Função para encontrar o antecessor de um registro na árvore B
void antecessor(Ponteiro Pagina, int Index, Ponteiro Pagina_Pai, short* Diminuiu) {
    // Verifica se há um filho mais à direita na página pai (Pagina_Pai)
    if (Pagina_Pai->descendente[Pagina_Pai->quantia] != NULL) {
        // Encontra recursivamente o antecessor no filho mais à direita
        antecessor(Pagina, Index, Pagina_Pai->descendente[Pagina_Pai->quantia], Diminuiu);
        
        // Verifica se o filho mais à direita requer rebalanceamento
        if (*Diminuiu == TRUE) {
            // Rebalanceia o filho mais à direita e atualiza o sinalizador 'Diminuiu'
            organiza(Pagina_Pai->descendente[Pagina_Pai->quantia], Pagina_Pai, Pagina_Pai->quantia, Diminuiu);
            return;
        }
    }

    // Define a chave do antecessor na página atual (Pagina) a partir da página pai (Pagina_Pai)
    Pagina->reg[Index - 1] = Pagina_Pai->reg[Pagina_Pai->quantia - 1];
    Pagina_Pai->quantia--;
    *Diminuiu = (Pagina_Pai->quantia < M);
}

// Função para remover um registro da árvore B (remoção primal)
static void retira_primitiva(reg Delete, Ponteiro* Pagina, short* Diminuiu) {
    long j;
    long Index = 1;
    Ponteiro Pag;

    // Verifica se a página atual está vazia (NULL)
    if (*Pagina == NULL) {
        printf("ERRO: Este registro não está na árvore\n");
        return;
    }

    // Atribui a página atual à variável Pag
    Pag = *Pagina;

    // Encontra o índice do registro a ser removido na página atual
    while (Index < Pag->quantia && Delete > Pag->reg[Index - 1])
        Index++;

    // Verifica se o registro a ser removido está na página atual
    if (Delete == Pag->reg[Index - 1]) {
        // Verifica se o registro a ser removido está em um nó folha
        if (Pag->descendente[Index - 1] == NULL) {
            // Remove o registro da página atual
            Pag->quantia--;
            if (Pag->quantia < M) {
                *Diminuiu = TRUE;
            } else {
                *Diminuiu = FALSE;
            }

            // Desloca os registros para preencher o espaço deixado pelo registro removido
            for (j = Index; j <= Pag->quantia; j++) {
                Pag->reg[j - 1] = Pag->reg[j];
                Pag->reg[j] = Pag->reg[j + 1];
            }
            return;
        }

        // Se o registro a ser removido não está em um nó folha,
        // encontre o antecessor do registro no filho da página atual
        antecessor(*Pagina, Index, Pag->descendente[Index - 1], Diminuiu);

        // Verifica se o filho requer rebalanceamento
        if (*Diminuiu)
            organiza(Pag->descendente[Index - 1], *Pagina, Index - 1, Diminuiu);
        return;
    }

    // Se o registro a ser removido não está na página atual,
    // verifique no filho apropriado da página atual
    if (Delete > Pag->reg[Index - 1])
        Index++;

    // Chama recursivamente a função remove_primal no filho apropriado
    retira_primitiva(Delete, &Pag->descendente[Index - 1], Diminuiu);

    // Verifica se o filho requer rebalanceamento
    if (*Diminuiu)
        organiza(Pag->descendente[Index - 1], *Pagina, Index - 1, Diminuiu);
}

// Função para remover um registro da árvore B (remoção final)
void retira(reg Delete, Ponteiro* Pagina) {
    short Diminuiu;
    Ponteiro aux;

    // Chama a função remove_primal para realizar a remoção primal do registro
    retira_primitiva(Delete, Pagina, &Diminuiu);

    // Verifica se a flag Subtract é TRUE e a página atual está vazia
    if (Diminuiu == TRUE && (*Pagina)->quantia == 0) {
        // Se a página atual estiver vazia após a remoção, ela é removida da árvore
        aux = *Pagina;
        *Pagina = aux->descendente[0];
        free(aux);
    }
}

// Função para testar as propriedades primais da árvore B
static void teste_primitivo(Ponteiro Arvore, int Chave, short Irmão_Direita) {
    int index;
    int antecessor = 0;

    // Verifica se o nó atual da árvore está vazio (NULL)
    if (Arvore == NULL)
        return;

    // Verifica se a chave do filho mais à esquerda é maior que a chave do pai (Irmão_Direita == FALSE)
    if (Arvore->reg[0] > Chave && Irmão_Direita == FALSE) {
        printf("ERRO: Filho %12d é maior que o pai %d\n", Arvore->reg[0], Chave);
        return;
    }

    // Verifica se cada chave no nó atual é maior que a chave do irmão à esquerda
    for (index = 0; index < Arvore->quantia; index++) {
        if (Arvore->reg[index] <= antecessor) {
            printf("ERRO: Irmão %ld é maior que o irmão à esquerda %d\n", (long)Arvore->reg[index], antecessor);
            return;
        }
        antecessor = Arvore->reg[index];
    }

    // Chama recursivamente a função test_primal em cada nó filho do nó atual (exceto o filho mais à direita)
    for (index = 0; index < Arvore->quantia; index++) {
        teste_primitivo(Arvore->descendente[index], Arvore->reg[index], FALSE);
    }

    // Chama recursivamente a função test_primal no nó filho mais à direita do nó atual
    teste_primitivo(Arvore->descendente[Arvore->quantia], Arvore->reg[index], TRUE);
}

// Função para testar toda a árvore B quanto às propriedades estruturais
void teste(Ponteiro Arvore) {
    int i;

    // Verifica se o nó atual da árvore está vazio (NULL)
    if (Arvore == NULL)
        return;

    // Chama recursivamente a função test_primal em cada nó filho do nó atual (exceto o filho mais à direita)
    for (i = 0; i < Arvore->quantia; i++)
        teste_primitivo(Arvore->descendente[i], Arvore->reg[i], FALSE);

    // Chama recursivamente a função test_primal no nó filho mais à direita do nó atual
    teste_primitivo(Arvore->descendente[Arvore->quantia], Arvore->reg[Arvore->quantia - 1], TRUE);
}

// Função para imprimir a árvore B em uma ordem primal
void imprime_primitivo(Ponteiro Arvore, int Altura) {
    long index;

    // Verifica se o nó atual da árvore está vazio (NULL)
    if (Arvore == NULL) {
        return;
    }

    // Imprime o nível (altura) do nó atual da árvore
    printf("Altura da árvore %d: ", Altura);

    // Imprime as chaves (registros) do nó atual da árvore
    for (index = 0; index < Arvore->quantia; index++) {
        printf("%d ", Arvore->reg[index]);
    }
    putchar('\n');

    // Incrementa a altura para o próximo nível (nós filhos)
    Altura++;

    // Chama recursivamente a função print_primal em cada nó filho do nó atual (incluindo o filho mais à direita)
    for (index = 0; index <= Arvore->quantia; index++) {
        imprime_primitivo(Arvore->descendente[index], Altura);
    }
}

// Função de encapsulamento para imprimir toda a árvore B
void imprime(Ponteiro Arvore) {
    int index = 0; // Inicializa a altura (nível) como 0
    imprime_primitivo(Arvore, index); // Chama a função print_primal para imprimir a árvore B
}

// Função recursiva para liberar a memória alocada para toda a árvore B
void libera(Ponteiro Arvore) {
    int i;

    // Caso base: se o nó atual estiver vazio (NULL), retorne
    if (Arvore == NULL) {
        return;
    }

    // Libera recursivamente a memória para cada nó filho do nó atual
    // chamando a função free_all em cada filho
    for (i = 0; i <= Arvore->quantia; i++) {
        libera(Arvore->descendente[i]);
    }

    // Libera a memória alocada para o nó atual
    free(Arvore);
}
