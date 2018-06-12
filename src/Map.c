#include "Map.h"

#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "Math/Shapes.h"
#include "Math/AStar.h"
#include "Utils/MuleArray.h"
#include "Utils/FileMethods.h"
#include "Game.h"
#include "Tower.h"

void updateViewMatrix(Map* map);

Map* Map_construct(GameContext* context, const char* mapFile)
{
    Map* map = (Map*)malloc(sizeof(Map));
    map->context = context;

    char* source = readText(mapFile);
    map->cells = NULL;
    if (!Map_parse(map, source))
    {
        printf("Failed to load map! The program will now exit...\n");
        exit(0);
    }
    free(source);

    AStarData* data = AStarData_construct(map->width, map->height);

    int x, y;
    for (x = 0; x < map->width; x++)
    {
        for (y = 0; y < map->height; y++)
        {
            map->cells[x][y].x = x;
            map->cells[x][y].y = y;
            map->cells[x][y].tower = NULL;

            switch (map->cells[x][y].cellType)
            {
                case Path:
                data->nodes[x][y]->walkable = true;
                break;
                case Spawner:
                data->start = data->nodes[x][y];
                data->nodes[x][y]->walkable = true;
                break;
                case Goal:
                data->goal = data->nodes[x][y];
                data->nodes[x][y]->walkable = true;
                break;
                default:
                data->nodes[x][y]->walkable = false;
                break;
            }

            if (map->cells[x][y].cellType != Unbuildable)
            {
                map->cells[x][y].sprite = (Sprite*)malloc(sizeof(Sprite));
                map->cells[x][y].sprite->model = Matrix4_mul(Matrix4_createTranslation2Ds(x * 128, y * 128), Matrix4_createScale2Ds(128, 128));
                int subimg = map->cells[x][y].cellType == Buildable ? 0 : 1;
                map->cells[x][y].sprite->uvTL = context->mainAtlas->subimages[subimg][0]->uvTL;
                map->cells[x][y].sprite->uvBR = context->mainAtlas->subimages[subimg][0]->uvBR;
                
                switch (map->cells[x][y].cellType) 
                {
                    case Buildable:
                    map->cells[x][y].sprite->color = Vector4_construct(0.2, 0.2, 0.2, 0.2);
                    break;
                    case Path:
                    map->cells[x][y].sprite->color = Vector4_construct(0.4, 0.1, 0, 0.5);
                    break;
                    case Spawner:
                    map->cells[x][y].sprite->color = Vector4_construct(0.4, 0, 0, 0.5);
                    break;
                    case Goal:
                    map->cells[x][y].sprite->color = Vector4_construct(0.4, 0.2, 0, 0.5);
                    break;
                    default: //TO stop GCC warnings
                    break;
                }
            }
            else map->cells[x][y].sprite = NULL;
        }
    }

    if (data->start == NULL || data->goal == NULL)
    {
        printf("Failed to load map! The program will now exit...\n");
        exit(0);
    }

    map->path = AStar_FindPath(data);
    if (map->path == NULL)
    {
        printf("Failed to load map! The program will now exit...\n");
        exit(0);
    }

    AStarData_delete(data);

    return map;
}

void Map_render(Map* map, double delta)
{
    int x, y;
    for (x = 0; x < map->width; x++)
    {
        for (y = 0; y < map->height; y++)
        {
            Rectangle cell = Rectangle_construct(x * 128, y * 128, 128, 128, 0);
            cell = Rectangle_transform(cell, map->context->camera->view);
            if (Camera_screenContains(map->context->camera, cell))
            {
                if (map->cells[x][y].sprite != NULL)
                {
                    SpriteBatch_upload(map->context->batch, map->cells[x][y].sprite, 1);
                    if (&map->cells[x][y] == map->context->selectedCell)
                    {
                        SpriteBatch_drawS(map->context->batch, map->context->mainAtlas->subimages[0][0], Vector2_construct(x * 128 + 64, y * 128 + 64), Vector2_construct(64, 64), 1, 0, Vector4_construct(0.9, 0.9, 0.1, 0.5));
                    }
                }
            }
        }
    }
}

void Map_delete(Map* map)
{
    Vector2List_delete(map->path);
    if (map->cells != NULL)
    {
        int x, y;
        for (x = 0; x < map->width; x++)
        {
            for (y = 0; y < map->height; y++)
            {
                free(map->cells[x][y].sprite);
                if (map->cells[x][y].tower != NULL)
                    Tower_delete(map->cells[x][y].tower);
            }
        }
        MuleArray_delete2D((void**)map->cells);
    }

    GameFunction_delete(map->hpFunc);
    GameFunction_delete(map->hpDevFunc);
    GameFunction_delete(map->speedFunc);
    GameFunction_delete(map->speedDevFunc);
    GameFunction_delete(map->spawnRateFunc);
    GameFunction_delete(map->moneyFunc);
    GameFunction_delete(map->numFunc);

    free(map);
}