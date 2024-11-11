    #include <stdio.h>    // Para printf
    #include <string.h>   // Para memcmp
    #include <stdlib.h>   // Para malloc e free
    #include "weblist_pub.h"
    #include "weblist_pri.h"
    #include <math.h> // INCLUI <math.h> PARA USO DA FUNÇÃO log2
    #include <limits.h> // INCLUI <limits.h> PARA USO DE INT_MAX E INT_MIN

        // Função para criar a WebList e inicializar cada nó com DDLLs vazias
int cWL(ppweblist web, int level, int sizedata) {
    *web = (pweblist)malloc(sizeof(struct weblist));
    if (!(*web)) return FAIL;

    (*web)->level = level;

    // Número total de nós: 1 para nível 0, 9 para nível 1, 8^n + 1 para níveis superiores
    (*web)->node_count = (level == 0) ? 1 : (int)pow(8, level) + 1; // 8^n + 1
    (*web)->nodes = (WebListNode *)malloc((*web)->node_count * sizeof(WebListNode));
    if (!(*web)->nodes) {
        free(*web);
        return FAIL;
    }

    // Inicializa cada nó
    for (int i = 0; i < (*web)->node_count; i++) {
        // Se for nível 0, o único nó é uma folha; se for nível 1, os nós 1 a 8 são folhas
        (*web)->nodes[i].is_leaf = (level == 0 || (level == 1 && i > 0));

        if ((*web)->nodes[i].is_leaf) {
            // Inicializa as listas para nós folha
            for (int j = 0; j < 8; j++) {
                (*web)->nodes[i].lists[j].key = j; // Define a chave da lista
                (*web)->nodes[i].lists[j].list = NULL; // Inicializa a DDLL como NULL

                // Cria a DDLL para a lista
                if (cDDLL(&((*web)->nodes[i].lists[j].list), sizedata) == FAIL) {
                    for (int k = 0; k < j; k++) {
                        dDDLL(&((*web)->nodes[i].lists[k].list));
                    }
                    free((*web)->nodes);
                    free(*web);
                    return FAIL;
                }
            }
        } else {
            // Inicializa os filhos para nós internos
            for (int j = 0; j < 8; j++) {
                (*web)->nodes[i].children[j] = NULL; // Inicializa filhos como NULL
            }
        }
        printf("Nó %d criado com sucesso. (Folha: %d)\n", i, (*web)->nodes[i].is_leaf);
    }

    (*web)->sizedata = sizedata;
    return SUCCESS;
}


    /*
    /// PERCURSO PRÉ-ORDEM COM VERIFICAÇÃO DE LIMITES
    void preOrderTraversal(pweblist web, int node_index) {
        if (!web || node_index >= web->node_count) return;
        printf("Visitando nó %d em pré-ordem (chave: %d)\n", node_index, web->nodes[node_index].key);
        
        // PERCORRE ATÉ 8 "FILHOS" POSSÍVEIS COM VERIFICAÇÃO DE LIMITES
        for (int i = 1; i <= 2 && node_index * 8 + i < web->node_count; i++) {
            int child_index = node_index * 8 + i;
            if (child_index < web->node_count) {
                preOrderTraversal(web, child_index);
            }
        }
    }

    // PERCURSO EM ORDEM COM VERIFICAÇÃO DE LIMITES
    void inOrderTraversal(pweblist web, int node_index) {
        if (!web || node_index >= web->node_count) return;

        // VISITA O PRIMEIRO "FILHO" SIMULADO
        if (node_index * 2 + 1 < web->node_count) {
            inOrderTraversal(web, node_index * 2 + 1);
        }

        printf("Visitando nó %d em ordem (chave: %d)\n", node_index, web->nodes[node_index].key);

        // VISITA O SEGUNDO "FILHO" SIMULADO
        if (node_index * 2 + 2 < web->node_count) {
            inOrderTraversal(web, node_index * 2 + 2);
        }
    }

    // PERCURSO PÓS-ORDEM COM VERIFICAÇÃO DE LIMITES
    void postOrderTraversal(pweblist web, int node_index) {
        if (!web || node_index >= web->node_count) return;

        // VISITA O PRIMEIRO "FILHO" SIMULADO
        if (node_index * 2 + 1 < web->node_count) {
            postOrderTraversal(web, node_index * 2 + 1);
        }

        // VISITA O SEGUNDO "FILHO" SIMULADO
        if (node_index * 2 + 2 < web->node_count) {
            postOrderTraversal(web, node_index * 2 + 2);
        }

        printf("Visitando nó %d em pós-ordem (chave: %d)\n", node_index, web->nodes[node_index].key);
    }  
    */

    // Função para destruir a WebList
