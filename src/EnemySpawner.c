#include "EnemySpawner.h"

#include <malloc.h>
#include <math.h>
#include "Lists/EnemyList.h"
#include "Math/Random.h"
#include "Map.h"

EnemySpawner* EnemySpawner_construct(Map* map)
{
    EnemySpawner* spawner = (EnemySpawner*)malloc(sizeof(EnemySpawner));
    spawner->map = map;
    spawner->wave = 0;
    return spawner;
}

double mapFunction(GameFunction* baseFunc, GameFunction* devFunc, int wave)
{
    return GameFunction_call(baseFunc, wave) + GameFunction_call(devFunc, wave) * Random_double(-1.0, 1.0);
}

void EnemySpawner_advanceWave(EnemySpawner* spawner)
{
    spawner->running = false;
    spawner->wave++;
    spawner->remainingEnemies = GameFunction_call(spawner->map->numFunc, spawner->wave);
    spawner->rate = GameFunction_call(spawner->map->spawnRateFunc, spawner->wave);
    spawner->timer = spawner->rate;
}

void EnemySpawner_update(EnemySpawner* spawner, double delta)
{
    spawner->timer += delta;
    if (spawner->running && spawner->timer > spawner->rate && spawner->remainingEnemies > 0)
    {
        spawner->timer = 0;
        EnemyList_insertAtEnd(spawner->map->context->enemyList, Enemy_construct(spawner->map->context, spawner->map->path,
            mapFunction(spawner->map->hpFunc, spawner->map->hpDevFunc, spawner->wave),
            mapFunction(spawner->map->speedFunc, spawner->map->speedDevFunc, spawner->wave),
            GameFunction_call(spawner->map->moneyFunc, spawner->wave)));
        
        spawner->remainingEnemies--;
    }
}

void EnemySpawner_delete(EnemySpawner* spawner)
{
    free(spawner);
}