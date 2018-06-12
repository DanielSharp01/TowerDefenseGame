#include "NodeList.h"

#include <malloc.h>

void NodeListElem_linkBefore(NodeListElem* listElem, Node* element)
{
    NodeListElem* newElem = (NodeListElem*)malloc(sizeof(NodeListElem));
    newElem->data = element;
    newElem->prev = listElem->prev;
    newElem->next = listElem;
    listElem->prev->next = newElem;
    listElem->prev = newElem;
}

void NodeListElem_linkAfter(NodeListElem* listElem, Node* element)
{
    NodeListElem* newElem = (NodeListElem*)malloc(sizeof(NodeListElem));
    newElem->data = element;
    newElem->prev = listElem;
    newElem->next = listElem->next;
    listElem->next->prev = newElem;
    listElem->next = newElem;
}

void NodeListElem_unlink(NodeListElem* listElem)
{
    listElem->prev->next = listElem->next;
    listElem->next->prev = listElem->prev;
    free(listElem);
}

NodeList* NodeList_construct()
{
    NodeList* list = (NodeList*)malloc(sizeof(NodeList));
    list->head = malloc(sizeof(NodeListElem));
    list->head->next = list->head->prev = list->head;
    list->count = 0;
    return list;
}

void NodeList_insertAtStart(NodeList* list, Node* element)
{
    NodeListElem_linkAfter(list->head, element);
    list->count++;
}

void NodeList_insertAtEnd(NodeList* list, Node* element)
{
    NodeListElem_linkBefore(list->head, element);
    list->count++;
}

void NodeList_insertBefore(NodeList* list, NodeListElem* listElem, Node* element)
{
    NodeListElem_linkBefore(listElem, element);
    list->count++;
}

void NodeList_insertAfter(NodeList* list, NodeListElem* listElem, Node* element)
{
    NodeListElem_linkAfter(listElem, element);
    list->count++;
}

void NodeList_remove(NodeList* list, NodeListElem* listElem)
{
    NodeListElem_unlink(listElem);
    list->count--;
}

void NodeList_clear(NodeList* list)
{
    NodeListElem* iter = list->head->next;
    while (iter != list->head)
    {
        NodeListElem* temp = iter;
        iter = iter->next;
        free(temp);
    }
    
    list->head->next = list->head->prev = list->head;

    list->count = 0;
}

void NodeList_delete(NodeList* list)
{
    NodeList_clear(list);
    free(list->head);
    free(list);
}