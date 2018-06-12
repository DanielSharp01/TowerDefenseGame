#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "../Math/Node.h"

typedef struct NodeListElem
{
    Node* data;

    struct NodeListElem* prev;
    struct NodeListElem* next;  
} NodeListElem;

void NodeListElem_linkBefore(NodeListElem* listElem, Node* element);
void NodeListElem_linkAfter(NodeListElem* listElem, Node* element);
void NodeListElem_unlink(NodeListElem* listElem);

typedef struct NodeList
{
    NodeListElem* head;
    int count;
} NodeList;

NodeList* NodeList_construct();
void NodeList_insertAtStart(NodeList* list, Node* element);
void NodeList_insertAtEnd(NodeList* list, Node* element);
void NodeList_insertBefore(NodeList* list, NodeListElem* listElem, Node* element);
void NodeList_insertAfter(NodeList* list, NodeListElem* listElem, Node* element);
void NodeList_remove(NodeList* list, NodeListElem* listElem);
void NodeList_clear(NodeList* list);
void NodeList_delete(NodeList* list);

#endif