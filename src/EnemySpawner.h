#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <stdbool.h>
#include "Map.h"

typedef struct EnemySpawner
{
    Map* map;
    int wave;
    bool running;
    int remainingEnemies;
    
    float rate;
    float timer;
} EnemySpawner;

EnemySpawner* EnemySpawner_construct(Map* map);
void EnemySpawner_advanceWave(EnemySpawner* spawner);
void EnemySpawner_update(EnemySpawner* spawner, double delta);
void EnemySpawner_delete(EnemySpawner* spawner);

#endif