#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"

// Estrutura do nó da WebList
typedef struct weblist_node {
    int key; // Chave identificadora para o nó, usada para diferenciar cada nó
    pDDLL list; // Ponteiro para uma DDLL associada ao nó, que armazenará os dados
} WebListNode;

// Estrutura da WebList
struct weblist {
    WebListNode *nodes; // Array de nós da WebList; cada nó possui uma chave e uma DDLL associadas
    int level; // Nível da árvore da WebList (pode ser usado para controle de profundidade)
    int node_count; //Contagem de nós-folha na WebList, indicando quantos nós estão alocados
    int sizedata; // Tamanho dos dados a serem armazenados em cada nó da DDLL
};

#endif
