#include "DDLL_pri.h"

int cDDLL(ppDDLL pp, int sizedata) {

    if (*pp != NULL) return FAIL;
    
    *pp = (pDDLL) malloc(sizeof(DDLL));

    if (*pp == NULL) return FAIL;

    (*pp)->head = NULL;
    (*pp)->tail = NULL;
    (*pp)->size = 0;
    (*pp)->sizedata = sizedata;
    return SUCCESS;
}

int dDDLL(ppDDLL pp) {
    if (*pp == NULL) return FAIL;

    Node *aux = (*pp)->head;
    Node *next = NULL;

    while (aux != NULL) {
        next = aux->next;
        free(aux->data);
        free(aux);
        aux = next;
    }

    free(*pp);
    *pp = NULL;
    return SUCCESS;
}

int cleanDDLL(pDDLL p) {
    if (p == NULL) return FAIL;

    Node *aux = p->head;
    Node *next = NULL;

    // enquanto houver elementos na lista, libera a memória alocada e avança para o próximo elemento
    while (aux != NULL) {
        next = aux->next;
        free(aux->data);
        free(aux);
        aux = next;
    }

    p->head = NULL;
    p->tail = NULL;
    p->size = 0;
    return SUCCESS;
}

int iBegin(pDDLL p, void *element) {
    if (p == NULL) return FAIL;

    Node *new = (Node *) malloc(sizeof(Node));
    if (new == NULL) return FAIL;

    new->data = malloc(p->sizedata);
    if (new->data == NULL) {
        free(new);
        return FAIL;
    }

    memcpy(new->data, element, p->sizedata); // copia os dados para o novo elemento
    new->prev = NULL; // como inserimos no início, o elemento anterior é NULL
    new->next = p->head; // o próximo elemento é o que era o primeiro

    if (p->head == NULL) { // se a lista estava vazia, o novo elemento é o primeiro e o último
        p->head = new;
        p->tail = new;
    } 
    
    else { // se não, o novo elemento é o primeiro e o antigo primeiro agora é o segundo
        p->head->prev = new;
        p->head = new;
    }

    p->size++;
    return SUCCESS;
}

int iEnd(pDDLL p, void *element) {
    if (p == NULL) return FAIL;

    Node *new = (Node *) malloc(sizeof(Node));
    if (new == NULL) return FAIL;

    new->data = malloc(p->sizedata);
    if (new->data == NULL) {
        free(new);
        return FAIL;
    }

    memcpy(new->data, element, p->sizedata); // copia os dados para o novo elemento
    new->next = NULL; // como inserimos no final, o próximo elemento é NULL
    new->prev = p->tail; // o elemento anterior é o que era o último

    if (p->tail == NULL) { // se a lista estava vazia, o novo elemento é o primeiro e o último
        p->head = new;
        p->tail = new;
    } 
    
    else { // se não, o novo elemento é o último e o antigo último agora é o penúltimo
        p->tail->next = new;
        p->tail = new;
    }

    p->size++;
    return SUCCESS;
}

int iPosition(pDDLL p, int N, void *element) {
    if (p == NULL || N < 0 || N > p->size) return FAIL;

    if (N==0) return iBegin(p, element); // se a posição for 0, insere no início
    if (N==p->size) return iEnd(p, element); // se a posição for o tamanho da lista, insere no final

    Node *aux = p->head;
    for (int i=0; i<N; i++) aux = aux->next; // avança até a posição N

    Node *new = (Node *) malloc(sizeof(Node));
    if (new == NULL) return FAIL;

    new->data = malloc(p->sizedata);
    if (new->data == NULL) {
        free(new);
        return FAIL;
    }

    memcpy(new->data, element, p->sizedata); // copia os dados para o novo elemento
    new->prev = aux->prev; // o elemento anterior é o anterior ao que estava na posição N
    new->next = aux; // o próximo elemento é o que estava na posição N

    aux->prev->next = new; // o próximo do anterior ao que estava na posição N é o novo elemento
    aux->prev = new; // o anterior do que estava na posição N é o novo elemento

    p->size++;
    return SUCCESS;
}

int rBegin(pDDLL p, void *element) {
    if (p == NULL || p->size == 0 || p->head == NULL) return FAIL;

    Node *aux = p->head;
    memcpy(element, aux->data, p->sizedata); // copia os dados do primeiro elemento para a variável element

    if (p->size == 1) { // se a lista tinha apenas um elemento, o primeiro e o último são NULL
        p->head = NULL;
        p->tail = NULL;
    } 
    
    else { // se não, o segundo elemento é o novo primeiro
        p->head = aux->next;
        p->head->prev = NULL;
    }

    free(aux->data);
    free(aux);
    p->size--;
    return SUCCESS;
}

int rEnd(pDDLL p, void *element) {
    if (p == NULL || p->size == 0 || p->tail == NULL) return FAIL;

    Node *aux = p->tail;
    memcpy(element, aux->data, p->sizedata); // copia os dados do último elemento para a variável element

    if (p->size == 1) { // se a lista tinha apenas um elemento, o primeiro e o último são NULL
        p->head = NULL;
        p->tail = NULL;
    } 
    
    else { // se não, o penúltimo elemento é o novo último
        p->tail = aux->prev;
        p->tail->next = NULL;
    }

    free(aux->data);
    free(aux);
    p->size--;
    return SUCCESS;
}

int rPosition(pDDLL p, int N, void *element) {
    if (p == NULL || N < 0 || N >= p->size) return FAIL;

    if (N==0) return rBegin(p, element); // se a posição for 0, remove o primeiro elemento
    if (N==p->size-1) return rEnd(p, element); // se a posição for o tamanho da lista - 1, remove o último elemento

    Node *aux = p->head;
    for (int i=0; i<N; i++) aux = aux->next; // avança até a posição N

    aux->prev->next = aux->next; // o próximo do anterior ao que estava na posição N é o próximo do que estava na posição N
    aux->next->prev = aux->prev; // o anterior do próximo ao que estava na posição N é o anterior do que estava na posição N

    memcpy(element, aux->data, p->sizedata); // copia os dados do elemento removido para a variável element
    free(aux->data);
    free(aux);
    p->size--;
    return SUCCESS;
}

int sBegin(pDDLL p, void *element) {
    if (p == NULL || p->size == 0 || p->head == NULL) return FAIL;

    memcpy(element, p->head->data, p->sizedata); // copia os dados do primeiro elemento para a variável element
    return SUCCESS;
}

int sEnd(pDDLL p, void *element) {
    if (p == NULL || p->size == 0 || p->tail == NULL) return FAIL;

    memcpy(element, p->tail->data, p->sizedata); // copia os dados do último elemento para a variável element
    return SUCCESS;
}

int sPosition(pDDLL p, int N, void *element) {
    if (p == NULL || N < 0 || N >= p->size) return FAIL;

    Node *aux = p->head;
    for (int i=0; i<N; i++) aux = aux->next; // avança até a posição N

    memcpy(element, aux->data, p->sizedata); // copia os dados do elemento na posição N para a variável element
    return SUCCESS;
}

int empty(pDDLL p) {
    if (p == NULL) return FAIL;

    return (p->size == 0) ? SUCCESS : FAIL;
}

int countElements(pDDLL p) {
    if (p == NULL) return FAIL;

    return p->size;
}
