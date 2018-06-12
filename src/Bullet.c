#include "Bullet.h"
#include <malloc.h>
#include "Graphics/Texture.h"
#include "Game.h"
#include "Math/Matrix4.h"
#include "Math/VectorFuncs.h"

Bullet* Bullet_construct(GameContext* context, Vector2 position, float rotation, float speed, float damage, float range)
{
    Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));
    bullet->context = context;

    bullet->position = position;
    bullet->rotation = rotation;
    bullet->speed = speed;
    bullet->damage = damage;
    bullet->range = range;
    bullet->color = Vector4_construct(0.1, 0.9, 0.1, 1);
    bullet->deathTimer = 0;
    bullet->dead = false;

    return bullet;
}

void Bullet_update(Bullet* bullet, double delta)
{
    bullet->deathTimer += delta;
    Vector2 velocity = directionSpeed(bullet->rotation, bullet->speed);
    bullet->position.x += velocity.x * delta;
    bullet->position.y += velocity.y * delta;

    bullet->bounds = Rectangle_construct(bullet->position.x - 16, bullet->position.y - 2,
                                         32, 2, bullet->rotation);

    if (bullet->deathTimer > bullet->range / bullet->speed)
    {
        bullet->dead = true;
    }
}

void Bullet_draw(Bullet* bullet, double delta)
{
    SpriteBatch_drawS(bullet->context->batch, bullet->context->mainAtlas->subimages[5][0], bullet->position, Vector2_construct(64, 64), 1, bullet->rotation, bullet->color);
}

void Bullet_delete(Bullet* bullet)
{
    free(bullet);
}