#include "Vector2List.h"

#include <malloc.h>

void Vector2ListElem_linkBefore(Vector2ListElem* listElem, Vector2 element)
{
    Vector2ListElem* newElem = (Vector2ListElem*)malloc(sizeof(Vector2ListElem));
    newElem->data = element;
    newElem->prev = listElem->prev;
    newElem->next = listElem;
    listElem->prev->next = newElem;
    listElem->prev = newElem;
}

void Vector2ListElem_linkAfter(Vector2ListElem* listElem, Vector2 element)
{
    Vector2ListElem* newElem = (Vector2ListElem*)malloc(sizeof(Vector2ListElem));
    newElem->data = element;
    newElem->prev = listElem;
    newElem->next = listElem->next;
    listElem->next->prev = newElem;
    listElem->next = newElem;
}

void Vector2ListElem_unlink(Vector2ListElem* listElem)
{
    listElem->prev->next = listElem->next;
    listElem->next->prev = listElem->prev;
    free(listElem);
}

Vector2List* Vector2List_construct()
{
    Vector2List* list = (Vector2List*)malloc(sizeof(Vector2List));
    list->head = malloc(sizeof(Vector2ListElem));
    list->head->next = list->head->prev = list->head;
    list->count = 0;
    return list;
}

void Vector2List_insertAtStart(Vector2List* list, Vector2 element)
{
    Vector2ListElem_linkAfter(list->head, element);
    list->count++;
}

void Vector2List_insertAtEnd(Vector2List* list, Vector2 element)
{
    Vector2ListElem_linkBefore(list->head, element);
    list->count++;
}

void Vector2List_insertBefore(Vector2List* list, Vector2ListElem* listElem, Vector2 element)
{
    Vector2ListElem_linkBefore(listElem, element);
    list->count++;
}

void Vector2List_insertAfter(Vector2List* list, Vector2ListElem* listElem, Vector2 element)
{
    Vector2ListElem_linkAfter(listElem, element);
    list->count++;
}

void Vector2List_remove(Vector2List* list, Vector2ListElem* listElem)
{
    Vector2ListElem_unlink(listElem);
    list->count--;
}

void Vector2List_clear(Vector2List* list)
{
    Vector2ListElem* iter= list->head->next;
    while (iter != list->head)
    {
        Vector2ListElem* temp = iter;
        iter = iter->next;
        free(temp);
    }
    
    list->head->next = list->head->prev = list->head;

    list->count = 0;
}

void Vector2List_delete(Vector2List* list)
{
    Vector2List_clear(list);
    free(list->head);
    free(list);
}