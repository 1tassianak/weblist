#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"

// Estrutura do nó da WebList
typedef struct weblist_list {
    int key;         // Chave da lista
    pDDLL list;     // Lista associada
} WebListList;

// Estrutura da WebList
typedef struct weblist_node {
    struct weblist_node *children[8]; // 8 filhos
    int is_leaf;                       // Flag para indicar se é um nó folha
    WebListList lists[8];             // Apenas para nós folha: 8 listas associadas
} WebListNode;

// Estrutura da WebList
struct weblist {
    WebListNode *nodes; // Array de nós
    int level;          // Nível da árvore
    int node_count;     // Número de nós folha
    int sizedata;       // Tamanho dos dados
};

#endif
