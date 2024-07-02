#include "Tower.h"

#include <malloc.h>
#include "Input.h"
#include "Math/VectorFuncs.h"
#include "Map.h"
#include "Lists/EnemyList.h"
#include "Lists/BulletList.h"

const int TOWER_MAX_UPGRADE_LEVEL = 5;
const int TOWER_COST = 5;
const float TOWER_SELL_MULTIPLER = 0.6;
const float TOWER_UPGRADE_SELL_MULTIPLER = 0.4;

Tower *Tower_construct(GameContext *context, Cell *parentCell)
{
    Tower *tower = (Tower *)malloc(sizeof(Tower));
    tower->context = context;
    tower->position.x = parentCell->x * 128 + 64;
    tower->position.y = parentCell->y * 128 + 64;
    tower->rotation = 0;
    tower->parentCell = parentCell;
    tower->parentCell->tower = tower;

    tower->sellCost = (int)(TOWER_SELL_MULTIPLER * TOWER_COST);
    tower->rangeUpgrLev = 1;
    tower->damageUpgrLev = 1;
    tower->rateUpgrLev = 1;
    tower->range = Tower_rangeUpgradeFunc(1);
    tower->damage = Tower_damageUpgradeFunc(1);
    tower->rateOfFire = Tower_rateUpgradeFunc(1);
    tower->bulletSpeed = 1600;
    tower->fireTimer = tower->rateOfFire;
    tower->focus = NULL;
    tower->aiFunc = Tower_AI_Closest;

    parentCell->sprite->color = Vector4_construct(0.1, 0.9, 0.1, 0.5);
    parentCell->sprite->uvTL = context->mainAtlas->subimages[1][0]->uvTL;
    parentCell->sprite->uvBR = context->mainAtlas->subimages[1][0]->uvBR;

    return tower;
}

void Tower_setTowerAI(Tower *tower, TowerAI aiFunc)
{
    tower->focus = NULL;
    tower->aiFunc = aiFunc;
}

void Tower_upgradeRange(Tower *tower)
{
    tower->sellCost += (int)(TOWER_UPGRADE_SELL_MULTIPLER * Tower_upgradeCostFunc(tower->rangeUpgrLev));
    tower->range = Tower_rangeUpgradeFunc(++tower->rangeUpgrLev);
}

void Tower_upgradeDamage(Tower *tower)
{
    tower->sellCost += (int)(TOWER_UPGRADE_SELL_MULTIPLER * Tower_upgradeCostFunc(tower->damageUpgrLev));
    tower->damage = Tower_damageUpgradeFunc(++tower->damageUpgrLev);
}

void Tower_upgradeRate(Tower *tower)
{
    tower->sellCost += (int)(TOWER_UPGRADE_SELL_MULTIPLER * Tower_upgradeCostFunc(tower->rateUpgrLev));
    tower->rateOfFire = Tower_rateUpgradeFunc(++tower->rateUpgrLev);
}

void Tower_update(Tower *tower, double delta)
{
    tower->fireTimer += delta;

    if (tower->aiFunc == NULL)
    {
        tower->rotation = pointDirection(tower->position, getMousePositionWorld());
        if (isMButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !tower->context->consumedMouse && tower->fireTimer > tower->rateOfFire * 0.75)
        {
            tower->fireTimer = 0;
            BulletList_insertAtEnd(tower->context->bulletList,
                                   Bullet_construct(tower->context, tower->position, tower->rotation, tower->bulletSpeed, tower->damage, tower->range));
        }
    }
    else
    {
        if (tower->focus == NULL || tower->focus->dead ||
            pointDistance(tower->position, tower->focus->position) > tower->range)
        {
            tower->focus = tower->aiFunc(tower, tower->context->enemyList);
        }

        if (tower->focus != NULL)
        {
            float bulletTime = pointDistance(tower->position, tower->focus->position) / tower->bulletSpeed;
            Vector2 targetPos = Enemy_anticipatePosition(tower->focus, bulletTime);
            tower->rotation = pointDirection(tower->position, targetPos);

            if (tower->fireTimer > tower->rateOfFire)
            {
                tower->fireTimer = 0;
                BulletList_insertAtEnd(tower->context->bulletList,
                                       Bullet_construct(tower->context, tower->position, tower->rotation,
                                                        tower->bulletSpeed, tower->damage, tower->range));
            }
        }
    }
}

void Tower_draw(Tower *tower, double delta)
{
    SpriteBatch_drawS(tower->context->batch, tower->context->mainAtlas->subimages[2][0], tower->position, Vector2_construct(64, 64), 1, tower->rotation, Vector4_construct(0.1, 0.9, 0.1, 1));
    SpriteBatch_drawS(tower->context->batch, tower->context->mainAtlas->subimages[3][0], tower->position, Vector2_construct(40, 64), 1, tower->rotation, Vector4_construct(0.1, 0.9, 0.1, 1));
}

void Tower_delete(Tower *tower)
{
    tower->parentCell->tower = NULL;
    tower->parentCell->sprite->color = Vector4_construct(0.2, 0.2, 0.2, 0.2);
    tower->parentCell->sprite->uvTL = tower->context->mainAtlas->subimages[0][0]->uvTL;
    tower->parentCell->sprite->uvBR = tower->context->mainAtlas->subimages[0][0]->uvBR;
    free(tower);
}

Enemy *Tower_AI_Closest(Tower *tower, EnemyList *enemyList)
{
    Enemy *closest = NULL;
    float closestDist;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (closest == NULL || closestDist > distance))
        {
            closest = iter->data;
            closestDist = distance;
        }
    }

    return closest;
}

Enemy *Tower_AI_Furthest(Tower *tower, EnemyList *enemyList)
{
    Enemy *furthest = NULL;
    float furthestDist;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (furthest == NULL || furthestDist < distance))
        {
            furthest = iter->data;
            furthestDist = distance;
        }
    }

    return furthest;
}

Enemy *Tower_AI_LowestHP(Tower *tower, EnemyList *enemyList)
{
    Enemy *ret = NULL;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (ret == NULL || ret->hp > iter->data->hp))
        {
            ret = iter->data;
        }
    }

    return ret;
}

Enemy *Tower_AI_HighestHP(Tower *tower, EnemyList *enemyList)
{
    Enemy *ret = NULL;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (ret == NULL || ret->hp < iter->data->hp))
        {
            ret = iter->data;
        }
    }

    return ret;
}

Enemy *Tower_AI_Slowest(Tower *tower, EnemyList *enemyList)
{
    Enemy *ret = NULL;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (ret == NULL || ret->speed > iter->data->speed))
        {
            ret = iter->data;
        }
    }

    return ret;
}

Enemy *Tower_AI_Fastest(Tower *tower, EnemyList *enemyList)
{
    Enemy *ret = NULL;
    EnemyListElem *iter;
    for (iter = enemyList->head->next; iter != enemyList->head; iter = iter->next)
    {
        float distance = pointDistance(tower->position, iter->data->position);
        if (distance <= tower->range && !iter->data->dead && (ret == NULL || ret->speed < iter->data->speed))
        {
            ret = iter->data;
        }
    }

    return ret;
}

float Tower_rangeUpgradeFunc(int level)
{
    return 300 + 300 * level; // 600 - 1800
}

float Tower_damageUpgradeFunc(int level)
{
    return 0.5 + level / 4.0; // 0.5 - 1.75
}

float Tower_rateUpgradeFunc(int level)
{
    return 0.1 + 0.2 / level; // 0.3 - 0.14
}

float Tower_upgradeCostFunc(int level)
{
    return 6 * level;
}