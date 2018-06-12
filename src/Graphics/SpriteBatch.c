#include "SpriteBatch.h"
#include <GL/glew.h>
#include <malloc.h>

SpriteBatch* SpriteBatch_construct(int size, Sprite* data)
{
    SpriteBatch* ret = (SpriteBatch*)malloc(sizeof(SpriteBatch));
    ret->size = size;
    ret->instanceCount = (data == NULL) ? 0 : size;
    ret->map = NULL;
    glGenVertexArrays(1, &ret->vao);
    glGenBuffers(1, &ret->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, ret->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite) * size, data, GL_DYNAMIC_DRAW);

    glBindVertexArray(ret->vao);
    glBindVertexBuffer(0, ret->vbo, 0, sizeof(Sprite));
    glVertexBindingDivisor(0, 1);
    
    //Model Matrix
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4);
    glVertexAttribBinding(1, 0);

    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8);
    glVertexAttribBinding(2, 0);

    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12);
    glVertexAttribBinding(3, 0);

    //COLOR

    glEnableVertexAttribArray(4);
    glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16);
    glVertexAttribBinding(4, 0);

    //UV
    glEnableVertexAttribArray(5);
    glVertexAttribFormat(5, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 20);
    glVertexAttribBinding(5, 0);

    return ret;
}

void SpriteBatch_bind(SpriteBatch* batch)
{
    glBindVertexArray(batch->vao);
}

void SpriteBatch_map(SpriteBatch* batch)
{
    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    batch->map = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    batch->instanceCount = 0;
}

void SpriteBatch_upload(SpriteBatch* batch, Sprite* sprite, int count)
{
    int i;
    for (i = 0; i < count; i++)
        *batch->map++ = *sprite++;
       
    batch->instanceCount += count;
}

void SpriteBatch_drawS(SpriteBatch* batch, Texture* texture, Vector2 position, Vector2 origin, float scale, float rotation, Vector4 color)
{
    batch->map->model = Matrix4_createTranslation2D(position);
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2Ds(scale, scale));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createRotationZ(rotation));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createTranslation2Ds(-origin.x, -origin.y));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2Ds(texture->width, texture->height));
    batch->map->color = color;
    batch->map->uvTL = texture->uvTL;
    batch->map->uvBR = texture->uvBR;
    batch->map++;
    batch->instanceCount++;
}

void SpriteBatch_draw(SpriteBatch* batch, Texture* texture, Vector2 position, Vector2 origin, Vector2 scale, float rotation, Vector4 color)
{
    batch->map->model = Matrix4_createTranslation2D(position);
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2D(scale));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createRotationZ(rotation));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createTranslation2Ds(-origin.x, -origin.y));
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2Ds(texture->width, texture->height));
    batch->map->color = color;
    batch->map->uvTL = texture->uvTL;
    batch->map->uvBR = texture->uvBR;
    batch->map++;
    batch->instanceCount++;
}

void SpriteBatch_drawString(SpriteBatch* batch, BMFont* font, int* bmStr, Vector2 position, float scale, Vector4 color)
{
    float startX = position.x;
    for (;*bmStr != '\0'; bmStr++)
    {
        if (*bmStr == '\n')
        {
            position.y += font->lineHeight * scale;
            position.x = startX;
            continue;
        }

        if (font->chars[*bmStr - 1].id != ' ')
        {
            batch->map->model = Matrix4_createTranslation2Ds(position.x + font->chars[*bmStr - 1].xoffset * scale, position.y + font->chars[*bmStr - 1].yoffset * scale);
            batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2Ds(font->chars[*bmStr - 1].width * scale, font->chars[*bmStr - 1].height * scale));
            batch->map->color = color;
            batch->map->uvTL = font->chars[*bmStr - 1].uvTL;
            batch->map->uvBR = font->chars[*bmStr - 1].uvBR;
            batch->map++;
            batch->instanceCount++;
        }
        
        position.x += font->chars[*bmStr - 1].xadvance * scale;
    }
}

void SpriteBatch_drawStringAlign(SpriteBatch* batch, BMFont* font, int* bmStr, Vector2 position, float scale, Vector4 color, float xalign, float yalign)
{
    Vector2 size = BMFont_measure(font, bmStr, scale);
    SpriteBatch_drawString(batch, font, bmStr, Vector2_construct(position.x - size.x * xalign, position.y - size.y * yalign), scale, color);
}

void SpriteBatch_drawRectangle(SpriteBatch* batch, Vector2 whiteUV, float x, float y, float width, float height, Vector4 color)
{
    batch->map->model = Matrix4_createTranslation2Ds(x, y);
    batch->map->model = Matrix4_mul(batch->map->model, Matrix4_createScale2Ds(width, height));
    batch->map->color = color;
    batch->map->uvTL = whiteUV;
    batch->map->uvBR = whiteUV;
    batch->map++;
    batch->instanceCount++;
}

void SpriteBatch_drawRectangleOutline(SpriteBatch* batch, Vector2 whiteUV, float x, float y, float width, float height, float thickness, Vector4 color)
{
    SpriteBatch_drawRectangle(batch, whiteUV, x + thickness, y - thickness, width - 2 * thickness, 2 * thickness, color);
    SpriteBatch_drawRectangle(batch, whiteUV, x + thickness, y + height - thickness, width - 2 * thickness, 2 * thickness, color);
    SpriteBatch_drawRectangle(batch, whiteUV, x - thickness, y - thickness, 2 * thickness, height + 2 * thickness, color);
    SpriteBatch_drawRectangle(batch, whiteUV, x + width - thickness, y - thickness, 2 * thickness, height + 2 * thickness, color);
}

void SpriteBatch_unmap(SpriteBatch* batch)
{
    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    batch->map = NULL;
}
void SpriteBatch_delete(SpriteBatch* batch)
{
    glDeleteVertexArrays(1, &batch->vao);
    glDeleteBuffers(1, &batch->vbo);
    free(batch);
}