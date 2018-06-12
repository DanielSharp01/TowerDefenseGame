#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "BMFont.h"
#include "Texture.h"
#include "../Math/Matrix4.h"
#include <stdbool.h>

typedef struct Sprite
{
    Matrix4 model;
    Vector4 color;
    Vector2 uvTL;
    Vector2 uvBR;
} Sprite;

typedef struct SpriteBatch
{
    unsigned int vao;
    unsigned int vbo;
    int instanceCount;
    int size;
    Sprite* map;
} SpriteBatch;

SpriteBatch* SpriteBatch_construct(int size, Sprite* data);
void SpriteBatch_bind(SpriteBatch* batch);
void SpriteBatch_map(SpriteBatch* batch);
void SpriteBatch_upload(SpriteBatch* batch, Sprite* sprite, int count);
void SpriteBatch_drawS(SpriteBatch* batch, Texture* texture, Vector2 position, Vector2 origin, float scale, float rotation, Vector4 color);
void SpriteBatch_draw(SpriteBatch* batch, Texture* texture, Vector2 position, Vector2 origin, Vector2 scale, float rotation, Vector4 color);
void SpriteBatch_drawString(SpriteBatch* batch, BMFont* font, int* bmStr, Vector2 position, float scale, Vector4 color);
void SpriteBatch_drawStringAlign(SpriteBatch* batch, BMFont* font, int* bmStr, Vector2 position, float scale, Vector4 color, float xalign, float yalign);
void SpriteBatch_drawRectangle(SpriteBatch* batch, Vector2 whiteUV, float x, float y, float width, float height, Vector4 color);
void SpriteBatch_drawRectangleOutline(SpriteBatch* batch, Vector2 whiteUV, float x, float y, float width, float height, float thickness, Vector4 color);
void SpriteBatch_unmap(SpriteBatch* batch);
void SpriteBatch_delete(SpriteBatch* batch);

#endif