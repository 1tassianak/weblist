#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "DDLL_pub.h"

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct DDLL {
    Node *head;
    Node *tail;
    int size;
    int sizedata;
} DDLL;