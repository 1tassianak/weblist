#ifndef DDLL_PRI_H
#define DDLL_PRI_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Define a estrutura de um nó na lista duplamente encadeada (DDLLNode)
struct DDLLNode {
    void *data;           // Ponteiro para os dados armazenados no nó
    struct DDLLNode *prev; // Ponteiro para o nó anterior na lista
    struct DDLLNode *next; // Ponteiro para o próximo nó na lista
};

// Define a estrutura principal da lista duplamente encadeada (DDLL)
struct DDLL {
    struct DDLLNode *head;       // Cabeça da lista, apontando para o primeiro nó
    struct DDLLNode *tail;       // Cauda da lista, apontando para o último nó
    int size;             // Tamanho de cada elemento (usado para alocação de memória)
    int count;            // Número de elementos atualmente na lista
};

#endif