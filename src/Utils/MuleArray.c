#include "MuleArray.h"

#include <malloc.h>

void*** MuleArray_create2DPointer(int w, int h)
{
    void*** ret = (void***)malloc(sizeof(void**) * w);
    ret[0] = (void**)malloc(sizeof(void*) * w * h);
    int i;
    for (i = 1; i < w; i++)
    {
        ret[i] = ret[i - 1] + h;
    }

    return ret;
}

void MuleArray_delete2DPointer(void*** arr)
{
    free(arr[0]);
    free(arr);
}

void** MuleArray_create2D(int w, int h, int elementSize)
{
    void** ret = (void**)malloc(sizeof(void*) * w);
    ret[0] = (void*)malloc(elementSize * w * h);
    int i;
    for (i = 1; i < w; i++)
    {
        ret[i] = ret[i - 1] + h * elementSize;
    }

    return ret;
}

void MuleArray_delete2D(void** arr)
{
    free(arr[0]);
    free(arr);
}