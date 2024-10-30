#include "main.h"

int main() {
    pweblist web; // Ponteiro para a WebList
    int level = 0; // Defina o nível da estrutura
    int sizedata = sizeof(int); // Tamanho dos dados a serem armazenados

    // Criar a WebList
    if (cWL(&web, level, sizedata) == FAIL) {
        fprintf(stderr, "Falha ao criar a WebList\n");
        return EXIT_FAILURE;
    }

    // Conjunto inicial de dados para inserir na WebList
    int dados_iniciais[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
    int n_dados = sizeof(dados_iniciais) / sizeof(dados_iniciais[0]);

    // Inserir dados iniciais
    printf("Inserindo dados iniciais:\n");
    for (int i = 0; i < n_dados; i++) {
        if (iDado(web, &dados_iniciais[i]) == FAIL) {
            fprintf(stderr, "Falha ao inserir dado %d\n", dados_iniciais[i]);
        } else {
            printf("Inserido: %d\n", dados_iniciais[i]);
        }
    }

    // Imprimir a WebList após as inserções iniciais
    printf("Estrutura da WebList após inserções iniciais:\n");
    pLista(web);

    // Operações de remoção complexas para testar o balanceamento
    int dados_remocao[] = {1,2,3,4};
    int n_remocoes = sizeof(dados_remocao) / sizeof(dados_remocao[0]);

    printf("Removendo dados:\n");
    for (int i = 0; i < n_remocoes; i++) {
        if (rDado(web, &dados_remocao[i]) == FAIL) {
            printf("Dado %d não encontrado para remoção\n", dados_remocao[i]);
        } else {
            printf("Removido: %d\n", dados_remocao[i]);
        }
    }

    // Imprimir a WebList após as remoções
    printf("Estrutura da WebList após remoções:\n");
    pLista(web);

    // Inserir mais dados para ver como a estrutura se reequilibra
    int novos_dados[] = {1,2,3,4};
    int n_novos_dados = sizeof(novos_dados) / sizeof(novos_dados[0]);

    printf("Inserindo novos dados:\n");
    for (int i = 0; i < n_novos_dados; i++) {
        if (iDado(web, &novos_dados[i]) == FAIL) {
            fprintf(stderr, "Falha ao inserir dado %d\n", novos_dados[i]);
        } else {
            printf("Inserido: %d\n", novos_dados[i]);
        }
    }

    // Imprimir a WebList após novas inserções
    printf("Estrutura da WebList após novas inserções:\n");
    pLista(web);

    // Destruir a WebList para liberar memória
    if (dWL(&web) == FAIL) {
        fprintf(stderr, "Falha ao destruir a WebList\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}