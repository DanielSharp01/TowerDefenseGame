#include "TEMPLATELinkedList.h"

#include <malloc.h>

void TEMPLATEListElem_linkBefore(TEMPLATEListElem* listElem, TEMPLATE element)
{
    TEMPLATEListElem* newElem = (TEMPLATEListElem*)malloc(sizeof(TEMPLATEListElem));
    newElem->data = element;
    newElem->prev = listElem->prev;
    newElem->next = listElem;
    listElem->prev->next = newElem;
    listElem->prev = newElem;
}

void TEMPLATEListElem_linkAfter(TEMPLATEListElem* listElem, TEMPLATE element)
{
    TEMPLATEListElem* newElem = (TEMPLATEListElem*)malloc(sizeof(TEMPLATEListElem));
    newElem->data = element;
    newElem->prev = listElem;
    newElem->next = listElem->next;
    listElem->next->prev = newElem;
    listElem->next = newElem;
}

void TEMPLATEListElem_unlink(TEMPLATEListElem* listElem)
{
    listElem->prev->next = listElem->next;
    listElem->next->prev = listElem->prev;
    //delete pointers if template is pointer
    free(listElem);
}

TEMPLATELinkedList* TEMPLATELinkedList_construct()
{
    TEMPLATELinkedList* list = (TEMPLATELinkedList*)malloc(sizeof(TEMPLATELinkedList));
    list->head = malloc(sizeof(TEMPLATEListElem));
    list->head->next = list->head->prev = list->head;
    list->count = 0;
    return list;
}

void TEMPLATELinkedList_insertAtStart(TEMPLATELinkedList* list, TEMPLATE element)
{
    TEMPLATEListElem_linkAfter(list->head, element);
    list->count++;
}

void TEMPLATELinkedList_insertAtEnd(TEMPLATELinkedList* list, TEMPLATE element)
{
    TEMPLATEListElem_linkBefore(list->head, element);
    list->count++;
}

void TEMPLATELinkedList_insertBefore(TEMPLATELinkedList* list, TEMPLATEListElem* listElem, TEMPLATE element)
{
    TEMPLATEListElem_linkBefore(listElem, element);
    list->count++;
}

void TEMPLATELinkedList_insertAfter(TEMPLATELinkedList* list, TEMPLATEListElem* listElem, TEMPLATE element)
{
    TEMPLATEListElem_linkAfter(listElem, element);
    list->count++;
}

void TEMPLATELinkedList_remove(TEMPLATELinkedList* list, TEMPLATEListElem* listElem)
{
    TEMPLATEListElem_unlink(listElem);
    list->count--;
}

void TEMPLATELinkedList_clear(TEMPLATELinkedList* list)
{
    TEMPLATEListElem* iter= list->head->next;
    while (iter != list->head)
    {
        TEMPLATEListElem* temp = iter->next;
        //delete pointers if template is pointer
        free(iter);
        iter = temp;
    }
    
    list->head->next = list->head->prev = list->head;

    list->count = 0;
}

void TEMPLATELinkedList_delete(TEMPLATELinkedList* list)
{
    TEMPLATELinkedList_clear(list);
    free(list->head);
    free(list);
}