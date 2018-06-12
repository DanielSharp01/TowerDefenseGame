#include "BulletList.h"

#include <malloc.h>

void BulletListElem_linkBefore(BulletListElem* listElem, Bullet* element)
{
    BulletListElem* newElem = (BulletListElem*)malloc(sizeof(BulletListElem));
    newElem->data = element;
    newElem->prev = listElem->prev;
    newElem->next = listElem;
    listElem->prev->next = newElem;
    listElem->prev = newElem;
}

void BulletListElem_linkAfter(BulletListElem* listElem, Bullet* element)
{
    BulletListElem* newElem = (BulletListElem*)malloc(sizeof(BulletListElem));
    newElem->data = element;
    newElem->prev = listElem;
    newElem->next = listElem->next;
    listElem->next->prev = newElem;
    listElem->next = newElem;
}

void BulletListElem_unlink(BulletListElem* listElem)
{
    listElem->prev->next = listElem->next;
    listElem->next->prev = listElem->prev;
    Bullet_delete(listElem->data);
    free(listElem);
}

BulletList* BulletList_construct()
{
    BulletList* list = (BulletList*)malloc(sizeof(BulletList));
    list->head = malloc(sizeof(BulletListElem));
    list->head->next = list->head->prev = list->head;
    list->count = 0;
    return list;
}

void BulletList_insertAtStart(BulletList* list, Bullet* element)
{
    BulletListElem_linkAfter(list->head, element);
    list->count++;
}

void BulletList_insertAtEnd(BulletList* list, Bullet* element)
{
    BulletListElem_linkBefore(list->head, element);
    list->count++;
}

void BulletList_insertBefore(BulletList* list, BulletListElem* listElem, Bullet* element)
{
    BulletListElem_linkBefore(listElem, element);
    list->count++;
}

void BulletList_insertAfter(BulletList* list, BulletListElem* listElem, Bullet* element)
{
    BulletListElem_linkAfter(listElem, element);
    list->count++;
}

void BulletList_remove(BulletList* list, BulletListElem* listElem)
{
    BulletListElem_unlink(listElem);
    list->count--;
}

void BulletList_clear(BulletList* list)
{
    BulletListElem* iter= list->head->next;
    while (iter != list->head)
    {
        BulletListElem* temp = iter;
        iter = iter->next;
        Bullet_delete(temp->data);
        free(temp);
    }
    
    list->head->next = list->head->prev = list->head;

    list->count = 0;
}

void BulletList_delete(BulletList* list)
{
    BulletList_clear(list);
    free(list->head);
    free(list);
}