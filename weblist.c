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
    //define o número de nós folhas da weblist
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
    if (!web || !dado) return FAIL;

    // Encontra o próximo índice usando round-robin
    static int nextIndex = 0;
    nextIndex = (nextIndex + 1) % web->totalLeaves;

    // Insere o dado no nó selecionado
    if (iEnd(web->nodes[nextIndex].list, dado) == FAIL) return FAIL;

    // printf - vou inserir o valor x no nó y no final da lista - para debug
    printf("Inserindo o valor %d no nó %d no final da lista\n", *dado, nextIndex);

    // printf da lista - para debug
    pLista(web);

    // Verifica e ajusta o balanceamento das folhas
    return balanceWebList(web);
}

// Função para remover um dado da WebList de forma balanceada
int rDado(pweblist web, int *dado) {
    if (!web || !dado) return FAIL;

    // Tenta remover o dado em cada folha
    for (int i = 0; i < web->totalLeaves; i++) {
        if (sBegin(web->nodes[i].list, dado) == SUCCESS) {
            if (rBegin(web->nodes[i].list, dado) == SUCCESS) {
                return balanceWebList(web); // Rebalanceia após remoção
            }
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

// Função auxiliar para redistribuir elementos entre as folhas
void redistributeElements(pweblist web) {
    int total_elements = 0;
    for (int i = 0; i < web->totalLeaves; i++) {
        total_elements += countElements(web->nodes[i].list);
    }

    int target_count = total_elements / web->totalLeaves;
    int remainder = total_elements % web->totalLeaves;

    // Redistribui os elementos para atingir o balanceamento
    for (int i = 0; i < web->totalLeaves; i++) {
        while (countElements(web->nodes[i].list) > target_count + (i < remainder ? 1 : 0)) {
            int temp;
            sBegin(web->nodes[i].list, &temp);
            rBegin(web->nodes[i].list, &temp);

            // Encontra a próxima folha com espaço
            for (int j = 0; j < web->totalLeaves; j++) {
                if (countElements(web->nodes[j].list) < target_count + (j < remainder ? 1 : 0)) {
                    iEnd(web->nodes[j].list, &temp);
                    break;
                }
            }
        }
    }
}

// Função para verificar e ajustar o balanceamento da WebList
int balanceWebList(pweblist web) {
    if (!web) return FAIL;

    int min_elements = countElements(web->nodes[0].list);
    int max_elements = min_elements;

    for (int i = 1; i < web->totalLeaves; i++) {
        int elements = countElements(web->nodes[i].list);
        if (elements < min_elements) min_elements = elements;
        if (elements > max_elements) max_elements = elements;
    }

    // Se a diferença excede 1, redistribui os elementos
    if (max_elements - min_elements > 1) {
        redistributeElements(web);
    }
    return SUCCESS;
}

int calculateNodes(int level) {
    // Implementar lógica para calcular o número de nós folha, que é 8^level+1, sem função pow
    int result = 8;
    for (int i = 1; i < level; i++) {
        result *= 8;
    }
    return result;
}
