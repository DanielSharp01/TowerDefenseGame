#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "Texture.h"

//NOTE: Delete frees up the base texture for convenience
typedef struct TextureAtlas
{
    Texture* base;
    Texture*** subimages;
    int cols, rows;
} TextureAtlas;

TextureAtlas* TextureAtlas_construct(Texture* base, int cols, int rows, int width, int height, int paddingX, int paddingY);
void TextureAtlas_delete(TextureAtlas* atlas);

#endif