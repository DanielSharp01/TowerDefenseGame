#include "EnemyList.h"

#include <malloc.h>

void EnemyListElem_linkBefore(EnemyListElem* listElem, Enemy* element)
{
    EnemyListElem* newElem = (EnemyListElem*)malloc(sizeof(EnemyListElem));
    newElem->data = element;
    newElem->prev = listElem->prev;
    newElem->next = listElem;
    listElem->prev->next = newElem;
    listElem->prev = newElem;
}

void EnemyListElem_linkAfter(EnemyListElem* listElem, Enemy* element)
{
    EnemyListElem* newElem = (EnemyListElem*)malloc(sizeof(EnemyListElem));
    newElem->data = element;
    newElem->prev = listElem;
    newElem->next = listElem->next;
    listElem->next->prev = newElem;
    listElem->next = newElem;
}

void EnemyListElem_unlink(EnemyListElem* listElem)
{
    listElem->prev->next = listElem->next;
    listElem->next->prev = listElem->prev;
    Enemy_delete(listElem->data);
    free(listElem);
}

EnemyList* EnemyList_construct()
{
    EnemyList* list = (EnemyList*)malloc(sizeof(EnemyList));
    list->head = malloc(sizeof(EnemyListElem));
    list->head->next = list->head->prev = list->head;
    list->count = 0;
    return list;
}

void EnemyList_insertAtStart(EnemyList* list, Enemy* element)
{
    EnemyListElem_linkAfter(list->head, element);
    list->count++;
}

void EnemyList_insertAtEnd(EnemyList* list, Enemy* element)
{
    EnemyListElem_linkBefore(list->head, element);
    list->count++;
}

void EnemyList_insertBefore(EnemyList* list, EnemyListElem* listElem, Enemy* element)
{
    EnemyListElem_linkBefore(listElem, element);
    list->count++;
}

void EnemyList_insertAfter(EnemyList* list, EnemyListElem* listElem, Enemy* element)
{
    EnemyListElem_linkAfter(listElem, element);
    list->count++;
}

void EnemyList_remove(EnemyList* list, EnemyListElem* listElem)
{
    EnemyListElem_unlink(listElem);
    list->count--;
}

void EnemyList_clear(EnemyList* list)
{
    EnemyListElem* iter= list->head->next;
    while (iter != list->head)
    {
        EnemyListElem* temp = iter;
        iter = iter->next;
        Enemy_delete(temp->data);
        free(temp);
    }
    
    list->head->next = list->head->prev = list->head;

    list->count = 0;
}

void EnemyList_delete(EnemyList* list)
{
    EnemyList_clear(list);
    free(list->head);
    free(list);
}