#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Math/Vector2.h"
#include <stdbool.h>

typedef struct Texture Texture;

struct Texture
{
    unsigned int id;
    int width;
    int height;
    Vector2 uvTL, uvBR;
    Texture* base;
};

Texture* Texture_construct(unsigned char* rawImage, int width, int height);
Texture* Texture_constructSubimage(Texture* base, int x, int y, int width, int height);
Texture* Texture_load(const char* imagePath);
void Texture_bind(Texture* tex);
void Texture_delete(Texture* tex);

#endif