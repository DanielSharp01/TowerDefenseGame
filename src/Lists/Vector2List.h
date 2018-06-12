#ifndef VECTOR2_LIST_H
#define VECTOR2_LIST_H

#include "../Math/Vector2.h"

typedef struct Vector2ListElem
{
    Vector2 data;

    struct Vector2ListElem* prev;
    struct Vector2ListElem* next;  
} Vector2ListElem;

void Vector2ListElem_linkBefore(Vector2ListElem* listElem, Vector2 element);
void Vector2ListElem_linkAfter(Vector2ListElem* listElem, Vector2 element);
void Vector2ListElem_unlink(Vector2ListElem* listElem);

typedef struct Vector2List
{
    Vector2ListElem* head;
    int count;
} Vector2List;

Vector2List* Vector2List_construct();
void Vector2List_insertAtStart(Vector2List* list, Vector2 element);
void Vector2List_insertAtEnd(Vector2List* list, Vector2 element);
void Vector2List_insertBefore(Vector2List* list, Vector2ListElem* listElem, Vector2 element);
void Vector2List_insertAfter(Vector2List* list, Vector2ListElem* listElem, Vector2 element);
void Vector2List_remove(Vector2List* list, Vector2ListElem* listElem);
void Vector2List_clear(Vector2List* list);
void Vector2List_delete(Vector2List* list);

#endif