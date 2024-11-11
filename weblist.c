    #include <stdio.h>    // Para printf
    #include <string.h>   // Para memcmp
    #include <stdlib.h>   // Para malloc e free
    #include "weblist_pub.h"
    #include "weblist_pri.h"
   

// Declaração de funções auxiliares para balanceamento, evitando declarações implícitas
    int balanceWebList(pweblist web);
    int balanceWebListAfterRemoval(pweblist web);

//Declaração das funções otimizadas (inserir no main se no weblist pub não puder)
int rDado_otimizada(pweblist web, void *dado);
int bDado_otimizada(pweblist web, void *dado);
int balanceWebList_otimizada(pweblist web);
int balanceWebListAfterRemoval_otimizada(pweblist web);


    //função para criar a weblist e inicializar cada nó com uma DDLL vazia
    int cWL(ppweblist web, int sizedata) {
        //aloca memória para a estrutura da weblist
        *web = (pweblist)malloc(sizeof(struct weblist));
        //verifica se deu certo a alocação, se não deu, falha
        if (!(*web)) return FAIL;

        //define o nível inicial da weblist como 0
        (*web)->level = 0;
        //define o número de nós folhas (nesse caso, 8)
        (*web)->node_count = 8;
        //aloca espaço para os nós
        (*web)->nodes = (WebListNode *)malloc(8 * sizeof(WebListNode));
        //verifica se a alocação para os nós falhou
        if (!(*web)->nodes) {
            //se falhar, libera memória da weblist
            free(*web);
            return FAIL;
        }

        //inicializa cada nó com uma chave e uma DDLL vazia
        for (int i = 0; i < (*web)->node_count; i++) {
            //define uma chave única para cada nó
            (*web)->nodes[i].key = i;
            //cria uma DDLL vazia para cada nó
            if (cDDLL(&((*web)->nodes[i].list), sizedata) == FAIL) {
                //se a criação da DDLL falhar, limpa todas as alocações e retorna erro
                for (int j = 0; j < i; j++) {
                    //destrói qualquer DDLL alocada previamente
                    dDDLL(&((*web)->nodes[j].list));
                }
                //libera a memória alocada para os nós
                free((*web)->nodes);
                //libera a estrutura principal da WebList
                free(*web);
                return FAIL;
            }

            printf("Nó %d criado com sucesso.\n", i);
        }

        //armazena o tamanho dos dados de cada nó
        (*web)->sizedata = sizedata;

        //retorna SUCCESS se a WebList foi criada corretamente
        return SUCCESS;
    }

    //funções de travessia de árvore para percorrer nós em pré-ordem, in-ordem e pós-ordem

    /// PERCURSO PRÉ-ORDEM COM VERIFICAÇÃO DE LIMITES
    void preOrderTraversal(pweblist web, int node_index) {
        //verifica se a WebList ou o índice do nó é válido.
        if (!web || node_index >= web->node_count) return;
        //imprime o nó atual
        printf("Visitando nó %d em pré-ordem (chave: %d)\n", node_index, web->nodes[node_index].key);
        
        // PERCORRE ATÉ 8 "FILHOS" POSSÍVEIS COM VERIFICAÇÃO DE LIMITES
        for (int i = 1; i <= 2 && node_index * 8 + i < web->node_count; i++) {
            //calcula o índice do filho
            int child_index = node_index * 8 + i;
            if (child_index < web->node_count) {
                //chama a função de forma recursiva para o filho
                preOrderTraversal(web, child_index);
            }
        }
    }

    // PERCURSO EM ORDEM COM VERIFICAÇÃO DE LIMITES
    void inOrderTraversal(pweblist web, int node_index) {
        //verifica se a WebList ou o índice do nó é válido
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
        //verifica se a WebList ou o índice do nó é válido
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

    //função para destruir a weblist, liberando toda a memória associada
    int dWL(pweblist *web) {
        //verifica se a WebList ou o ponteiro para a WebList são nulos
        if (!web || !(*web)) return FAIL;
        //percorre e destrói cada DDLL associada a cada nó da WebList
        for (int i = 0; i < (*web)->node_count; i++) {
            // se a lista do nó existir
            if ((*web)->nodes[i].list) {
                //destrói a DDLL do nó atual
                dDDLL(&((*web)->nodes[i].list));
                
            }
        }
        //libera a memória dos nós
        free((*web)->nodes);
        //libera a estrutura principal da weblist
        free(*web);
        //define o ponteiro como NULL para evitar acessos inválidos
        *web = NULL;

        return SUCCESS;
    }


    //função para inserir dado com verificação de balanceamento
    //distribui a carga entre os nós de forma cíclica,
    //em vez de inserir dados sempre no mesmo nó, os dados são inseridos de maneira rotativa em cada nó, um após o outro. 
    //Ajudando a "balancear" a carga de dados entre os nós, fazendo com que cada nó receba uma quantidade de dados mais ou menos igual ao longo do tempo.
    //Depois utiliza iBegin para inserir os dados e verifica o balanceamento após cada inserção
    int iDado(pweblist web, void *dado) {
        if (!web) return FAIL;  //verifica se a WebList é válida

        // Distribui a carga entre os nós
        //variável estática para rastrear o índice do nó
        static int node_index = 0;
        //avança o índice de forma cíclica para distribuir dados
        node_index = (node_index + 1) % web->node_count;

        //aloca memória para copiar o dado
        void *data_copy = malloc(web->sizedata);
        //verifica a alocação
        if (!data_copy) return FAIL;
        //copia o dado para o espaço alocado
        memcpy(data_copy, dado, web->sizedata);

        //insere o dado no início da DDLL
        int resultado = iBegin(web->nodes[node_index].list, data_copy);
        if (resultado == SUCCESS) {
            //verifica e ajusta o balanceamento após a inserção
            resultado = balanceWebList(web);
        }

        //libera a memória do dado temporário
        free(data_copy);
        //retorna o resultado da inserção
        return resultado;
    }


    //função para remover um dado específico da WebList
    int rDado(pweblist web, void *dado) {
        //verifica se a WebList ou o dado são válidos
        if (!web || !dado) return FAIL;

        // Percorre cada nó da WebList
        for (int i = 0; i < web->node_count; i++) {
            //se a lista existe, busca o dado
            if (web->nodes[i].list) {
                //aloca espaço para comparação
                void *temp_data = malloc(web->sizedata);
                if (!temp_data) return FAIL; //verifica a alocação

                //começa a buscar o dado na DDLL
                if (sBegin(web->nodes[i].list, temp_data) == SUCCESS) {
                    //compara o primeiro dado
                    if (memcmp(temp_data, dado, web->sizedata) == 0) {
                        //remove se for encontrado.
                        rBegin(web->nodes[i].list, temp_data);
                        free(temp_data); //libera a memória alocada
                        balanceWebListAfterRemoval(web); //balanceia a WebList após a remoção
                        return SUCCESS;
                    }

                    // Continua buscando o dado em outras posições
                    //variável para rastrear a posição
                    int pos = 0;
                    //continua buscando.
                    while (sPosition(web->nodes[i].list, ++pos, temp_data) == SUCCESS) {
                        if (memcmp(temp_data, dado, web->sizedata) == 0) {
                            //dado encontrado, remove-o
                            rPosition(web->nodes[i].list, pos, temp_data);
                            free(temp_data); //libera a memória alocada
                            balanceWebListAfterRemoval(web); //balanceia a WebList após a remoção
                            return SUCCESS;
                        }
                    }
                }
                free(temp_data); //libera a memória alocada se não encontrou
            }
        }
        return FAIL; //retorna falha se o dado não foi encontrado em nenhuma lista
    }


    //função para buscar dado específico na WebList
    int bDado(pweblist web, void *dado) {
        //verifica se a WebList ou o dado são válidos
        if (!web || !dado) return FAIL;
        //percorre cada nó da WebList
        for (int i = 0; i < web->node_count; i++) {
            //verifica se a DDLL do nó atual não é nula
            if (web->nodes[i].list) {
                //aloca memória para a comparação de dados
                void *temp_data = malloc(web->sizedata);
                //verifica se a alocação de memória foi bem-sucedida.
                if (!temp_data) return FAIL; 

                //pega o dado no início da DDLL
                if (sBegin(web->nodes[i].list, temp_data) == SUCCESS) {
                    //compara o dado buscado com o primeiro elemento
                    if (memcmp(temp_data, dado, web->sizedata) == 0) {
                        //libera a memória temporária
                        free(temp_data);
                        //retorna sucesso se o dado foi encontrado
                        return SUCCESS;
                    }
                    //variável para rastrear a posição durante a busca
                    int pos = 0;
                    //percorre repetidamente a DDLL buscando o elemento na posição desejada
                    while (sPosition(web->nodes[i].list, ++pos, temp_data) == SUCCESS) {
                        //compara o conteúdo de dois blocos de memória
                        if (memcmp(temp_data, dado, web->sizedata) == 0) {
                            //libera a memória temporária
                            free(temp_data);
                            //retorna sucesso se o dado foi encontrado
                            return SUCCESS;
                        }
                    }
                }
                //libera a memória se o dado não for encontrado na DDLL do nó
                free(temp_data);
            }
        }
        //retorna falha se o dado não foi encontrado em nenhum nó
        return FAIL;
    }


    // Função para percorrer e imprimir todos os dados na WebList
    int pLista(pweblist web, void (*printFunc)(void*)) {
        //verifica se a WebList e a função de impressão são válidas
        if (!web || !printFunc) return FAIL;
        //percorre cada nó da WebList para acessar a DDLL associada

        for (int i = 0; i < web->node_count; i++) {
            //imprime o índice do nó
            printf("Nó %d:\n", i); 
            //verifica se a DDLL está vazia
            if (empty(web->nodes[i].list) == SUCCESS) { 
                printf("  DDLL vazia.\n"); //informa que a lista associada ao nó está vazia
            } else {
                int pos = 0; //variável para rastrear a posição do elemento
                void *temp_data = malloc(web->sizedata); //aloca memória para armazenar os dados
                if (!temp_data) return FAIL; //retorna falha se a alocação de memória falhar

                //percorre e imprime cada dado na DDLL do nó usando a função fornecida
                while (sPosition(web->nodes[i].list, pos++, temp_data) == SUCCESS) {
                    printFunc(temp_data); //chama a função de impressão fornecida para o dado atual
                }
                free(temp_data); //libera a memória temporária após a impressão
            }
        }
    return SUCCESS; //retorna sucesso após percorrer e imprimir todos os nós
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

        *retorno = temp;

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

    /*
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
    }*/

    //Função de verificação de balanceamento que garante que a regra:
    // - a soma do numero de dados incluıdos nas DDLLs por no da arvore nao pode variar mais do que 1 elemento
    // seja cumprida
    int balanceWebList(pweblist web) {
    if (!web) return FAIL;

    // Calcula o total de elementos em todos os nós
    int total_elements = 0;
    for (int i = 0; i < web->node_count; i++) {
        total_elements += countElements(web->nodes[i].list);
    }

    // Calcula a quantidade ideal de elementos por nó
    int ideal_count = total_elements / web->node_count;
    int extra = total_elements % web->node_count; // Número de nós que devem ter 1 elemento extra

    // Redistribui os elementos para que cada nó tenha `ideal_count` ou `ideal_count + 1` elementos
    for (int i = 0; i < web->node_count; i++) {
        int current_count = countElements(web->nodes[i].list);
        while (current_count > ideal_count + (i < extra ? 1 : 0)) {
            // Encontra um nó com menos elementos para transferir
            int j = (i + 1) % web->node_count;
            while (countElements(web->nodes[j].list) >= ideal_count + (j < extra ? 1 : 0) && j != i) {
                j = (j + 1) % web->node_count;
            }

            if (j == i) break; // Todos os nós estão balanceados

            // Move o elemento do nó `i` para o nó `j`
            void *data_to_move = malloc(web->sizedata);
            if (!data_to_move) return FAIL;

            if (rEnd(web->nodes[i].list, data_to_move) == SUCCESS) {
                if (iBegin(web->nodes[j].list, data_to_move) == FAIL) {
                    free(data_to_move);
                    return FAIL;
                }
            }

            free(data_to_move);
            current_count = countElements(web->nodes[i].list); // Atualiza a contagem
        }
    }

    // Verifica novamente se a diferença entre o maior e o menor é no máximo 1 após redistribuição
    int min_elements = countElements(web->nodes[0].list);
    int max_elements = min_elements;
    for (int i = 1; i < web->node_count; i++) {
        int elements = countElements(web->nodes[i].list);
        if (elements < min_elements) min_elements = elements;
        if (elements > max_elements) max_elements = elements;
    }

    // Verifica se a condição de diferença máxima de 1 é atendida
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

    #include "weblist_pub.h"

// Função que realiza o balanceamento por redistribuição dos elementos entre os nós da WebList
int redistributeBalance(pweblist web) {
    if (!web) return FAIL;

    // Calcula o número total de elementos e a média ideal por nó
    int total_elements = 0;
    for (int i = 0; i < web->node_count; i++) {
        total_elements += countElements(web->nodes[i].list);
    }
    int ideal_count = total_elements / web->node_count;
    int extra = total_elements % web->node_count;

    // Redistribui elementos para que cada nó tenha `ideal_count` ou `ideal_count + 1` elementos
    for (int i = 0; i < web->node_count; i++) {
        while (countElements(web->nodes[i].list) > ideal_count + (i < extra ? 1 : 0)) {
            // Encontra um nó com menos elementos para transferir
            int j = (i + 1) % web->node_count;
            while (countElements(web->nodes[j].list) >= ideal_count + (j < extra ? 1 : 0) && j != i) {
                j = (j + 1) % web->node_count;
            }
            if (j == i) break;

            // Move o elemento do nó `i` para o nó `j`
            void *data_to_move = malloc(web->sizedata);
            if (!data_to_move) return FAIL;
            if (rEnd(web->nodes[i].list, data_to_move) == SUCCESS) {
                if (iBegin(web->nodes[j].list, data_to_move) == FAIL) {
                    free(data_to_move);
                    return FAIL;
                }
            }
            free(data_to_move);
        }
    }
    return SUCCESS;
}

// Função que realiza o balanceamento baseado na altura da WebList
int heightBasedBalance(pweblist web) {
    if (!web) return FAIL;

    // Assume altura mínima de um nó como a quantidade mínima de elementos em qualquer nó
    int min_height = countElements(web->nodes[0].list);
    int max_height = min_height;
    for (int i = 1; i < web->node_count; i++) {
        int elements = countElements(web->nodes[i].list);
        if (elements < min_height) min_height = elements;
        if (elements > max_height) max_height = elements;
    }

    // Realiza redistribuição apenas se a altura varia mais do que 1 entre os nós
    if (max_height - min_height > 1) {
        return redistributeBalance(web); // Usa o balanceamento por redistribuição como fallback
    }

    return SUCCESS; // Já está balanceada em altura
}


//FUNÇÕES OTIMIZADAS

#define HASH_TABLE_SIZE 8  // Define o tamanho da tabela hash para 8 entradas

// Estrutura para o índice de hash para localização rápida dos nós
typedef struct hash_entry {
    int key;            // Chave do nó na WebList
    pDDLL list;         // Ponteiro para a lista associada ao nó
    struct hash_entry* next; // Ponteiro para a próxima entrada na tabela hash (tratamento de colisão)
} hash_entry;

// Declaração da tabela hash global
hash_entry* hash_table[HASH_TABLE_SIZE]; // Tabela hash para acesso rápido aos nós

// Função de hash para calcular o índice na tabela com base na chave
int hash(int key) {
    return key % HASH_TABLE_SIZE; // Calcula o índice usando o operador de módulo
}

// Função para inserir um nó na tabela hash
void insert_into_hash(int key, pDDLL list) {
    int index = hash(key); // Calcula o índice na tabela hash
    hash_entry* new_entry = (hash_entry*)malloc(sizeof(hash_entry)); // Aloca memória para uma nova entrada
    new_entry->key = key;       // Define a chave da entrada
    new_entry->list = list;     // Associa a lista ao nó
    new_entry->next = hash_table[index]; // Insere a entrada na posição calculada
    hash_table[index] = new_entry; // Atualiza o índice da tabela hash com a nova entrada
}

// Função para buscar um nó na tabela hash
pDDLL search_in_hash(int key) {
    int index = hash(key);          // Calcula o índice na tabela hash
    hash_entry* entry = hash_table[index]; // Acessa a posição na tabela hash
    while (entry) {                   // Itera sobre as entradas (tratamento de colisão)
        if (entry->key == key)        // Verifica se a chave corresponde
            return entry->list;       // Retorna a lista associada ao nó
        entry = entry->next;          // Move para a próxima entrada em caso de colisão
    }
    return NULL;                      // Retorna NULL se o nó não for encontrado
}

// Função para limpar a tabela hash
void clear_hash() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) { // Percorre cada entrada na tabela hash
        hash_entry* entry = hash_table[i];      // Obtém a entrada na posição atual
        while (entry) {                         // Itera sobre as entradas encadeadas
            hash_entry* temp = entry;           // Armazena a entrada atual temporariamente
            entry = entry->next;                // Move para a próxima entrada
            free(temp);                         // Libera a entrada atual
        }
        hash_table[i] = NULL;                   // Define a posição na tabela hash como NULL
    }
}

// Função otimizada para remover um dado específico na WebList
int rDado_otimizada(pweblist web, void *dado) {
    if (!web || !dado) return FAIL;     // Verifica se a WebList e o dado são válidos

    // Percorre cada nó da WebList para encontrar e remover o dado
    for (int i = 0; i < web->node_count; i++) { // Itera sobre cada nó
        pDDLL list = search_in_hash(i);         // Busca a lista associada ao nó via hash
        if (list) {                             // Verifica se a lista existe
            void *temp_data = malloc(web->sizedata); // Aloca memória para comparação de dados
            if (!temp_data) return FAIL;              // Verifica a alocação de memória

            // Itera sobre a lista buscando o dado a partir do início
            int pos = 0;                       // Inicializa a posição de busca
            if (sBegin(list, temp_data) == SUCCESS) { // Move para o início da lista
                do {
                    if (memcmp(temp_data, dado, web->sizedata) == 0) { // Compara os dados
                        if (rPosition(list, pos, temp_data) == SUCCESS) { // Remove o dado da posição
                            free(temp_data);                // Libera a memória temporária
                            balanceWebListAfterRemoval_otimizada(web); // Balanceia após remoção
                            return SUCCESS;                 // Retorna sucesso se o dado foi removido
                        } else {
                            free(temp_data);                // Libera a memória em caso de erro
                            return FAIL;                    // Falha ao remover
                        }
                    }
                    pos++; // Incrementa a posição
                } while (sPosition(list, pos, temp_data) == SUCCESS); // Continua até o fim da lista
            }
            free(temp_data);                         // Libera a memória temporária se o dado não foi encontrado
        }
    }
    return FAIL;                                     // Retorna falha se o dado não foi encontrado
}



// Função otimizada para busca de um dado específico
int bDado_otimizada(pweblist web, void *dado) {
    if (!web || !dado) return FAIL;      // Verifica se a WebList e o dado são válidos

    // Usa a tabela hash para busca rápida
    for (int i = 0; i < web->node_count; i++) { // Percorre cada nó da WebList
        pDDLL list = search_in_hash(i);         // Busca a lista associada ao nó via hash
        if (list) {                             // Verifica se a lista existe
            void *temp_data = malloc(web->sizedata); // Aloca memória para comparação de dados
            if (!temp_data) return FAIL;              // Verifica a alocação de memória

            if (sBegin(list, temp_data) == SUCCESS) { // Inicia a busca pelo dado no começo da lista
                if (memcmp(temp_data, dado, web->sizedata) == 0) { // Compara os dados
                    free(temp_data);              // Libera a memória temporária
                    return SUCCESS;               // Retorna sucesso se o dado foi encontrado
                }
                int pos = 0;                      // Inicializa a posição para busca adicional
                while (sPosition(list, ++pos, temp_data) == SUCCESS) { // Continua buscando na lista
                    if (memcmp(temp_data, dado, web->sizedata) == 0) { // Compara dados na posição atual
                        free(temp_data);            // Libera a memória temporária
                        return SUCCESS;             // Retorna sucesso se o dado foi encontrado
                    }
                }
            }
            free(temp_data);                    // Libera a memória temporária se o dado não foi encontrado
        }
    }
    return FAIL;                               // Retorna falha se o dado não foi encontrado
}

// Função de balanceamento otimizado em lote
int balanceWebList_otimizada(pweblist web) {
    if (!web) return FAIL;          // Verifica se a WebList é válida

    int total_elements = 0;         // Inicializa a contagem total de elementos
    for (int i = 0; i < web->node_count; i++) { // Conta o total de elementos em todos os nós
        total_elements += countElements(web->nodes[i].list); // Soma os elementos do nó atual
    }

    // Calcula o número ideal de elementos por nó
    int ideal_count = total_elements / web->node_count; // Determina o número ideal de elementos por nó
    int extra = total_elements % web->node_count; // Calcula o número de nós que precisam de um elemento extra

    // Redistribui os elementos em lote para balanceamento
    for (int i = 0; i < web->node_count; i++) { // Percorre cada nó para balanceamento
        int current_count = countElements(web->nodes[i].list); // Conta os elementos no nó atual
        while (current_count > ideal_count + (i < extra ? 1 : 0)) { // Enquanto o nó tiver mais que o ideal
            int j = (i + 1) % web->node_count; // Encontra o próximo nó para redistribuição
            while (countElements(web->nodes[j].list) >= ideal_count + (j < extra ? 1 : 0) && j != i) {
                j = (j + 1) % web->node_count; // Continua buscando um nó com menos elementos
            }
            if (j == i) break; // Sai do loop se não houver nós para redistribuir

            void *data_to_move = malloc(web->sizedata); // Aloca memória para mover dados
            if (!data_to_move) return FAIL;             // Verifica a alocação de memória

            if (rEnd(web->nodes[i].list, data_to_move) == SUCCESS) { // Remove o dado do nó atual
                if (iBegin(web->nodes[j].list, data_to_move) == FAIL) { // Insere o dado no próximo nó
                    free(data_to_move);       // Libera a memória se a inserção falhar
                    return FAIL;              // Retorna falha se não conseguir inserir
                }
            }
            free(data_to_move);               // Libera a memória temporária após mover
            current_count = countElements(web->nodes[i].list); // Atualiza a contagem do nó atual
        }
    }
    return SUCCESS;                           // Retorna sucesso após balanceamento
}

// Função de balanceamento após remoção em lote
int balanceWebListAfterRemoval_otimizada(pweblist web) {
    if (!web) return FAIL; // Verifica se a WebList é válida

    int total_elements = 0; // Inicializa a contagem total de elementos
    for (int i = 0; i < web->node_count; i++) { // Conta o total de elementos em todos os nós
        total_elements += countElements(web->nodes[i].list); // Soma os elementos do nó atual
    }
    int avg_elements_per_node = total_elements / web->node_count; // Determina o número médio por nó

    for (int i = 0; i < web->node_count; i++) { // Percorre cada nó para balanceamento
        while (countElements(web->nodes[i].list) > avg_elements_per_node) { // Enquanto o nó tiver mais que a média
            int j = (i + 1) % web->node_count; // Encontra o próximo nó para redistribuição
            while (countElements(web->nodes[j].list) >= avg_elements_per_node && j != i) {
                j = (j + 1) % web->node_count; // Continua buscando um nó com menos elementos
            }
            if (j == i) break; // Sai do loop se não houver nós para redistribuir

            void *data_to_move = malloc(web->sizedata); // Aloca memória para mover dados
            if (!data_to_move) return FAIL;             // Verifica a alocação de memória

            if (rEnd(web->nodes[i].list, data_to_move) == FAIL) { // Remove o dado do nó atual
                free(data_to_move);            // Libera a memória se a remoção falhar
                return FAIL;                   // Retorna falha se não conseguir remover
            }
            if (iBegin(web->nodes[j].list, data_to_move) == FAIL) { // Insere o dado no próximo nó
                free(data_to_move);            // Libera a memória se a inserção falhar
                return FAIL;                   // Retorna falha se não conseguir inserir
            }
            free(data_to_move);                // Libera a memória temporária após mover
        }
    }
    return SUCCESS;                            // Retorna sucesso após balanceamento
}