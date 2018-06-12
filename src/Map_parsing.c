#include "Map.h"

#include <malloc.h>
#include <math.h>
#include <string.h>
#include "Utils/MuleArray.h"
#include "Utils/FileMethods.h"
#include "Utils/Parsing.h"

bool loadSpawnerImage(Map* map, const char* imageFilePath);
bool parseCellBlock(Map* map, const char** source);
bool parseSpawnerBlock(Map* map, const char** source);
bool parseSpawnerFunction(const char** source, GameFunction** gfunc);

bool Map_parse(Map* map, const char* source)
{
    bool spawnerBlock = false, cellBlock = false;
    skipWhitespace(&source);
    while (*source != '\0')
    {
        char command[256];
        parseWord(&source, command);
        if (strcmp(command, "cell_camera") == 0)
        {
            if (!expectString(&source, " ")) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->cameraX)) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->cameraY)) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else if (strcmp(command, "money") == 0)
        {
            if (!expectString(&source, " ")) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->startingMoney)) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else if (strcmp(command, "hp") == 0)
        {
            if (!expectString(&source, " ")) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->startingHP)) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else if (!cellBlock && strcmp(command, "cells_from_image") == 0)
        {
            cellBlock = true;
            if (!expectString(&source, " ")) return false;
            getLine(&source, command);
            if (!loadSpawnerImage(map, command)) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else if (!cellBlock && strcmp(command, "cells") == 0)
        {
            cellBlock = true;
            if (!expectString(&source, " ")) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->width)) return false;
            skipWhitespaceInLine(&source);
            if (!parseInt(&source, &map->height)) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;

            map->cells = (Cell**)MuleArray_create2D(map->width, map->height, sizeof(Cell));

            //Implicit buildable
            int x, y;
            for (x = 0; x < map->width; x++)
            {
                for (y = 0; y < map->height; y++)
                {
                    map->cells[x][y].cellType = Buildable;
                }
            }

            if (!parseCellBlock(map, &source)) return false;
            
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else if (!spawnerBlock && strcmp(command, "spawner") == 0)
        {
            spawnerBlock = true;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
            
            if (!parseSpawnerBlock(map, &source)) return false;

            if (!expectString(&source, "endspawner")) return false;
            if (!skipWhitespaceExpectLineEnd(&source)) return false;
        }
        else return false;

        skipWhitespace(&source);
    }

    return cellBlock && spawnerBlock;
}

bool loadSpawnerImage(Map* map, const char* imageFilePath)
{
    unsigned char* data = readImage(imageFilePath, &map->width, &map->height);

    if (data == NULL) return false;

    map->cells = (Cell**)MuleArray_create2D(map->width, map->height, sizeof(Cell));

    int i;
    for (i = 0; i < map->width * map->height; i++)
    {
        switch (*((int*)&data[i * 4]))
        {
            case 0xFF0000FF:
                map->cells[i % map->width][i / map->width].cellType = Spawner;
                break;
            case 0xFF00FFFF:
                map->cells[i % map->width][i / map->width].cellType = Path;
                break;
            case 0xFF00FF00:
                map->cells[i % map->width][i / map->width].cellType = Goal;
                break;
            case 0xFF000000:
                map->cells[i % map->width][i / map->width].cellType = Unbuildable;
                break;
            default:
                map->cells[i % map->width][i / map->width].cellType = Buildable;
                break;
        }
    }

    return true;
}

bool parseSpawnerFunction(const char** source, GameFunction** gfunc)
{
    char command[256];
    skipWhitespaceInLine(source);
    parseWord(source, command);
    int expectedParamCount;
    FuncPtr func;
    if (strcmp(command, "constant") == 0)
    {
        func = constFunc;
        expectedParamCount = 1;
    }
    else if (strcmp(command, "linear") == 0)
    {
        func = linearFunc;
        expectedParamCount = 2;
    }
    else if (strcmp(command, "quadratic") == 0)
    {
        func = quadraticFunc;
        expectedParamCount = 2;
    }
    else if (strcmp(command, "power") == 0)
    {
        func = powFunc;
        expectedParamCount = 3;
    }
    else if (strcmp(command, "exponential") == 0)
    {
        func = expFunc;
        expectedParamCount = 3;
    }
    else if (strcmp(command, "logarithmic") == 0)
    {
        func = logFunc;
        expectedParamCount = 3;
    }
    else return false;

    *gfunc = GameFunction_construct(expectedParamCount);
    (*gfunc)->func = func;

    int i = 0;
    for (i = 0; i < expectedParamCount; i++)
    {
        skipWhitespaceInLine(source);
        if (!parseDouble(source, &(*gfunc)->params[i])) return false;
    }

    if (!skipWhitespaceExpectLineEnd(source)) return false;

    return true;
}

bool parseSpawnerBlock(Map* map, const char** source)
{
    skipWhitespace(source);
    char command[256];
    parseWord(source, command);
    if (strcmp(command, "hp_func") == 0 && !parseSpawnerFunction(source, &map->hpFunc))
        return false;

    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "hp_deviation_func") == 0 && !parseSpawnerFunction(source, &map->hpDevFunc))
        return false;
    
    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "speed_func") == 0 && !parseSpawnerFunction(source, &map->speedFunc))
        return false;

    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "speed_deviation_func") == 0 && !parseSpawnerFunction(source, &map->speedDevFunc))
        return false;

    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "spawn_rate_func") == 0 && !parseSpawnerFunction(source, &map->spawnRateFunc))
        return false;

    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "money_func") == 0 && !parseSpawnerFunction(source, &map->moneyFunc))
        return false;
    
    skipWhitespace(source);
    parseWord(source, command);
    if (strcmp(command, "num_func") == 0 && !parseSpawnerFunction(source, &map->numFunc))
        return false;

    skipWhitespace(source);

    return true;
}

bool parseCellBlock(Map* map, const char** source)
{
    skipWhitespace(source);
    while (*source != '\0')
    {
        char command[256];
        int x, y;
        parseWord(source, command);
        if (strcmp(command, "endcells") == 0) return true;
        else if (strcmp(command, "cell") == 0)
        {
            skipWhitespaceInLine(source);
            if (!parseInt(source, &x)) return false;
            skipWhitespaceInLine(source);
            if (!parseInt(source, &y)) return false;
            parseWord(source, command);
            if (strcmp(command, "buildable") == 0) map->cells[x][y].cellType = Buildable;
            else if (strcmp(command, "unbuildable") == 0) map->cells[x][y].cellType = Unbuildable;
            else if (strcmp(command, "path") == 0) map->cells[x][y].cellType = Path;
            else if (strcmp(command, "spawner") == 0) map->cells[x][y].cellType = Spawner;
            else if (strcmp(command, "goal") == 0) map->cells[x][y].cellType = Goal;
            else return false;
            if (!skipWhitespaceExpectLineEnd(source)) return false;
        }
        else return false;

        skipWhitespace(source);
    }
    return false;
}