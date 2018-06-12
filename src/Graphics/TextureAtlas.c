#include "TextureAtlas.h"

#include <malloc.h>
#include "../Utils/MuleArray.h"

TextureAtlas* TextureAtlas_construct(Texture* base, int cols, int rows, int width, int height, int paddingX, int paddingY)
{
    TextureAtlas* atlas = (TextureAtlas*)malloc(sizeof(TextureAtlas));
    atlas->base = base;
    atlas->subimages = (Texture***)MuleArray_create2DPointer(cols, rows);
    atlas->cols = cols;
    atlas->rows = rows;
    int x, y;
    int tx = 0, ty = 0;
    for (x = 0; x < cols; x++)
    {
        tx += paddingX;
        for (y = 0; y < rows; y++)
        {
            ty += paddingY;
            atlas->subimages[x][y] = Texture_constructSubimage(base, tx, ty, width, height);
            ty += width;
        }
        ty = 0;
        tx += height;
    }

    return atlas;
}

void TextureAtlas_delete(TextureAtlas* atlas)
{
    int x, y;
    for (x = 0; x < atlas->cols; x++)
    {
        for (y = 0; y < atlas->rows; y++)
        {
            Texture_delete(atlas->subimages[x][y]);
        }
    }
    MuleArray_delete2DPointer((void***)atlas->subimages);
    Texture_delete(atlas->base);
    free(atlas);
}