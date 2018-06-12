#include "BMFont.h"

#include <malloc.h>
#include <string.h>
#include "../Utils/FileMethods.h"
#include "../Utils/Parsing.h"

BMFont* BMFont_construct(Texture* texture, int charCount, int lineHeight, int base)
{
    BMFont* font = (BMFont*)malloc(sizeof(BMFont));
    font->texture = texture;
    font->chars = (BMChar*)malloc(sizeof(BMChar) * charCount);
    font->charCount = charCount;
    font->lineHeight = lineHeight;
    font->base = base;
    return font;
}

BMFont* BMFont_load(const char* fontName, int unknownCharId)
{
    char* path = (char*)malloc(strlen(fontName) + 16);
    path = getDirectoryName(path, fontName);
    char* file = readText(fontName);
    const char* source = file;
    int lineHeight, base, charCount;

    if (!expectString(&source, "info")) return NULL;
    
    //Skip the entire line
    for (; *source != '\n' && *source != '\0'; source++);
    if (*source == '\0') return NULL;
    source++;

    if (!expectString(&source, "common")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "lineHeight")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "=")) return NULL;
    skipWhitespaceInLine(&source);
    if (!parseInt(&source, &lineHeight))  return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "base")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "=")) return NULL;
    skipWhitespaceInLine(&source);
    if (!parseInt(&source, &base)) return NULL;
    
    //Skip the rest of the line
    for (; *source != '\n' && *source != '\0'; source++);
    if (*source == '\0') return NULL;
    source++;
    
    if (!expectString(&source, "page")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "id")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "=")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "0")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "file")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "=")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "\"")) return NULL;
    
    //Read until quote
    char* iter = path + strlen(path);
    for (; *source != '"' && *source != '\0'; source++)
    {
        *iter++ = *source;
    }
    *iter = '\0';

    if (!expectString(&source, "\"")) return NULL;
    if (!skipWhitespaceExpectLineEnd(&source)) return NULL;

    if (!expectString(&source, "chars")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "count")) return NULL;
    skipWhitespaceInLine(&source);
    if (!expectString(&source, "=")) return NULL;
    skipWhitespaceInLine(&source);
    if (!parseInt(&source, &charCount)) return NULL;
    if (!skipWhitespaceExpectLineEnd(&source)) return NULL;

    Texture* texture = Texture_load(path);
    BMFont* font = BMFont_construct(texture, charCount, lineHeight, base);

    int i;
    for (i = 0; i < charCount; i++)
    {
        int x, y;
        if (!expectString(&source, "char")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "id")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].id)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "x")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &x)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "y")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &y)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "width")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].width)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "height")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].height)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "xoffset")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].xoffset)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "yoffset")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].yoffset)) return NULL;
        skipWhitespaceInLine(&source);

        if (!expectString(&source, "xadvance")) return NULL;
        skipWhitespaceInLine(&source);
        if (!expectString(&source, "=")) return NULL;
        skipWhitespaceInLine(&source);
        if (!parseInt(&source, &font->chars[i].xadvance)) return NULL;

        font->chars[i].uvTL.x = (float)x / font->texture->width;
        font->chars[i].uvTL.y = (float)y / font->texture->height;
        font->chars[i].uvBR.x = (float)(x + font->chars[i].width) / font->texture->width;
        font->chars[i].uvBR.y = (float)(y + font->chars[i].height) / font->texture->height;

        //Skip the rest of the line
        for (; *source != '\n' && *source != '\0'; source++);
        if (*source == '\0' && i < charCount - 1) return NULL;
        source++;
    }

    font->unknownChar = BMFont_getCharIndex(font, unknownCharId);

    free(path);
    free(file);
    
    return font;
}

void BMFont_prepareString(BMFont* font, int* str)
{
    for (;*str != 0; str++)
    {
        if (*str < 32) continue;
        *str = BMFont_getCharIndex(font, *str) + 1;
    }
}

Vector2 BMFont_measure(BMFont* font, int* bmStr, float scale)
{
    Vector2 position = Vector2_construct(0, 0);
    float startX = 0;
    float maxX = 0;
    float maxY = 0;

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
            float xoffs = font->chars[*bmStr - 1].xoffset * scale + font->chars[*bmStr - 1].width * scale;
            float yoffs = font->chars[*bmStr - 1].yoffset * scale + font->chars[*bmStr - 1].height * scale;
            if (position.x + xoffs > maxX)
                maxX = position.x + xoffs;

            if (position.y + yoffs > maxY)
                maxY = position.y + yoffs;
        }
        
        position.x += font->chars[*bmStr - 1].xadvance * scale;
    }

    return Vector2_construct(maxX, maxY);
}

int BMFont_getCharIndex(BMFont* font, int id)
{
    if (id < 32) return id;
    
    int i = 0;
    for (i = 0; i < font->charCount; i++)
    {
        if (font->chars[i].id == id) return i;
    }

    return font->unknownChar;
}

void BMFont_delete(BMFont* font)
{
    Texture_delete(font->texture);
    free(font->chars);
    free(font);
}