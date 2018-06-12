#ifndef BULLET_H
#define BULLET_H

#include "Game.h"
#include "Math/Vector2.h"
#include "Graphics/SpriteBatch.h"
#include "Math/Shapes.h"

typedef struct Bullet
{
    GameContext* context;
    Vector2 position;
    Rectangle bounds;
    Vector4 color;
    
    float rotation;
    float speed;
    float damage;
    float range;
    
    bool dead;
    float deathTimer;

} Bullet;

Bullet* Bullet_construct(GameContext* context, Vector2 position, float rotation, float speed, float damage, float range);
void Bullet_update(Bullet* bullet, double delta);
void Bullet_draw(Bullet* bullet, double delta);
void Bullet_delete(Bullet* bullet);

#endif