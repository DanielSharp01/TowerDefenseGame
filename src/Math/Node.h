#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

typedef struct Node
{
    int x, y;
    struct Node* parent;
    int H;
    int G;
    int F;
    bool walkable;
    bool closed;
} Node;

#endif