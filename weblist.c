#include <stdlib.h>
#include "weblist_pri.h"

//função para criar a weblist e inicializar cada nó com uma DDLL vazia
int cWL(ppweblist web) {
    //aloca memória para a weblist
    *web = (pweblist)malloc(sizeof(struct weblist));
    //verifica se deu certo a alocação, se não deu, falha
    if (!(*web)) return FAIL;

    //define o nível da weblist
    (*web)->level = 0;
    //define o número de nós folhas (nesse caso, 8)
    (*web)->node_count = 8;
    //aloca espaço para os nós
    (*web)->nodes = (WebListNode *)malloc(8 * sizeof(WebListNode));
    //verifica se o espaço foi alocado
    if (!(*web)->nodes) {
        //se falhar, libera memória
        free(*web);
        return FAIL;
    }

    //inicializa cada nó com uma chave e uma DDLL vazia
    for (int i = 0; i < (*web)->node_count; i++) {
        //define uma chave para cada nó
        (*web)->nodes[i].key = i;
        //cria uma DDLL vazia para cada nó
        if (cDDLL(&((*web)->nodes[i].list), sizeof(int)) == FAIL) {
            //em caso de falha, limpa o que foi alocado e retorna erro
            for (int j = 0; j < i; j++) {
                dDDLL(&((*web)->nodes[j].list));
            }
            //libera os nós
            free((*web)->nodes);
            //libera a weblist
            free(*web);
            return FAIL;
        }
    }
    return SUCCESS;
}


//função para destruir a weblist
int dWL(pweblist *web) {
    //verifica se a weblist existe, se não existe, falha
    if (!web || !(*web)) return FAIL;
    //percorre todos os nós da weblist
    for (int i = 0; i < (*web)->node_count; i++) {
        //verifica se a DDLL existe
        if ((*web)->nodes[i].list) {
            //destrói a DDLL associada ao nó
            dDDLL(&(*web)->nodes[i].list);
        }
    }
    //libera a memória dos nós
    free((*web)->nodes);
    //libera a estrutura principal da weblist
    free(*web);
    //define o ponteiro como NULL
    *web = NULL;

    return SUCCESS;
}

//função para inserir dado com verificação de balanceamento
int iDado(pweblist web, int *dado) {
    //se a weblist não existe, falha
    if (!web) return FAIL;

    //inserindo no primeiro nó
    int node_index = 0;

    //insere o dado na DDLL do nó especificado
    //insere no início da DDLL
    int resultado = iBegin(web->nodes[node_index].list, dado);
    if (resultado == SUCCESS) {
        //verifica e ajusta o balanceamento após a inserção
        resultado = balanceWebList(web);
    }
    return resultado;
}


//função para remover dado
int rDado(pweblist web, int *dado) {
    //verifica se a weblist existe, se não, falha
    if (!web) return FAIL;
    //remove do início da DDLL no primeiro nó
    return rBegin(web->nodes[0].list, dado);
}

//função para buscar dado
int bDado(pweblist web, int *dado) {
    if (!web) return FAIL;
    // Implementar lógica de busca na DDLL correspondente
    return FAIL;
}

// Função para percorrer lista
int pLista(pweblist web) {
    if (!web) return FAIL;
    // Lógica para percorrer listas
    return SUCCESS;
}

// Funções de nós folha
int cpLista(pweblist web, int chave, ppDDLL retorno) {
    if (!web || chave >= web->node_count) return FAIL;
    *retorno = web->nodes[chave].list;
    return SUCCESS;
}

int sbLista(pweblist web, int chave, pDDLL novaLista) {
    if (!web || chave >= web->node_count) return FAIL;
    web->nodes[chave].list = novaLista;
    return SUCCESS;
}

int rmLista(pweblist web, int chave, ppDDLL rmLista) {
    if (!web || chave >= web->node_count) return FAIL;
    *rmLista = web->nodes[chave].list;
    web->nodes[chave].list = NULL;
    return SUCCESS;
}

int nvLista(pweblist web, int chave) {
    if (!web || chave >= web->node_count) return FAIL;
    return cDDLL(&(web->nodes[chave].list), sizeof(int));
}

// Funções da WebList
int nroEleNoFolha(pweblist web, int *retorno) {
    if (!web) return FAIL;
    *retorno = countElements(web->nodes[0].list);
    return SUCCESS;
}

int nroNoFolha(pweblist web, int *retorno) {
    if (!web) return FAIL;
    *retorno = web->node_count;
    return SUCCESS;
}

int nroEleWL(pweblist web, int *retorno) {
    if (!web) return FAIL;
    *retorno = 0;
    for (int i = 0; i < web->node_count; i++) {
        *retorno += countElements(web->nodes[i].list);
    }
    return SUCCESS;
}

int lstChaves(pweblist web, ppDDLL retorno) {
    if (!web) return FAIL;
    *retorno = NULL;
    // Implementação da lista de chaves se necessário
    return SUCCESS;
}

int WLbalanceada(pweblist web) {
    if (!web) return FAIL;
    return balanceWebList(web);
}

// Função auxiliar para verificar o balanceamento da WebList
int balanceWebList(pweblist web) {
    int min_elements = countElements(web->nodes[0].list);
    int max_elements = min_elements;
    for (int i = 1; i < web->node_count; i++) {
        int elements = countElements(web->nodes[i].list);
        if (elements < min_elements) min_elements = elements;
        if (elements > max_elements) max_elements = elements;
    }
    return (max_elements - min_elements <= 1) ? SUCCESS : FAIL;
}
