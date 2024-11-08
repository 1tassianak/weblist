    #include <stdio.h>    // Para printf
    #include <string.h>   // Para memcmp
    #include <stdlib.h>   // Para malloc e free
    #include "weblist_pub.h"
    #include "weblist_pri.h"
    #include <math.h> // INCLUI <math.h> PARA USO DA FUNÇÃO log2

    // Declaração da função balanceWebList para evitar declaração implícita
    int balanceWebList(pweblist web);

    //função para criar a weblist e inicializar cada nó com uma DDLL vazia
    int cWL(ppweblist web, int sizedata) {
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

            printf("Nó %d criado com sucesso.\n", i);
        }

        (*web)->sizedata = sizedata;
        return SUCCESS;
    }

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

    //função para destruir a weblist
    int dWL(pweblist *web) {
        //verifica se a weblist existe, se não existe, falha
        if (!web || !(*web)) return FAIL;
        //percorre todos os nós da weblist
        for (int i = 0; i < (*web)->node_count; i++) {
            // se a lista do nó existir
            
            if ((*web)->nodes[i].list) {
                //destrói a lista
                dDDLL(&((*web)->nodes[i].list));
                
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
    //distribui a carga entre os nós de forma cíclica,
    //em vez de inserir dados sempre no mesmo nó, os dados são inseridos de maneira rotativa em cada nó, um após o outro. 
    //Ajudando a "balancear" a carga de dados entre os nós, fazendo com que cada nó receba uma quantidade de dados mais ou menos igual ao longo do tempo.
    //Depois utiliza iBegin para inserir os dados e verifica o balanceamento após cada inserção
    int iDado(pweblist web, void *dado) {
        if (!web) return FAIL;  // Verifica se a WebList existe

        // Distribui a carga entre os nós
        static int node_index = 0;
        node_index = (node_index + 1) % web->node_count;

        void *data_copy = malloc(web->sizedata);
        if (!data_copy) return FAIL;
        memcpy(data_copy, dado, web->sizedata);

        // Insere o dado no início da DDLL do nó especificado
        int resultado = iBegin(web->nodes[node_index].list, data_copy);
        if (resultado == SUCCESS) {
            // Verifica e ajusta o balanceamento após a inserção
            resultado = balanceWebList(web);
        }

        free(data_copy);
        
        return resultado;
    }


    // Função para remover dado da WebList
    int rDado(pweblist web, void *dado) {
        if (!web || !dado) return FAIL;

        // Percorre cada nó da WebList
        for (int i = 0; i < web->node_count; i++) {
            if (web->nodes[i].list) {
                // Tenta remover o dado da DDLL do nó atual
                void *temp_data = malloc(web->sizedata);
                if (!temp_data) return FAIL; // Verifica se a alocação foi bem-sucedida

                // Começa a buscar o dado na DDLL
                if (sBegin(web->nodes[i].list, temp_data) == SUCCESS) {
                    // Verifica se o primeiro elemento é o que queremos remover
                    if (memcmp(temp_data, dado, web->sizedata) == 0) {
                        // Chama a função para remover o primeiro elemento
                        rBegin(web->nodes[i].list, temp_data);
                        free(temp_data); // Libera a memória alocada
                        balanceWebListAfterRemoval(web); // Balanceia a WebList após a remoção
                        return SUCCESS; // Retorna sucesso
                    }

                    // Continua buscando o dado em outras posições
                    int pos = 0;
                    while (sPosition(web->nodes[i].list, ++pos, temp_data) == SUCCESS) {
                        if (memcmp(temp_data, dado, web->sizedata) == 0) {
                            // Dado encontrado, remove-o
                            rPosition(web->nodes[i].list, pos, temp_data);
                            free(temp_data); // Libera a memória alocada
                            balanceWebListAfterRemoval(web); // Balanceia a WebList após a remoção
                            return SUCCESS; // Retorna sucesso
                        }
                    }
                }
                free(temp_data); // Libera a memória alocada se não encontrou
            }
        }
        return FAIL; // Retorna falha se o dado não foi encontrado em nenhuma lista
    }


    // Função para buscar dado na WebList
    int bDado(pweblist web, void *dado) {
        if (!web || !dado) return FAIL;

        for (int i = 0; i < web->node_count; i++) {
            if (web->nodes[i].list) {
                void *temp_data = malloc(web->sizedata);
                int pos = 0;

                if (sBegin(web->nodes[i].list, temp_data) == SUCCESS) {
                    if (memcmp(temp_data, dado, web->sizedata) == 0) {
                        free(temp_data);
                        return SUCCESS;
                    }

                    while (sPosition(web->nodes[i].list, ++pos, temp_data) == SUCCESS) {
                        if (memcmp(temp_data, dado, web->sizedata) == 0) {
                            free(temp_data);
                            return SUCCESS;
                        }
                    }
                }
                free(temp_data);
            }
        }
        return FAIL;
    }


    // Função para percorrer e exibir a lista de dados na WebList
    int pLista(pweblist web, void (*printFunc)(void*)) {
        if (!web) return FAIL;

        for (int i = 0; i < web->node_count; i++) {
            printf("Nó %d:\n", i);
            if (web->nodes[i].list) {
                void *temp_data = malloc(web->sizedata);
                int pos = 0;

                if (sBegin(web->nodes[i].list, temp_data) == SUCCESS) {
                    printFunc(temp_data);

                    while (sPosition(web->nodes[i].list, ++pos, temp_data) == SUCCESS) {
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
        return cDDLL(&(web->nodes[chave].list), web->sizedata);
    }

    // Funções da WebList
    int nroEleNoFolha(pweblist web, int *retorno) {
        if (!web) return FAIL;
        int temp = 0;
        for (int i = 0; i < web->node_count; i++) {
            temp += countElements(web->nodes[i].list);
        }

        *retorno = temp;

        return SUCCESS;
    }

    int nroNoFolha(pweblist web, int *retorno) {
        if (!web) return FAIL;
        *retorno = web->node_count;
        return SUCCESS;
    }

    int nroEleWL(pweblist web, int *retorno) {
        if (!web) return FAIL;
        int temp = 0;
        for (int i = 0; i < web->node_count; i++) {
            temp += countElements(web->nodes[i].list);
        }

        return SUCCESS;
    }


    // Função para retornar uma lista de chaves dos nós da WebList
    int lstChaves(pweblist web, ppDDLL retorno) {
        if (!web) return FAIL; // Retorna FAIL se a WebList não existe

        // Cria uma nova DDLL para armazenar as chaves
        if (cDDLL(retorno, web->sizedata) == FAIL) {
            return FAIL; // Retorna FAIL se a criação da DDLL falhar
        }

        // Percorre cada nó da WebList e adiciona a chave à DDLL
        for (int i = 0; i < web->node_count; i++) {
            int key = web->nodes[i].key; // Obtém a chave do nó atual

            // Insere a chave na DDLL de retorno
            if (iEnd(*retorno, &key) == FAIL) {
                dDDLL(retorno); // Limpa a DDLL em caso de falha
                return FAIL;
            }
        }

        return SUCCESS; // Retorna SUCCESS após adicionar todas as chaves
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

    int balanceWebListAfterRemoval(pweblist web) {
        if (!web) return FAIL;

        // Calcula o total de elementos em todos os nós
        int total_elements = 0;
        for (int i = 0; i < web->node_count; i++) {
            total_elements += countElements(web->nodes[i].list);
        }

        // Calcula a média de elementos por nó (arredondada para baixo)
        int avg_elements_per_node = total_elements / web->node_count;

        printf("Média de elementos por nó: %d\n", avg_elements_per_node);

        // Redistribui os elementos para preencher nós vazios
        for (int i = 0; i < web->node_count; i++) {
            while (countElements(web->nodes[i].list) > avg_elements_per_node) {
                // Encontra o próximo nó vazio ou com menos que a média
                int j = (i + 1) % web->node_count;
                while (countElements(web->nodes[j].list) >= avg_elements_per_node && j != i) {
                    j = (j + 1) % web->node_count;
                }

                if (j == i) break; // Não há mais nós para redistribuir

                // Move o elemento do final do nó `i` para o início do nó `j`
                void *data_to_move = malloc(web->sizedata);
                if (!data_to_move) return FAIL;

                if (rEnd(web->nodes[i].list, data_to_move) == FAIL) {
                    free(data_to_move);
                    return FAIL;
                }

                if (iBegin(web->nodes[j].list, data_to_move) == FAIL) {
                    free(data_to_move);
                    return FAIL;
                }

                free(data_to_move);
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