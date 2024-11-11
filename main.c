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

    // Testando WebList de inteiros
    if (cWL(&webInt, 0, sizeof(int)) == SUCCESS) {
        printf("Testing integer WebList:\n");

        int intValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
        for (int i = 0; i < 30; i++) {
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
        
        int key = 2;
        pDDLL ddll = NULL;

        if (cpLista(webInt, key, &ddll) == SUCCESS) {
            printf("Copied list from key %d:\n", key);
            int count = countElements(ddll);
            printf("  Number of elements: %d\n", count);
        } else {
            printf("Failed to copy list from key %d\n", key);
        }
    
        pDDLL ptr_rmLista = NULL;
        
        if (rmLista(webInt, key, &ptr_rmLista) == SUCCESS) {
            printf("Removed list from key %d:\n", key);
            int count = countElements(ptr_rmLista);
            printf("  Number of elements: %d\n", count);
        } else {
            printf("Failed to remove list from key %d\n", key);
        }
    
        if (nvLista(webInt, key) == SUCCESS) {
            printf("Created new list for key %d\n", key);
        } else {
            printf("Failed to create new list for key %d\n", key);
        }

        int eleCount;
        if (nroEleNoFolha(webInt->nodes[0], &eleCount) == SUCCESS) {
            printf("Number of elements in leaf node: %d\n", eleCount);
        } else {
            printf("Failed to get number of elements in leaf node\n");
        }

        int leafCount;
        if (nroNoFolha(webInt, &leafCount) == SUCCESS) {
            printf("Number of leaf nodes: %d\n", leafCount);
        } else {
            printf("Failed to get number of leaf nodes\n");
        }

        int totalEleCount;
        if (nroEleWL(webInt, &totalEleCount) == SUCCESS) {
            printf("Total number of elements: %d\n", totalEleCount);
        } else {
            printf("Failed to get total number of elements\n");
        }
        
        pDDLL keyList;
        if (cDDLL(&keyList, sizeof(int)) == FAIL) {
            printf("Failed to create DDLL for keys\n");
            dWL(&webInt);
            return -1;
        }

        if (lstChaves(webInt, &keyList) == SUCCESS) {
            printf("List of keys:\n");
            int keyData;
            int position = 0;
            while (sPosition(keyList, position, &keyData) == SUCCESS) {
                printf("  Key %d: %d\n", position, keyData);
                position++;
            }
        } else {
            printf("Failed to get list of keys\n");
        }

        // remove 25 and 17 and insert 31 and then print the weblist again
        int removeInt = 25;
        if (rDado(webInt, &removeInt) == SUCCESS) {
            printf("Removed integer: %d\n", removeInt);
        } else {
            printf("Failed to remove integer: %d\n", removeInt);
        }

        removeInt = 17;
        if (rDado(webInt, &removeInt) == SUCCESS) {
            printf("Removed integer: %d\n", removeInt);
        } else {
            printf("Failed to remove integer: %d\n", removeInt);
        }

        int newInt = 31;
        if (iDado(webInt, &newInt) == SUCCESS) {
            printf("Inserted integer: %d\n", newInt);
        } else {
            printf("Failed to insert integer: %d\n", newInt);
        }

        // balanced?
        if (WLbalanceada(webInt) == SUCCESS) {
            printf("WebList is balanced\n");
        } else {
            printf("WebList is not balanced\n");
        }

        pLista(webInt, printInt);
        
        dDDLL(&keyList);
        dDDLL(&ptr_rmLista);
        dWL(&webInt);
    }

    return 0;
}
