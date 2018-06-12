#include "FileMethods.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

char* readText(const char* path)
{
    FILE* file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* contents = (char*)malloc(length + 1);
    memset(contents, 0, length + 1);
    fread(contents, 1, length, file);
    contents[length] = '\0';
    fclose(file);
    return contents;
}

unsigned char* readImage(const char* path, int* width, int* height)
{
    int channelNum;
    return stbi_load(path, width, height, &channelNum, 0);
}

void freeImage(unsigned char* rawImage)
{
    stbi_image_free(rawImage);
}

char* getDirectoryName(char* dst, const char* src)
{
    int len = strlen(src);
    memcpy(dst, src, len);
    char* iter = dst + len - 1;
    while (iter >= dst && *iter != '/' && *iter != '\\')
    {
        *iter = '\0';
        iter--;
    }
    return dst;
}