#include "main.h"
#include <stdlib.h>

void printInt(void *data) {
    printf("  Dado: %d\n", *(int*)data);
}

void printDouble(void *data) {
    printf("  Dado: %.1f\n", *(double*)data);
}

void printString(void *data) {
    printf("  Dado: %s\n", *(char**)data);
}

int main() {
    pweblist webInt;
    pweblist webDouble;
    pweblist webString;

    
    if (cWL(&webInt, sizeof(int)) == SUCCESS) {
        printf("Testing integer WebList:\n");

        int intValues[] = {1, 2, 3, 4, 5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
        for (int i = 0; i < 30; i++) {
            if (iDado(webInt, &intValues[i]) == FAIL) {
                printf("Failed to insert integer: %d\n", intValues[i]);
            }
            else {
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
        
        dWL(&webInt);
    }

    
    if (cWL(&webDouble, sizeof(double)) == SUCCESS) {
        printf("\nTesting double WebList:\n");

        double doubleValues[] = {1.1, 2.2, 3.3};
        for (int i = 0; i < 3; i++) {
            if (iDado(webDouble, &doubleValues[i]) == FAIL) {
                printf("Failed to insert double: %.1f\n", doubleValues[i]);
            }
            else {
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
        
        dWL(&webDouble);
    }

    
    if (cWL(&webString, sizeof(char*)) == SUCCESS) {
        printf("\nTesting string WebList:\n");

        char *stringValues[] = {"Hello", "World", "Test"};
        for (int i = 0; i < 3; i++) {
            if (iDado(webString, &stringValues[i]) == FAIL) {
                printf("Failed to insert string: %s\n", stringValues[i]);
            }
            else {
                printf("Inserted string: %s\n", stringValues[i]);
            }
        }

        char *searchString = "World";
        if (bDado(webString, &searchString) == SUCCESS) {
            printf("Found string: %s\n", searchString);
        } else {
            printf("String not found: %s\n", searchString);
        }

        pLista(webString, printString);
        
        dWL(&webString);
    }

    
    printf("\nTesting empty WebList:\n");
    pweblist emptyWeb;
    if (cWL(&emptyWeb, sizeof(int)) == SUCCESS) {
        pLista(emptyWeb, printInt);
        dWL(&emptyWeb);
    }

    return 0;
}
