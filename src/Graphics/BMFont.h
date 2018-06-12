#ifndef BM_FONT_H
#define BM_FONT_H

#include "Texture.h"
#include "../Math/Vector2.h"

typedef struct BMChar
{
    int id;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
    Vector2 uvTL;
    Vector2 uvBR;
} BMChar;

typedef struct BMFont
{
    Texture* texture;
    BMChar* chars;
    int unknownChar;
    int charCount;
    int lineHeight;
    int base;
} BMFont;

BMFont* BMFont_construct(Texture* texture, int charCount, int lineHeight, int base);
BMFont* BMFont_load(const char* fontName, int unknownCharId);
void BMFont_prepareString(BMFont* font, int* str);
Vector2 BMFont_measure(BMFont* font, int* bmStr, float scale);
int BMFont_getCharIndex(BMFont* font, int id);
void BMFont_delete(BMFont* font);

#endif