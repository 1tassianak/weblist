#include "main.h"
#include <stdlib.h>

// Funções de impressão
void printInt(void *data) {
    printf("  Dado: %d\n", *(int*)data);
}

void printDouble(void *data) {
    printf("  Dado: %.1f\n", *(double*)data);
}

void printString(void *data) {
    printf("  Dado: %s\n", *(char**)data);
}

// Função para testar a WebList de inteiros com mais cenários
void testIntegerWebList() {
    pweblist webInt;
    if (cWL(&webInt, sizeof(int)) == SUCCESS) {
        printf("Testing integer WebList:\n");

        int intValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        for (int i = 0; i < 15; i++) {
            if (iDado(webInt, &intValues[i]) == FAIL) {
                printf("Failed to insert integer: %d\n", intValues[i]);
            } else {
                printf("Inserted integer: %d\n", intValues[i]);
            }
        }

        int searchInt = 3;
        if (bDado(webInt, &searchInt) == SUCCESS) {
            printf("Found integer: %d\n", searchInt);
        } else {
            printf("Integer not found: %d\n", searchInt);
        }

        pLista(webInt, printInt);

        // Remoção de elementos e balanceamento
        printf("Removing integer 5...\n");
        if (rDado(webInt, &intValues[4]) == SUCCESS) {
            printf("Removed integer 5 successfully.\n");
        } else {
            printf("Failed to remove integer 5.\n");
        }

        printf("Removing integer 13...\n");
        if (rDado(webInt, &intValues[12]) == SUCCESS) {
            printf("Removed integer 13 successfully.\n");
        } else {
            printf("Failed to remove integer 13.\n");
        }

        if (WLbalanceada(webInt) == SUCCESS) {
            printf("WebList is balanced after removal.\n");
        } else {
            printf("WebList is not balanced after removal.\n");
        }

        pLista(webInt, printInt);

        // Testes de borda
        int nonExistentValue = 99;
        printf("Attempting to remove non-existent integer 99...\n");
        if (rDado(webInt, &nonExistentValue) == FAIL) {
            printf("Confirmed failure when trying to remove non-existent integer.\n");
        }

        printf("Attempting to insert duplicate integer 8...\n");
        if (iDado(webInt, &intValues[7]) == SUCCESS) {
            printf("Inserted duplicate integer 8 successfully (depends on implementation if allowed).\n");
        } else {
            printf("Failed to insert duplicate integer 8.\n");
        }

        dWL(&webInt); // Liberação da memória da WebList de inteiros
    }
}

// Função para testar a WebList de doubles com mais cenários
void testDoubleWebList() {
    pweblist webDouble;
    if (cWL(&webDouble, sizeof(double)) == SUCCESS) {
        printf("\nTesting double WebList:\n");

        double doubleValues[] = {1.1, 2.2, 3.3};
        for (int i = 0; i < 3; i++) {
            if (iDado(webDouble, &doubleValues[i]) == FAIL) {
                printf("Failed to insert double: %.1f\n", doubleValues[i]);
            } else {
                printf("Inserted double: %.1f\n", doubleValues[i]);
            }
        }

        double searchDouble = 2.2;
        if (bDado(webDouble, &searchDouble) == SUCCESS) {
            printf("Found double: %.1f\n", searchDouble);
        } else {
            printf("Double not found: %.1f\n", searchDouble);
        }

        pLista(webDouble, printDouble);

        // Teste de borda: Remoção de um double inexistente
        double nonExistentDouble = 4.4;
        printf("Attempting to remove non-existent double 4.4...\n");
        if (rDado(webDouble, &nonExistentDouble) == FAIL) {
            printf("Confirmed failure when trying to remove non-existent double.\n");
        }

        dWL(&webDouble); // Liberação da memória da WebList de doubles
    }
}

// Função para testar a WebList de strings com mais cenários
void testStringWebList() {
    pweblist webString;
    if (cWL(&webString, sizeof(char*)) == SUCCESS) {
        printf("\nTesting string WebList:\n");

        char *stringValues[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
        for (int i = 0; i < 26; i++) {
            if (iDado(webString, &stringValues[i]) == FAIL) {
                printf("Failed to insert string: %s\n", stringValues[i]);
            } else {
                printf("Inserted string: %s\n", stringValues[i]);
            }
        }

        char *searchString = "Z";
        if (bDado(webString, &searchString) == SUCCESS) {
            printf("Found string: %s\n", searchString);
        } else {
            printf("String not found: %s\n", searchString);
        }

        pLista(webString, printString);

        // Remoção de strings e verificação de balanceamento
        printf("Removing string 'A'...\n");
        if (rDado(webString, &stringValues[0]) == SUCCESS) {
            printf("Removed string 'A' successfully.\n");
        } else {
            printf("Failed to remove string 'A'.\n");
        }

        printf("Removing string 'D'...\n");
        if (rDado(webString, &stringValues[3]) == SUCCESS) {
            printf("Removed string 'D' successfully.\n");
        } else {
            printf("Failed to remove string 'D'.\n");
        }

        printf("Removing string 'Z'...\n");
        if (rDado(webString, &stringValues[25]) == SUCCESS) {
            printf("Removed string 'Z' successfully.\n");
        } else {
            printf("Failed to remove string 'Z'.\n");
        }

        if (WLbalanceada(webString) == SUCCESS) {
            printf("WebList is balanced after removal.\n");
        } else {
            printf("WebList is not balanced after removal.\n");
        }

        pLista(webString, printString);
        
        dWL(&webString); // Liberação da memória da WebList de strings
    }
}

// Função para testar uma WebList vazia
void testEmptyWebList() {
    printf("\nTesting empty WebList:\n");
    pweblist emptyWeb;
    if (cWL(&emptyWeb, sizeof(int)) == SUCCESS) {
        pLista(emptyWeb, printInt);

        // Teste de remoção em lista vazia
        int nonExistentValue = 0;
        printf("Attempting to remove integer from empty list...\n");
        if (rDado(emptyWeb, &nonExistentValue) == FAIL) {
            printf("Confirmed failure when trying to remove integer from empty list.\n");
        }

        dWL(&emptyWeb);
    }
}

// Função para testar uma WebList não inicializada (NULL pointer)
void testNullWebList() {
    printf("\nTesting null WebList:\n");
    pweblist nullWeb = NULL;

    // Tentativa de operações em uma WebList não inicializada
    int value = 10;
    printf("Attempting to insert into null WebList...\n");
    if (iDado(nullWeb, &value) == FAIL) {
        printf("Confirmed failure when trying to insert into null WebList.\n");
    }

    printf("Attempting to remove from null WebList...\n");
    if (rDado(nullWeb, &value) == FAIL) {
        printf("Confirmed failure when trying to remove from null WebList.\n");
    }

    printf("Attempting to destroy null WebList...\n");
    if (dWL(&nullWeb) == FAIL) {
        printf("Confirmed failure when trying to destroy null WebList.\n");
    }
}

int main() {
    testIntegerWebList();
    testDoubleWebList();
    testStringWebList();
    testEmptyWebList();
    testNullWebList();

    return 0;
}
