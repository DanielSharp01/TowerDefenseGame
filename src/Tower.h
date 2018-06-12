#ifndef TOWER_H
#define TOWER_H

#include "Game.h"

typedef struct Enemy Enemy;
typedef struct Tower Tower;

//Enemy from list within range
typedef Enemy* (*TowerAI)(Tower*, EnemyList*);

const int TOWER_MAX_UPGRADE_LEVEL;
const int TOWER_COST;
const float TOWER_SELL_MULTIPLER;
const float TOWER_UPGRADE_SELL_MULTIPLER;

struct Tower
{
    GameContext* context;
    Vector2 position;
    float rotation;
    
    int sellCost;
    float rateOfFire;
    float fireTimer;
    float bulletSpeed;
    float damage;
    float range;

    int rangeUpgrLev;
    int damageUpgrLev;
    int rateUpgrLev;

    TowerAI lastAiFunc;
    TowerAI aiFunc;
    Enemy* focus;
    Cell* parentCell;
};

Tower* Tower_construct(GameContext* context, Cell* parentCell);

void Tower_setTowerAI(Tower* tower, TowerAI aiFunc);
void Tower_upgradeRange(Tower* tower);
void Tower_upgradeDamage(Tower* tower);
void Tower_upgradeRate(Tower* tower);

void Tower_update(Tower* tower, double delta);
void Tower_draw(Tower* tower, double delta);
void Tower_delete(Tower* tower);

Enemy* Tower_AI_Closest(Tower* tower, EnemyList* enemyList);
Enemy* Tower_AI_Furthest(Tower* tower, EnemyList* enemyList);
Enemy* Tower_AI_LowestHP(Tower* tower, EnemyList* enemyList);
Enemy* Tower_AI_HighestHP(Tower* tower, EnemyList* enemyList);
Enemy* Tower_AI_Slowest(Tower* tower, EnemyList* enemyList);
Enemy* Tower_AI_Fastest(Tower* tower, EnemyList* enemyList);

float Tower_rangeUpgradeFunc(int level);
float Tower_damageUpgradeFunc(int level);
float Tower_rateUpgradeFunc(int level);
float Tower_upgradeCostFunc(int level);

#endif