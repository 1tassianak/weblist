#ifndef WEBLIST_PUB_H
#define WEBLIST_PUB_H

#include "DDLL_pub.h"
#include "weblist_pri.h"

# define SUCCESS 0
# define FAIL -1

typedef struct weblist * pweblist , ** ppweblist ;

// Funcoes operacionais
int cWL (ppweblist web, int sizedata) ; // criar a estrutura ;
int dWL (ppweblist web) ; // destruir a estrutura ;

// Funcoes focada nos dados
int iDado (pweblist web, void *dado) ; // inserir um novo dado na estrutura ;
int rDado (pweblist web, void *dado) ; // remover um dado da estrutura ;
int bDado (pweblist web, void *dado) ; // buscar um dado na estrutura ;
int pLista(pweblist web, void (*printFunc)(void*)) ; // percorrer a lista de dados ;

// Funcoes focada nos nos - folha
int cpLista (pweblist web , int chave , ppDDLL retorno ) ; // retornar uma copia da DDLL correspondente a chave ;
int sbLista (pweblist web , int chave , pDDLL novaLista ); // substituir a lista DDLL correspondente a chave pela lista recebida por parametro ( novaLista );
int rmLista (pweblist web , int chave , ppDDLL rmLista ) ; // retornar a lista ’ rmLista ’ por parametro e remover a lista DDLL correspondente a chave ;
int nvLista (pweblist web , int chave ) ; // criar uma DDLL vazia para a chave recebida como parametro ;

// Funcoes da WebList
int nroEleNoFolha (pweblist web , int * retorno ) ; // retornar o numero de elementos em um no - folha especifico ( soma de elementos de cada lista do no - folha )
int nroNoFolha (pweblist web , int* retorno ); // retornar o numero total de nos - folha da estrutura
int nroEleWL (pweblist web , int * retorno ); // retornar o numero total de elementos cadastrados na webList
int lstChaves (pweblist web , ppDDLL retorno ); // retornar uma lista com todas as chaves da WebList .
int WLbalanceada (pweblist web) ; // retornar SUCCESS se a webList estiver balanceada e, FAIL , caso contrario .

//FUNÇÕES ADICIONAIS ALTERNATIVAS   
//FUNÇÕES DE PERCURSO PARA A WEBLIST
void preOrderTraversal(pweblist web, int node_index);
void inOrderTraversal(pweblist web, int node_index);
void postOrderTraversal(pweblist web, int node_index);

// DECLARAÇÕES DAS FUNÇÕES DE BALANCEAMENTO ALTERNATIVAS
int redistributeBalance(pweblist web);  // BALANCEAMENTO POR REDISTRIBUIÇÃO
int heightBasedBalance(pweblist web);   // BALANCEAMENTO POR ALTURA
// FUNÇÃO PARA EXIBIR A ESTRUTURA DA WEBLIST
void exibirWebList(pweblist web);

#endif