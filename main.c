#include "main.h"

int printWebList(pweblist web) {
    if (!web) return FAIL;
    for (int i = 0; i < web->totalLeaves; i++) {
        printf("Chave %d: ", web->nodes[i].key);
        pDDLL lista = web->nodes[i].list;
        if (!lista) {
            printf("Lista vazia\n");
            continue;
        }
        Node *aux = lista->head;
        while (aux != NULL) {
            printf("%d ", *(int *)aux->data);
            aux = aux->next;
        }
        printf("\n");
    }
    return SUCCESS;
}

void loadData(const char *filename, pweblist web) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int dado;
    while (fscanf(file, "%d", &dado) != EOF) {
        iDado(web, &dado);
    }

    fclose(file);
}

int main() {
    pweblist web;
    int level = 2;
    int sizedata = sizeof(int);

    // Criar a WebList
    if (cWL(&web, level, sizedata) == FAIL) {
        return EXIT_FAILURE;
    }

    // Inserir o conjunto de dados {3, 2, 5, 4, 1}
    int dados_iniciais[] = {3, 2, 5, 4, 1};
    for (int i = 0; i < 5; i++) {
        iDado(web, &dados_iniciais[i]);
    }

    // Remover os dados 5, 2, 3 e 4
    int dados_para_remover[] = {5, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        rDado(web, &dados_para_remover[i]);
    }

    // Inserir o dado 15
    int dado_para_inserir = 15;
    iDado(web, &dado_para_inserir);

    // Imprimir apenas os dados restantes na WebList
    for (int i = 0; i < web->totalLeaves; i++) {
        pDDLL lista = web->nodes[i].list;
        Node *aux = lista->head;
        while (aux != NULL) {
            printf("%d ", *(int *)aux->data);
            aux = aux->next;
        }
    }
    printf("\n");

    // Destruir a WebList
    dWL(&web);
    
    return EXIT_SUCCESS;
}