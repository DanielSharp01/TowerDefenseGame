#ifndef ENEMY_H
#define ENEMY_H

#include "Game.h"
#include "Math/Vector2.h"
#include "Graphics/SpriteBatch.h"
#include "Lists/Vector2List.h"
#include "Math/Shapes.h"
#include <stdbool.h>

typedef struct Enemy
{
    GameContext* context;
    Vector2 position;
    float rotation;
    float speed;
    float hp;
    float maxHP;
    int money;
    bool dead;

    Circle bounds;

    Vector2List* path;
    float pathInterpolation;
    Vector2ListElem* elemM;
    int mPathIndex;

} Enemy;

Enemy* Enemy_construct(GameContext* context, Vector2List* path, float hp, float speed, int money);
void Enemy_update(Enemy* enemy, double delta);

Vector2 Enemy_anticipatePosition(Enemy* enemy, float time);

void Enemy_draw(Enemy* enemy, double delta);
void Enemy_delete(Enemy* enemy);

#endif