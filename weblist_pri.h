#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"
#include "DDLL_pub.h"

// Estrutura do nó da WebList
typedef struct weblist_node {
    int key;
    pDDLL list; // Lista associada
} WebListNode;

// Estrutura da WebList
struct weblist {
    WebListNode *nodes; // Array de nós
    int level; // Nível da árvore
    int node_count; // Número de nós folha
};

// Funções auxiliares
int balanceWebList(pweblist web);

#endif
