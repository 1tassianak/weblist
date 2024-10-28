// main.c
#include <stdio.h>
#include "main.h"
#include "weblist_pub.h"

int main() {

    //ponteiro para a estrutura weblist
    pweblist web;
    int resultado;

    //cria a WebList
    resultado = cWL(&web);
    //verifica o retorno da criação
    if (resultado == SUCCESS) {
        printf("WebList criada com sucesso.\n");
    } else {
        printf("Falha ao criar WebList.\n");
        return FAIL;
    }

    //inserção de dados
    //inicializa um dado para ser inserido
    int dado = 10;
    //chama a função de inserção
    resultado = iDado(web, &dado);
    //verifica o retorno da inserção
    if (resultado == SUCCESS) {
        printf("Dado inserido com sucesso.\n");
    } else {
        printf("Falha ao inserir dado.\n");
    }

    //verifica se a weblist está balanceada
    resultado = WLbalanceada(web);
    if (resultado == SUCCESS) {
        printf("WebList está balanceada.\n");
    } else {
        printf("WebList não está balanceada.\n");
    }

    //destrói a weblist
    resultado = dWL(&web);
    if (resultado == SUCCESS) {
        printf("WebList destruída com sucesso.\n");
    } else {
        printf("Falha ao destruir WebList.\n");
    }

    return 0;
}
