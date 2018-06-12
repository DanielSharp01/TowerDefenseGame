#include "Enemy.h"
#include <malloc.h>
#include "Math/Matrix4.h"
#include "Math/VectorFuncs.h"
#include "Lists/BulletList.h"

Enemy* Enemy_construct(GameContext* context, Vector2List* path, float hp, float speed, int money)
{
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    enemy->context = context;

    enemy->path = path;
    enemy->speed = speed;
    enemy->hp = hp;
    enemy->maxHP = hp;
    enemy->money = money;
    enemy->dead = false;
    enemy->pathInterpolation = 0;
    enemy->mPathIndex = 0;
    enemy->elemM = path->head->next;
    enemy->position = enemy->elemM->data;
    return enemy;
}

void Enemy_update(Enemy* enemy, double delta)
{
    enemy->pathInterpolation += delta * enemy->speed;

    if (enemy->pathInterpolation > enemy->path->count - 1)
    {
        enemy->dead = true;
        return;
    }

    int pathIndex = (int)(enemy->pathInterpolation + 0.5);
    
    while (pathIndex > enemy->mPathIndex)
    {
        enemy->elemM = enemy->elemM->next;
        enemy->mPathIndex++;
    }
    
    Vector2ListElem* elemP = enemy->elemM->prev;
    Vector2ListElem* elemQ = enemy->elemM->next;
    
    Vector2 p = Vector2_add(elemP->data, Vector2_divS(Vector2_sub(enemy->elemM->data, elemP->data), 2));
    Vector2 q = Vector2_add(enemy->elemM->data, Vector2_divS(Vector2_sub(elemQ->data, enemy->elemM->data), 2));

    if (elemP == enemy->path->head) p = Vector2_sub(enemy->elemM->data, Vector2_sub(q, enemy->elemM->data));
    if (elemQ == enemy->path->head) q = Vector2_sub(enemy->elemM->data, Vector2_sub(p, enemy->elemM->data));

    enemy->position = bezierCurve(p, q, enemy->elemM->data, fmod(enemy->pathInterpolation + 0.5, 1));
    Vector2 bezierDeriv = bezierCurveDerivative(p, q, enemy->elemM->data, fmod(enemy->pathInterpolation + 0.5, 1));
    enemy->rotation = atan2(bezierDeriv.y, bezierDeriv.x);

    enemy->bounds = Circle_construct(enemy->position, 38);

    BulletListElem* bulIter;
    for (bulIter = enemy->context->bulletList->head->next;
        bulIter != enemy->context->bulletList->head; bulIter = bulIter->next)
    {
        if (Rectangle_intersectsCircle(bulIter->data->bounds, enemy->bounds))
        {
            bulIter->data->dead = true;
            enemy->hp -= bulIter->data->damage;

            if (enemy->hp <= 0)
            {
                enemy->dead = true;
                int i;
                for (i = 0; i < 50; i++)
                {
                    Bullet* particle = Bullet_construct(enemy->context, enemy->position, 2*PI * (i / 50.0), 1200, 0, 300);
                    particle->color = Vector4_construct(0.9, 0.1, 0.1, 0.9);
                    BulletList_insertAtEnd(enemy->context->particleList, particle);
                }
                return;
            }
        }
    }
}

Vector2 Enemy_anticipatePosition(Enemy* enemy, float time)
{
    float anticipatedInterp = enemy->pathInterpolation + enemy->speed * time;
    if (anticipatedInterp > enemy->path->count - 1)
    {
        anticipatedInterp = enemy->path->count - 1;
    }

    int pathIndex = (int)(anticipatedInterp + 0.5);
    
    int mPathIndex = enemy->mPathIndex;
    Vector2ListElem* elemM = enemy->elemM;
    while (pathIndex > mPathIndex)
    {
        elemM = elemM->next;
        mPathIndex++;
    }
    
    Vector2ListElem* elemP = elemM->prev;
    Vector2ListElem* elemQ = elemM->next;
    
    Vector2 p = Vector2_add(elemP->data, Vector2_divS(Vector2_sub(elemM->data, elemP->data), 2));
    Vector2 q = Vector2_add(elemM->data, Vector2_divS(Vector2_sub(elemQ->data, elemM->data), 2));

    if (elemP == enemy->path->head) p = Vector2_sub(elemM->data, Vector2_sub(q, elemM->data));
    if (elemQ == enemy->path->head) q = Vector2_sub(elemM->data, Vector2_sub(p, elemM->data));

    return bezierCurve(p, q, elemM->data, fmod(anticipatedInterp + 0.5, 1));
}

void Enemy_draw(Enemy* enemy, double delta)
{
    SpriteBatch_drawS(enemy->context->batch, enemy->context->mainAtlas->subimages[4][0], enemy->position, Vector2_construct(64, 64), 1, enemy->rotation, Vector4_construct(0.9, 0.1, 0.1, 1));
    SpriteBatch_drawRectangleOutline(enemy->context->batch, Vector2_construct(0.921875, 0.15625), enemy->position.x - 40, enemy->position.y - 70, 80, 16, 1, Vector4_construct(0.9, 0.9, 0.9, 0.5));
    SpriteBatch_drawRectangle(enemy->context->batch, Vector2_construct(0.921875, 0.15625), enemy->position.x - 38, enemy->position.y - 68, 76 * (enemy->hp / enemy->maxHP), 12, Vector4_construct(0.9, 0.1, 0.1, 0.9));
}

void Enemy_delete(Enemy* enemy)
{
    free(enemy);
}