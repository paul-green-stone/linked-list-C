#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* data;

    struct Node* next;
} Node;

#endif