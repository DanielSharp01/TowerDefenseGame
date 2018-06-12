#ifndef ASTAR_H
#define ASTAR_H

#include "../Lists/NodeList.h"
#include "../Lists/Vector2List.h"

typedef struct AStarData
{
    NodeList* openList;
    int cols;
    int rows;
    Node*** nodes;
    Node* start;
    Node* goal;
} AStarData;

AStarData* AStarData_construct(int cols, int rows);
void AStarData_delete(AStarData* data);

Vector2List* AStar_FindPath(AStarData* data);

#endif