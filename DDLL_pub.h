#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SUCCESS 0
#define FAIL 1

typedef struct DDLL *pDDLL, **ppDDLL;


// Funções básicas de uma DDLL
/**
 * @brief Cria uma nova lista duplamente encadeada
 * 
 * @param pp ponteiro para o ponteiro que aponta para a lista
 * @param sizedata  tamanho dos dados que serao armazenados em cada elemento da lista
 * @return int 0 se a lista foi criada com sucesso, 1 caso contrário
 */
int cDDLL(ppDDLL pp, int sizedata);

/**
 * @brief Destroi a lista e libera toda a memória alocada
 * 
 * @param pp ponteiro para o ponteiro que aponta para a lista
 * @return int 0 se a lista foi destruída com sucesso, 1 caso contrário
 */
int dDDLL(ppDDLL pp);

/**
 * @brief Remove todos os elementos da lista, mantendo-a vazia
 * 
 * @param p ponteiro para a lista
 * @return int 0 se a lista foi limpa com sucesso, 1 caso contrário
 */
int cleanDDLL(pDDLL p);

/**
 * @brief Insere um elemento no inicio da lista
 * 
 * @param p ponteiro para a lista
 * @param element ponteiro para o elemento a ser inserido
 * @return int 0 se o elemento foi inserido com sucesso, 1 caso contrário
 */
int iBegin(pDDLL p, void *element);

/**
 * @brief Insere um elemento no final da lista
 * 
 * @param p ponteiro para a lista
 * @param element elemento a ser inserido
 * @return int 0 se o elemento foi inserido com sucesso, 1 caso contrário
 */
int iEnd(pDDLL p, void *element);

/**
 * @brief insere um elemento em uma posicao especifica na lista
 * 
 * @param p ponteiro para a lista
 * @param N posição onde o elemento será inserido
 * @param element elemento a ser inserido
 * @return int 0 se o elemento foi inserido com sucesso, 1 caso contrário
 */
int iPosition(pDDLL p, int N, void *element);

/**
 * @brief remove o primeiro elemento da lista
 * 
 * @param p ponteiro para a lista
 * @param element elemento removido
 * @return int 0 se o elemento foi removido com sucesso, 1 caso contrário
 */
int rBegin(pDDLL p, void *element);

/**
 * @brief remove o último elemento da lista
 * 
 * @param p ponteiro para a lista
 * @param element elemento removido
 * @return int 0 se o elemento foi removido com sucesso, 1 caso contrário
 */
int rEnd(pDDLL p, void *element);

/**
 * @brief remove um elemento de uma posição específica na lista
 * 
 * @param p ponteiro para a lista
 * @param N posição do elemento a ser removido
 * @param element elemento removido
 * @return int 0 se o elemento foi removido com sucesso, 1 caso contrário
 */
int rPosition(pDDLL p, int N, void *element);

/**
 * @brief retorna o primeiro elemento da lista
 * 
 * @param p ponteiro para a lista
 * @param element elemento retornado
 * @return int 0 se o elemento foi retornado com sucesso, 1 caso contrário
 */
int sBegin(pDDLL p, void *element);

/**
 * @brief retorna o último elemento da lista
 * 
 * @param p ponteiro para a lista
 * @param element elemento retornado
 * @return int 0 se o elemento foi retornado com sucesso, 1 caso contrário
 */
int sEnd(pDDLL p, void *element);

/**
 * @brief retorna um elemento de uma posição específica na lista
 * 
 * @param p ponteiro para a lista
 * @param N posição do elemento a ser retornado
 * @param element elemento retornado
 * @return int 0 se o elemento foi retornado com sucesso, 1 caso contrário
 */
int sPosition(pDDLL p, int N, void *element);

// Funções adicionais
/**
 * @brief verifica se a lista está vazia
 * 
 * @param p 
 * @return int 0 se a lista está vazia, 1 caso contrário
 */
int empty(pDDLL p);

/**
 * @brief retorna o numero de elementos na lista
 * 
 * @param p 
 * @return int o numero de elementos na lista
 */
int countElements(pDDLL p);