int dWL(pweblist *web) {
    if (!web || !(*web)) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < (*web)->node_count; i++) {
        if ((*web)->nodes[i].is_leaf) {
            // Para nós folha, libera todas as listas
            for (int j = 0; j < 8; j++) {
                if ((*web)->nodes[i].lists[j].list) {
                    dDDLL(&((*web)->nodes[i].lists[j].list)); // Libera a DDLL
                }
            }
        }
    }

    // Libera a memória alocada para os nós e a estrutura da WebList
    free((*web)->nodes);
    free(*web);
    *web = NULL; // Define o ponteiro como NULL para evitar dangling pointer

    return SUCCESS;
}

    //função para inserir dado com verificação de balanceamento
    //distribui a carga entre os nós de forma cíclica,
    //em vez de inserir dados sempre no mesmo nó, os dados são inseridos de maneira rotativa em cada nó, um após o outro. 
    //Ajudando a "balancear" a carga de dados entre os nós, fazendo com que cada nó receba uma quantidade de dados mais ou menos igual ao longo do tempo.
    //Depois utiliza iBegin para inserir os dados e verifica o balanceamento após cada inserção
// Função para inserir dado com verificação de balanceamento
    int iDado(pweblist web, void *dado) {
        if (!web) return FAIL;

        void *data_copy = malloc(web->sizedata);
        if (!data_copy) return FAIL;
        memcpy(data_copy, dado, web->sizedata);

        // Encontrar o nó adequado para inserção
        static int node_index = 0; // Para percorrer os nós
        node_index = (node_index + 1) % web->node_count; // Alterna entre os nós

        int best_list_index = -1;
        int min_elements = 10000000;

        // Verifica qual lista no nó atual tem o menor número de elementos
        for (int i = 0; i < 8; i++) {
            if (web->nodes[node_index].lists[i].list) {
                int element_count = countElements(web->nodes[node_index].lists[i].list);
                if (element_count < min_elements) {
                    min_elements = element_count;
                    best_list_index = i; // Seleciona a lista com o menor número de elementos
                }
            } else {
                // Se a lista estiver vazia, insere o dado nela
                best_list_index = i;
                break;
            }
        }

        // Insere o dado na lista selecionada
        int resultado = iBegin(web->nodes[node_index].lists[best_list_index].list, data_copy);

        // Verifica o balanceamento após a inserção
        if (balanceWebList(web) == FAIL) {
            printf("WebList is not balanced after inserting data\n");
        } else {
            printf("WebList is balanced after inserting data\n");
        }

        free(data_copy);
        
        return resultado;
    }


    // Função para remover dado da WebList
    int rDado(pweblist web, void *dado) {
        if (!web || !dado) return FAIL;

        for (int i = 0; i < web->node_count; i++) {
            if (sBegin(web->nodes[i].lists[0].list, dado) == SUCCESS) {
                if (rBegin(web->nodes[i].lists[0].list, dado) == SUCCESS) {
                    // Verifica o balanceamento após a remoção
                    if (WLbalanceada(web) == FAIL) {
                        printf("Balancing after removal...\n");
                        return balanceWebList(web);
                    } else {
                        printf("WebList is balanced after removing data\n");
                    }
                    return SUCCESS;
                }
            }
        }

        return FAIL;
    }
                    
    int bDado(pweblist web, void *dado) {
    // Verifica se a WebList ou o dado é nulo
    if (!web || !dado) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < web->node_count; i++) {
        // Verifica se o nó atual é um nó folha
        if (web->nodes[i].is_leaf) {
            // Percorre cada lista associada ao nó
            for (int j = 0; j < 8; j++) {
                // Verifica se a lista não é nula
                if (web->nodes[i].lists[j].list) {
                    // Aloca memória para armazenar dados temporários
                    void *temp_data = malloc(web->sizedata);
                    int pos = 0;

                    // Verifica o primeiro elemento da lista
                    if (sBegin(web->nodes[i].lists[j].list, temp_data) == SUCCESS) {
                        // Compara o primeiro elemento com o dado procurado
                        if (memcmp(temp_data, dado, web->sizedata) == 0) {
                            free(temp_data); // Libera memória alocada
                            return SUCCESS; // Retorna sucesso se encontrou o dado
                        }

                        // Percorre os demais elementos da lista
                        while (sPosition(web->nodes[i].lists[j].list, ++pos, temp_data) == SUCCESS) {
                            // Compara cada elemento com o dado procurado
                            if (memcmp(temp_data, dado, web->sizedata) == 0) {
                                free(temp_data); // Libera memória alocada
                                return SUCCESS; // Retorna sucesso se encontrou o dado
                            }
                        }
                    }
                    free(temp_data); // Libera memória alocada se não encontrou o dado
                }
            }
        }
    }
    return FAIL; // Retorna falha se o dado não foi encontrado em nenhuma lista
}



    // Função para percorrer e exibir a lista de dados na WebList

    int pLista(pweblist web, void (*printFunc)(void*)) {
        if (!web) return FAIL;

        for (int i = 0; i < web->node_count; i++) {
            printf("Nó %d (Folha: %d):\n", i, web->nodes[i].is_leaf);
            if (web->nodes[i].is_leaf) {
                for (int j = 0; j < 8; j++) {
                    if (web->nodes[i].lists[j].list) {
                        printf("  Lista (chave: %d):\n", web->nodes[i].lists[j].key);
                        void *temp_data = malloc(web->sizedata);
                        int pos = 0;

                        if (sBegin(web->nodes[i].lists[j].list, temp_data) == SUCCESS) {
                            printFunc(temp_data);

                            while (sPosition(web->nodes[i].lists[j].list, ++pos, temp_data) == SUCCESS) {
                                printFunc(temp_data);
                            }
                        } else {
                            printf("  DDLL vazia.\n");
                        }
                        free(temp_data);
                    } else {
                            printf("  DDLL vazia.\n");
                        }
                    }
                }
            }

            return SUCCESS;
        }

    // Funções de nós folha
