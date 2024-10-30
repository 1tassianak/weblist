#ifndef WEBLIST_PUB_H
#define WEBLIST_PUB_H

#include "DDLL_pri.h"

typedef struct weblist *pweblist , **ppweblist;

// Funcoes operacionais
int cWL (ppweblist web, int level, int sizedata) ; // criar a estrutura ;
int dWL (ppweblist web) ; // destruir a estrutura ;

// Funcoes focada nos dados
int iDado (pweblist web, int *dado) ; // inserir um novo dado na estrutura ;
int rDado (pweblist web, int *dado) ; // remover um dado da estrutura ;
int bDado (pweblist web, int *dado) ; // buscar um dado na estrutura ;
int pLista (pweblist web) ; // percorrer a lista de dados ;

// Funcoes focada nos nos - folha
int cpLista (pweblist web , int chave , ppDDLL retorno ) ; // retornar uma copia da DDLL correspondente a chave ;
int sbLista (pweblist web , int chave , pDDLL novaLista ); // substituir a lista DDLL correspondente a chave pela lista recebida por parametro ( novaLista );
int rmLista (pweblist web , int chave , ppDDLL rmLista ) ; // retornar a lista ’ rmLista ’ por parametro e remover a lista DDLL correspondente a chave ;
int nvLista (pweblist web , int chave ) ; // criar uma DDLL vazia para a chave recebida como parametro ;

// Funcoes da WebList
int nroEleNoFolha (pweblist web ,int chave, int * retorno ) ; // retornar o numero de elementos em um no - folha especifico ( soma de elementos de cada lista do no - folha )
int nroNoFolha (pweblist web , int* retorno ); // retornar o numero total de nos - folha da estrutura
int nroEleWL (pweblist web , int * retorno ); // retornar o numero total de elementos cadastrados na webList
int lstChaves (pweblist web , ppDDLL retorno ); // retornar uma lista com todas as chaves da WebList .
int WLbalanceada (pweblist web) ; // retornar SUCCESS se a webList estiver balanceada e, FAIL , caso contrario .

#endif