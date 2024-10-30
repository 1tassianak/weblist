#include "weblist_pri.h"

//função para criar a weblist e inicializar cada nó com uma DDLL vazia
int cWL(ppweblist web, int level, int sizedata) {
    if(level<0) return FAIL;
    //aloca memória para a weblist
    *web = (pweblist)malloc(sizeof(struct weblist));
    //verifica se deu certo a alocação, se não deu, falha
    if (!(*web)) return FAIL;

    //define o nível da weblist
    (*web)->level = level;
    //define o número de nós folhas (nesse caso, 8)
    (*web)->totalLeaves = calculateNodes(level);
    //aloca espaço para os nós
    (*web)->nodes = (WebListNode *)malloc((*web)->totalLeaves * sizeof(WebListNode));
    //verifica se o espaço foi alocado
    if (!(*web)->nodes) {
        //se falhar, libera memória
        free(*web);
        return FAIL;
    }
    //define o tamanho dos dados
    (*web)->sizedata = sizedata;

    //inicializa cada nó com uma chave e uma DDLL vazia
    for (int i = 0; i < (*web)->totalLeaves; i++) {
        //define uma chave para cada nó
        (*web)->nodes[i].key = i;
        //cria uma DDLL vazia para cada nó
        if (cDDLL(&((*web)->nodes[i].list), sizedata) == FAIL) {
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
int dWL(ppweblist web) {
    //verifica se a weblist existe, se não existe, falha
    if (!web || !(*web)) return FAIL;
    //percorre todos os nós da weblist
    for (int i = 0; i < (*web)->totalLeaves; i++) {
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
    if (!web || !dado) return FAIL;
    int minIndex = 0;
    int minCount = countElements(web->nodes[0].list);

    for (int i=0; i<web->totalLeaves; i++) {
        int count = countElements(web->nodes[i].list);
        if (count < minCount) {
            minCount = count;
            minIndex = i;
        }
    }

    return iEnd(web->nodes[minIndex].list, dado);
}


//função para remover dado
int rDado(pweblist web, int *dado) {
    //verifica se a weblist existe, se não, falha
    if (!web || !dado) return FAIL;

    for (int i = 0; i < web->totalLeaves; i++) {
        //verifica se o dado foi removido
        if (rBegin(web->nodes[i].list, dado) == SUCCESS) {
            return SUCCESS;
        }
    }

    return FAIL;
    
}

//função para buscar dado
int bDado(pweblist web, int *dado) {
    if (!web || !dado) return FAIL;
    // Implementar lógica de busca na DDLL correspondente
    for (int i = 0; i < web->totalLeaves; i++) {
        if (sBegin(web->nodes[i].list, dado) == SUCCESS) {
            return SUCCESS;
        }
    }

    return FAIL;
}

// Função para percorrer lista
int pLista(pweblist web) {
    if (!web) return FAIL;
    // Lógica para percorrer listas
    for (int i = 0; i < web->totalLeaves; i++) {
        int count = countElements(web->nodes[i].list);
        printf("Nó %d (Chave %d): Nº de Elementos: %d Elementos da lista: ", i, web->nodes[i].key, count);
        // Percorrer a lista
        int temp;
        for (int j = 0; j < count; j++) {
            if (sPosition(web->nodes[i].list, j, &temp) == SUCCESS) {
                printf("%d ", temp);
            }
        }
        printf("\n");
    }
    return SUCCESS;
}

// Funções de nós folha
int cpLista(pweblist web, int chave, ppDDLL retorno) {
    if (!web || chave >= web->totalLeaves || !retorno || chave < 0) return FAIL;
    *retorno = web->nodes[chave].list;
    return SUCCESS;
}

int sbLista(pweblist web, int chave, pDDLL novaLista) {
    if (!web || chave >= web->totalLeaves || chave < 0) return FAIL;
    web->nodes[chave].list = novaLista;
    return SUCCESS;
}

int rmLista(pweblist web, int chave, ppDDLL rmLista) {
    if (!web || chave >= web->totalLeaves) return FAIL;
    *rmLista = web->nodes[chave].list;
    web->nodes[chave].list = NULL;
    return SUCCESS;
}

int nvLista(pweblist web, int chave) {
    if (!web || chave >= web->totalLeaves) return FAIL;
    return cDDLL(&(web->nodes[chave].list), web->sizedata);
}

// Funções da WebList
int nroEleNoFolha(pweblist web, int chave, int *retorno) {
    if (!web || !retorno || chave < 0 || chave >= web->totalLeaves) return FAIL;
    *retorno = countElements(web->nodes[0].list);
    return SUCCESS;
}

int nroNoFolha(pweblist web, int *retorno) {
    if (!web || !retorno) return FAIL;
    *retorno = web->totalLeaves;
    return SUCCESS;
}

int nroEleWL(pweblist web, int *retorno) {
    if (!web || !retorno) return FAIL;
    *retorno = 0;
    for (int i = 0; i < web->totalLeaves; i++) {
        *retorno += countElements(web->nodes[i].list);
    }
    return SUCCESS;
}

int lstChaves(pweblist web, ppDDLL retorno) {
    if (!web || !retorno) return FAIL;
    
    if (cDDLL(retorno, sizeof(int)) == FAIL) return FAIL;

    for (int i = 0; i < web->totalLeaves; i++) {
        iEnd(*retorno, &web->nodes[i].key);
    }

    return SUCCESS;
}

int WLbalanceada(pweblist web) {
    if (!web) return FAIL;
    return balanceWebList(web);
}

// Função auxiliar para verificar o balanceamento da WebList
int balanceWebList(pweblist web) {
    if (!web) return FAIL;

    int min_elements = countElements(web->nodes[0].list);
    int max_elements = min_elements;
    for (int i = 1; i < web->totalLeaves; i++) {
        int elements = countElements(web->nodes[i].list);
        if (elements < min_elements) min_elements = elements;
        if (elements > max_elements) max_elements = elements;
    }
    return (max_elements - min_elements <= 1) ? SUCCESS : FAIL;
}

int calculateNodes(int level) {
    int nodes = 1; // Começa com o nó raiz
    for (int i = 0; i < level; i++) {
        nodes *= 8; // Multiplica por 8 para cada nível
    }
    return nodes;
}