int cpLista(pweblist web, int chave, ppDDLL retorno) {
    // Verifica se a WebList ou a chave são inválidas
    if (!web || chave < 0) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < web->node_count; i++) {
        // Verifica se o nó atual é um nó folha
        if (web->nodes[i].is_leaf) {
            // Percorre as 8 listas do nó
            for (int j = 0; j < 8; j++) {
                // Verifica se a chave corresponde
                if (web->nodes[i].lists[j].key == chave) {
                    *retorno = web->nodes[i].lists[j].list; // Retorna a lista correspondente
                    return SUCCESS; // Retorna sucesso
                }
            }
        }
    }
    return FAIL; // Retorna falha se a chave não for encontrada
}



int sbLista(pweblist web, int chave, pDDLL novaLista) {
    // Verifica se a WebList ou a chave são inválidas
    if (!web || chave < 0) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < web->node_count; i++) {
        // Verifica se o nó atual é um nó folha
        if (web->nodes[i].is_leaf) {
            // Percorre as 8 listas do nó
            for (int j = 0; j < 8; j++) {
                // Verifica se a chave corresponde
                if (web->nodes[i].lists[j].key == chave) {
                    // Destrói a lista antiga
                    dDDLL(&(web->nodes[i].lists[j].list));
                    // Substitui pela nova lista
                    web->nodes[i].lists[j].list = novaLista;
                    return SUCCESS; // Retorna sucesso
                }
            }
        }
    }
    return FAIL; // Retorna falha se a chave não for encontrada
}


int rmLista(pweblist web, int chave, ppDDLL rmLista) {
    // Verifica se a WebList ou a chave são inválidas
    if (!web || chave < 0) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < web->node_count; i++) {
        // Verifica se o nó atual é um nó folha
        if (web->nodes[i].is_leaf) {
            // Percorre as 8 listas do nó
            for (int j = 0; j < 8; j++) {
                // Verifica se a chave corresponde
                if (web->nodes[i].lists[j].key == chave) {
                    // Remove a lista e libera a memória
                    *rmLista = web->nodes[i].lists[j].list;
                    web->nodes[i].lists[j].list = NULL; // Define como NULL após remoção
                    return SUCCESS; // Retorna sucesso
                }
            }
        }
    }
    return FAIL; // Retorna falha se a chave não for encontrada
}

