#ifndef TEMPLATE_LINKED_LIST_H
#define TEMPLATE_LINKED_LIST_H

typedef struct TEMPLATEListElem
{
    TEMPLATE data;

    struct TEMPLATEListElem* prev;
    struct TEMPLATEListElem* next;  
} TEMPLATEListElem;

void TEMPLATEListElem_linkBefore(TEMPLATEListElem* listElem, TEMPLATE element);
void TEMPLATEListElem_linkAfter(TEMPLATEListElem* listElem, TEMPLATE element);
void TEMPLATEListElem_unlink(TEMPLATEListElem* listElem);

typedef struct TEMPLATELinkedList
{
    TEMPLATEListElem* head;
    int count;
} TEMPLATELinkedList;

TEMPLATELinkedList* TEMPLATELinkedList_construct();
void TEMPLATELinkedList_insertAtStart(TEMPLATELinkedList* list, TEMPLATE element);
void TEMPLATELinkedList_insertAtEnd(TEMPLATELinkedList* list, TEMPLATE element);
void TEMPLATELinkedList_insertBefore(TEMPLATELinkedList* list, TEMPLATEListElem* listElem, TEMPLATE element);
void TEMPLATELinkedList_insertAfter(TEMPLATELinkedList* list, TEMPLATEListElem* listElem, TEMPLATE element);
void TEMPLATELinkedList_remove(TEMPLATELinkedList* list, TEMPLATEListElem* listElem);
void TEMPLATELinkedList_clear(TEMPLATELinkedList* list);
void TEMPLATELinkedList_delete(TEMPLATELinkedList* list);

#endif