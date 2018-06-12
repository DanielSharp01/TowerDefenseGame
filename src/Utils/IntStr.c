#include "IntStr.h"

#include <string.h>
#include <malloc.h>
#include <stdio.h>

int IntStr_length(int* istr)
{
    int len = 0;
    for (; *istr !='\0'; istr++) len++;

    return len;
}

int* IntStr_fromCStr(int* dst, char* cstr)
{
    int* iter = dst;
    for (; *cstr !='\0'; cstr++)
    {
        *iter++ = *cstr;
    }
    *iter = '\0';
    return dst;
}

int* IntStr_fromInt(int* dst, int i)
{
    int* iter = dst;
    char* tmp = (char*)malloc(11);
    sprintf(tmp, "%d", i);
    char* numIter;
    for (numIter = tmp; *numIter !='\0'; numIter++)
    {
        *iter++ = *numIter;
    }
    *iter = '\0';
    free(tmp);
    return dst;
}

int* IntStr_append(int* dst, int* app)
{
    int len = IntStr_length(dst);
    memcpy(dst + len, app, len * sizeof(int));
    return dst;
}

int* IntStr_appendCStr(int* dst, char* app)
{
    int len = IntStr_length(dst);
    IntStr_fromCStr(dst + len, app);
    return dst;
}

int* IntStr_appendInt(int* dst, int i)
{
    int len = IntStr_length(dst);
    IntStr_fromInt(dst + len, i);
    return dst;
}