int nvLista(pweblist web, int chave) {
    // Verifica se a WebList ou a chave são inválidas
    if (!web || chave < 0 ) return FAIL;

    // Percorre cada nó da WebList
    for (int i = 0; i < web->node_count; i++) {
        // Verifica se o nó atual é um nó folha
        if (web->nodes[i].is_leaf) {
            // Percorre as 8 listas do nó
            for (int j = 0; j < 8; j++) {
                // Verifica se a chave corresponde
                if (web->nodes[i].lists[j].key == chave) {
                    // Libera a lista antiga, se existir
                    if (web->nodes[i].lists[j].list) {
                        dDDLL(&(web->nodes[i].lists[j].list));
                    }
                    // Cria uma nova DDLL
                    if (cDDLL(&(web->nodes[i].lists[j].list), web->sizedata) == FAIL) {
                        return FAIL; // Retorna falha se a criação da nova lista falhar
                    }
                    return SUCCESS; // Retorna sucesso
                }
            }
        }
    }

    return FAIL; // Retorna falha se a chave não for encontrada
}


    // Funções da WebList
    int nroEleNoFolha(WebListNode web, int *retorno) {
        // traverse each list and count the number of elements
        //if (!web) return FAIL;
        *retorno = 0;
        for (int i = 0; i < 8; i++) {
            
            *retorno += countElements(web.lists[i].list);
        }
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
            int count;
            nroEleNoFolha(web->nodes[i], &count);
            *retorno += count;
        }
        return SUCCESS;
    }


   int lstChaves(pweblist web, ppDDLL retorno) {
    // Verifica se a WebList existe
    if (!web) return FAIL;

    // Insere as chaves na DDLL
    for (int i = 0; i < web->node_count; i++) {
        for (int j = 0; j < 8; j++) {
            // Verifica se a lista existe e se a chave não é nula
            if (web->nodes[i].lists[j].list != NULL) {
                // Insere a chave na DDLL
                if (iEnd(*retorno, &(web->nodes[i].lists[j].key)) == FAIL) {
                    //dDDLL(retorno); // Libera a DDLL em caso de falha
                    return FAIL; // Retorna FAIL se a inserção falhar
                }
            }
        }
    }

    return SUCCESS; // Retorna SUCCESS se todas as chaves foram inseridas
}



    int WLbalanceada(pweblist web) {
    if (!web) return FAIL;

    // Verifica o balanceamento em cada nó
    for (int i = 0; i < web->node_count; i++) {
        int min = INT_MAX; // Inicializa com o maior valor possível
        int max = INT_MIN; // Inicializa com o menor valor possível
        
        // Percorre cada lista no nó
        for (int j = 0; j < 8; j++) {
            int count = 0; // Contador de elementos na lista atual
            
            // Conta os elementos na lista
            if (web->nodes[i].lists[j].list) {
                count = countElements(web->nodes[i].lists[j].list);
            }

            // Atualiza min e max
            if (count < min) {
                min = count;
            }
            if (count > max) {
                max = count;
            }
        }

        // Verifica se a diferença entre max e min é maior que 1
        if (max - min > 1) {
            return FAIL; // O nó não está balanceado
        }
    }

    return SUCCESS; // Todos os nós estão balanceados
}


// Função de balanceamento da WebList
int balanceWebList(pweblist web) {
    if (!web) return FAIL;

    for (int i = 0; i < web->node_count; i++) {
        int min = INT_MAX; // Inicializa com o maior valor possível
        int max = INT_MIN; // Inicializa com o menor valor possível

        // Percorre cada lista no nó
        for (int j = 0; j < 8; j++) {
            int count = countElements(web->nodes[i].lists[j].list);
            if (count < min) {
                min = count;
            }
            if (count > max) {
                max = count;
            }
        }

        // Verifica se a diferença entre max e min é maior que 1
        if (max - min > 1) {
            // Redistribuir elementos entre as listas
            if (redistribuir(&web->nodes[i], web->sizedata) == FAIL) {
                return FAIL;
            }
        }
    }

    return SUCCESS; // Todos os nós estão balanceados
}

