#ifndef ENEMY_LIST_H
#define ENEMY_LIST_H

#include "../Enemy.h"

typedef struct EnemyListElem
{
    Enemy* data;

    struct EnemyListElem* prev;
    struct EnemyListElem* next;  
} EnemyListElem;

void EnemyListElem_linkBefore(EnemyListElem* listElem, Enemy* element);
void EnemyListElem_linkAfter(EnemyListElem* listElem, Enemy* element);
void EnemyListElem_unlink(EnemyListElem* listElem);

typedef struct EnemyList
{
    EnemyListElem* head;
    int count;
} EnemyList;

EnemyList* EnemyList_construct();
void EnemyList_insertAtStart(EnemyList* list, Enemy* element);
void EnemyList_insertAtEnd(EnemyList* list, Enemy* element);
void EnemyList_insertBefore(EnemyList* list, EnemyListElem* listElem, Enemy* element);
void EnemyList_insertAfter(EnemyList* list, EnemyListElem* listElem, Enemy* element);
void EnemyList_remove(EnemyList* list, EnemyListElem* listElem);
void EnemyList_clear(EnemyList* list);
void EnemyList_delete(EnemyList* list);

#endif