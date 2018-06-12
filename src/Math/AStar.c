#include "AStar.h"
#include <math.h>
#include <malloc.h>
#include "../Utils/MuleArray.h"
#include "Vector2.h"

void openNode(Node* parent, Node* node, AStarData* data)
{
    if (!node->walkable) return;
    
    if (node->parent != NULL)
    {
        if (node->G > parent->G + 1)
        {
            node->parent = parent;
            node->G = parent->G + 1;
            node->F = node->G + node->H;
        }
    }
    else if (!node->closed)
    {
        node->parent = parent;
        node->G = parent->G + 1;
        node->F = node->G + node->H;
        NodeList_insertAtEnd(data->openList, node);
    }
}

void openAroundNode(Node* parent, AStarData* data)
{
    if (parent->x - 1 >= 0) openNode(parent, data->nodes[parent->x-1][parent->y], data);
    if (parent->x + 1 < data->cols) openNode(parent, data->nodes[parent->x+1][parent->y], data);
    if (parent->y - 1 >= 0) openNode(parent, data->nodes[parent->x][parent->y-1], data);
    if (parent->y + 1 < data->rows) openNode(parent, data->nodes[parent->x][parent->y+1], data);
}

int nodeDistance(Node* a, Node* b)
{
    return abs(a->x - b->x) + abs(a->y - b->y);
}

void setupNodes(AStarData* data)
{
    int x, y;
    for (x = 0; x < data->cols; x++)
    {
        for (y = 0; y < data->rows; y++)
        {
            data->nodes[x][y]->H = nodeDistance(data->nodes[x][y], data->goal);
            data->nodes[x][y]->parent = NULL;
            data->nodes[x][y]->closed = false;
        }
    }   
}

NodeListElem* findBestNode(AStarData* data)
{
    NodeListElem* min = NULL;
    NodeListElem* iter;
    for (iter = data->openList->head->next; iter != data->openList->head; iter = iter->next)
    {
        if (min == NULL || min->data->F > iter->data->F)
        {
            min = iter;
        }
    }
    
    return min;
}

Vector2List* AStar_FindPath(AStarData* data)
{
    setupNodes(data);
    NodeList_clear(data->openList);
    Node* current = data->start;
    while (current != NULL && current != data->goal)
    {
        openAroundNode(current, data);
        current->closed = true;
        NodeListElem* minElem = findBestNode(data);
        current = minElem == NULL ? NULL : minElem->data;
        if (minElem != NULL)
        {
            NodeList_remove(data->openList, minElem);
        }
    }

    if (current == NULL) return NULL;
    Vector2List* ret = Vector2List_construct();
    while (current != NULL)
    {
        Vector2List_insertAtStart(ret, Vector2_construct(current->x * 128 + 64, current->y * 128 + 64));
        current = current->parent;
    }

    return ret;
}

AStarData* AStarData_construct(int cols, int rows)
{
    AStarData* data = (AStarData*)malloc(sizeof(AStarData));
    data->cols = cols;
    data->rows = rows;
    data->openList = NodeList_construct();
    data->nodes = (Node***)MuleArray_create2DPointer(data->cols, data->rows);
    int x, y;
    for (x = 0; x < data->cols; x++)
    {
        for (y = 0; y < data->rows; y++)
        {
            data->nodes[x][y] = (Node*)malloc(sizeof(Node));
            data->nodes[x][y]->x = x;
            data->nodes[x][y]->y = y;
            data->nodes[x][y]->walkable = false;
        }
    }

    return data;
}

void AStarData_delete(AStarData* data)
{
    int x, y;
    for (x = 0; x < data->cols; x++)
    {
        for (y = 0; y < data->rows; y++)
        {
            free(data->nodes[x][y]);
        }
    }
    MuleArray_delete2DPointer((void***)data->nodes);
    NodeList_delete(data->openList);
    free(data);
}