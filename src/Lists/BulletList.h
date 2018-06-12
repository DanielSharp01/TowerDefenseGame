#ifndef BULLET_LIST_H
#define BULLET_LIST_H

#include "../Bullet.h"

typedef struct BulletListElem
{
    Bullet* data;

    struct BulletListElem* prev;
    struct BulletListElem* next;  
} BulletListElem;

void BulletListElem_linkBefore(BulletListElem* listElem, Bullet* element);
void BulletListElem_linkAfter(BulletListElem* listElem, Bullet* element);
void BulletListElem_unlink(BulletListElem* listElem);

typedef struct BulletList
{
    BulletListElem* head;
    int count;
} BulletList;

BulletList* BulletList_construct();
void BulletList_insertAtStart(BulletList* list, Bullet* element);
void BulletList_insertAtEnd(BulletList* list, Bullet* element);
void BulletList_insertBefore(BulletList* list, BulletListElem* listElem, Bullet* element);
void BulletList_insertAfter(BulletList* list, BulletListElem* listElem, Bullet* element);
void BulletList_remove(BulletList* list, BulletListElem* listElem);
void BulletList_clear(BulletList* list);
void BulletList_delete(BulletList* list);

#endif