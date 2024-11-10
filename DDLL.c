#include "DDLL_pub.h"
#include <stdlib.h>
#include <string.h>

// Função para criar a DDLL
int cDDLL(ppDDLL pp, int sizedata) {
    *pp = (pDDLL)malloc(sizeof(struct DDLL));
    if (!(*pp)) return FAIL;

    (*pp)->head = NULL;
    (*pp)->tail = NULL;
    (*pp)->size = sizedata;
    (*pp)->count = 0;
    return SUCCESS;
}

// Função para destruir a DDLL
int dDDLL(ppDDLL pp) {
    if (!(*pp)) return FAIL;
    cleanDDLL(*pp);
    free(*pp);
    *pp = NULL;
    return SUCCESS;
}

// Função para limpar a DDLL (remover todos os elementos)
int cleanDDLL(pDDLL p) {
    if (!p) return FAIL;

    struct DDLLNode *current = p->head;
    while (current) {
        struct DDLLNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    p->head = p->tail = NULL;
    p->count = 0;
    return SUCCESS;
}

// Função para buscar o primeiro elemento (sBegin)
int sBegin(pDDLL p, void *element) {
    if (!p || !p->head) return FAIL;

    memcpy(element, p->head->data, p->size);
    return SUCCESS;
}

// Função para buscar o elemento na posição N (sPosition)
int sPosition(pDDLL p, int N, void *element) {
    if (!p || N < 0 || N >= p->count) return FAIL;

    struct DDLLNode *current = p->head;
    for (int i = 0; i < N; i++) {
        if (!current) return FAIL;
        current = current->next;
    }

    if (current) {
        memcpy(element, current->data, p->size);
        return SUCCESS;
    }
    return FAIL;
}

// Função para inserir um elemento no início
int iBegin(pDDLL p, void *element) {
    if (!p) return FAIL;

    struct DDLLNode *newNode = (struct DDLLNode *)malloc(sizeof(struct DDLLNode));
    if (!newNode) return FAIL;

    newNode->data = malloc(p->size);
    if (!newNode->data) {
        free(newNode);
        return FAIL;
    }
    memcpy(newNode->data, element, p->size);
    newNode->prev = NULL;
    newNode->next = p->head;

    if (p->head) {
        p->head->prev = newNode;
    } else {
        p->tail = newNode;
    }
    p->head = newNode;
    p->count++;
    return SUCCESS;
}

// Função para inserir um elemento no final
int iEnd(pDDLL p, void *element) {
    if (!p) return FAIL;

    struct DDLLNode *newNode = (struct DDLLNode *)malloc(sizeof(struct DDLLNode));
    if (!newNode) return FAIL;

    newNode->data = malloc(p->size);
    if (!newNode->data) {
        free(newNode);
        return FAIL;
    }
    memcpy(newNode->data, element, p->size);
    newNode->next = NULL;
    newNode->prev = p->tail;

    if (p->tail) {
        p->tail->next = newNode;
    } else {
        p->head = newNode;
    }
    p->tail = newNode;
    p->count++;
    return SUCCESS;
}

// Função para inserir um elemento em uma posição específica
int iPosition(pDDLL p, int N, void *element) {
    if (!p || N < 0 || N > p->count) return FAIL;

    if (N == 0) return iBegin(p, element);
    if (N == p->count) return iEnd(p, element);

    struct DDLLNode *current = p->head;
    for (int i = 0; i < N; i++) {
        current = current->next;
    }

    struct DDLLNode *newNode = (struct DDLLNode *)malloc(sizeof(struct DDLLNode));
    if (!newNode) return FAIL;

    newNode->data = malloc(p->size);
    if (!newNode->data) {
        free(newNode);
        return FAIL;
    }
    memcpy(newNode->data, element, p->size);

    newNode->prev = current->prev;
    newNode->next = current;
    current->prev->next = newNode;
    current->prev = newNode;
    p->count++;
    return SUCCESS;
}

// Função para remover o primeiro elemento
int rBegin(pDDLL p, void *element) {
    if (!p || !p->head) return FAIL;

    struct DDLLNode *node = p->head;
    memcpy(element, node->data, p->size);

    p->head = node->next;
    if (p->head) {
        p->head->prev = NULL;
    } else {
        p->tail = NULL;
    }

    free(node->data);
    free(node);
    p->count--;
    return SUCCESS;
}

// Função para remover o último elemento
int rEnd(pDDLL p, void *element) {
    if (!p || !p->tail) return FAIL;

    struct DDLLNode *node = p->tail;
    memcpy(element, node->data, p->size);

    p->tail = node->prev;
    if (p->tail) {
        p->tail->next = NULL;
    } else {
        p->head = NULL;
    }

    free(node->data);
    free(node);
    p->count--;
    return SUCCESS;
}

// Função para remover um elemento em uma posição específica
int rPosition(pDDLL p, int N, void *element) {
    if (!p || N < 0 || N >= p->count) return FAIL;

    if (N == 0) return rBegin(p, element);
    if (N == p->count - 1) return rEnd(p, element);

    struct DDLLNode *current = p->head;
    for (int i = 0; i < N; i++) {
        current = current->next;
    }

    memcpy(element, current->data, p->size);
    current->prev->next = current->next;
    current->next->prev = current->prev;

    free(current->data);
    free(current);
    p->count--;
    return SUCCESS;
}

// Função para verificar se a lista está vazia
int empty(pDDLL p) {
    return (p && p->count == 0) ? SUCCESS : FAIL;
}

// Função para contar os elementos da lista
int countElements(pDDLL p) {
    return p ? p->count : 0;
}