// Função para redistribuir elementos entre as listas do nó
int redistribuir(WebListNode *node, int sizedata) {
    if (!node || !node->is_leaf) return FAIL;

    // Calcula o total de elementos no nó
    int total_elements = 0;
    for (int i = 0; i < 8; i++) {
        total_elements += countElements(node->lists[i].list);
    }

    // Calcula a média de elementos por lista
    int avg_elements_per_list = total_elements / 8;

    // Redistribui elementos
    for (int i = 0; i < 8; i++) {
        while (countElements(node->lists[i].list) > avg_elements_per_list) {
            // Encontra a próxima lista que pode receber elementos
            int next_list = (i + 1) % 8;
            while (countElements(node->lists[next_list].list) >= avg_elements_per_list) {
                next_list = (next_list + 1) % 8; // Avança para a próxima lista
                if (next_list == i) {
                    return SUCCESS; // Se voltar à lista original, sai
                }
            }

            // Move o elemento do final da lista `i` para o início da lista `next_list`
            void *data_to_move = malloc(sizedata);
            if (!data_to_move) return FAIL;

            if (rEnd(node->lists[i].list, data_to_move) == FAIL) {
                free(data_to_move);
                return FAIL;
            }

            if (iBegin(node->lists[next_list].list, data_to_move) == FAIL) {
                free(data_to_move);
                return FAIL;
            }

            free(data_to_move);
        }
    }

    return SUCCESS;
}




    /*
    //FUNÇÕES DE BALANCEAMENTO ALTERNATIVAS
    // FUNÇÃO DE BALANCEAMENTO POR REDISTRIBUIÇÃO DE DADOS
    int redistributeBalance(pweblist web) {
        if (!web) return FAIL;

        // OBTÉM O TOTAL DE ELEMENTOS EM TODA A WEBLIST
        int total_elements = 0;
        int element;
        
        // CRIA UM BUFFER TEMPORÁRIO PARA ARMAZENAR TODOS OS DADOS
        int buffer[100]; // AJUSTE O TAMANHO CONFORME NECESSÁRIO
        int buffer_index = 0;

        for (int i = 0; i < web->node_count; i++) {
            int count = countElements(web->nodes[i].list);
            while (count > 0) {
                if (rBegin(web->nodes[i].list, &element) == SUCCESS) {
                    buffer[buffer_index++] = element;
                }
                count--;
            }
        }

        // CALCULA O NÚMERO MÉDIO DE ELEMENTOS QUE CADA NÓ DEVE TER
        int target_elements_per_node = buffer_index / web->node_count;

        // REDISTRIBUI OS ELEMENTOS ARMAZENADOS NO BUFFER PARA CADA NÓ
        buffer_index = 0;
        for (int i = 0; i < web->node_count; i++) {
            for (int j = 0; j < target_elements_per_node && buffer_index < 100; j++) {
                iBegin(web->nodes[i].list, &buffer[buffer_index++]);
            }
        }

        return SUCCESS;
    }

    // IMPLEMENTAÇÃO DA FUNÇÃO HYPOTÉTICA `calculateHeight`
    int calculateHeight(pweblist web, int node_index) {
        int height = 0;
        int current_index = node_index;

        // Em uma árvore octogonal, a altura aumenta conforme cada camada é preenchida
        while (current_index < web->node_count) {
            height++;
            current_index = current_index * 8 + 1;  // Avança para o próximo "filho" em uma estrutura octogonal
        }
        return height;
    }


    // FUNÇÃO DE BALANCEAMENTO POR ALTURA DA ÁRVORE
    int heightBasedBalance(pweblist web) {
        if (!web) return FAIL;

        int ideal_height = (int)log2(web->node_count) / log2(8) + 1;

        for (int i = 0; i < web->node_count; i++) {
            int height = calculateHeight(web, i);
            if (height > ideal_height) {
                redistributeBalance(web);  // Redistribuição quando a altura ultrapassa o ideal
                break;
            }
        }
        return SUCCESS;
    }

    // FUNÇÃO PARA EXIBIR A ESTRUTURA DA WEBLIST APÓS O BALANCEAMENTO
    void exibirWebList(pweblist web) {
        if (!web) return;

        printf("\nEstrutura da WebList:\n");
        for (int i = 0; i < web->node_count; i++) {
            printf("Nó %d (chave: %d):\n", i, web->nodes[i].key);

            if (web->nodes[i].list && !empty(web->nodes[i].list)) {
                int data;
                int position = 0;
                // Percorre e exibe todos os elementos na DDLL do nó
                while (sPosition(web->nodes[i].list, position, &data) == SUCCESS) {
                    printf("  Dado %d: %d\n", position, data);
                    position++;
                }
            } else {
                printf("  DDLL vazia.\n");
            }
        }
        printf("Fim da estrutura da WebList\n");
    }
    */