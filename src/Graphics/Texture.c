#include "Texture.h"

#include <GL/glew.h>
#include <malloc.h>
#include "../Utils/FileMethods.h"

Texture* Texture_construct(unsigned char* rawImage, int width, int height)
{
    Texture* tex = (Texture*)malloc(sizeof(Texture));
    tex->base = NULL;
    tex->width = width;
    tex->height = height;
    tex->uvTL = Vector2_construct(0, 0);
    tex->uvBR = Vector2_construct(1, 1);
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return tex;
}

Texture* Texture_constructSubimage(Texture* base, int x, int y, int width, int height)
{
    Texture* tex = (Texture*)malloc(sizeof(Texture));
    tex->base = base;
    tex->id = base->id;
    tex->width = width;
    tex->height = height;
    tex->uvTL = Vector2_construct((float)x / base->width, (float)y / base->height);
    tex->uvBR = Vector2_construct((float)(x+width) / base->width, (float)(y+height) / base->height);
    return tex;
}

Texture* Texture_load(const char* imagePath)
{
    int w, h;
    unsigned char* rawBytes = readImage(imagePath, &w, &h);
    Texture* tex = Texture_construct(rawBytes, w, h);
    freeImage(rawBytes);
    return tex;
}

void Texture_bind(Texture* tex)
{
    glBindTexture(GL_TEXTURE_2D, tex->id);
}

void Texture_delete(Texture* tex)
{
    if (tex->base == NULL) glDeleteTextures(1, &tex->id);
    free(tex);
}