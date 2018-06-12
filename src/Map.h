#ifndef MAP_H
#define MAP_H

#include "Game.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Math/Matrix4.h"
#include "Graphics/Texture.h"
#include "Graphics/SpriteBatch.h"
#include "Lists/Vector2List.h"
#include "GameFunction.h"

typedef struct Tower Tower;

enum CellType { Buildable, Unbuildable, Path, Spawner, Goal };
typedef struct Cell
{
    int x, y;
    Sprite* sprite;
    enum CellType cellType;
    Tower* tower;
} Cell;

typedef struct Map
{
    GameContext* context;
    Cell** cells;
    int width, height;
    
    int cameraX, cameraY;
    Vector2List* path;
    int startingMoney;
    int startingHP;
    GameFunction* hpFunc;
    GameFunction* hpDevFunc;
    GameFunction* speedFunc;
    GameFunction* speedDevFunc;
    GameFunction* spawnRateFunc;
    GameFunction* moneyFunc;
    GameFunction* numFunc;
} Map;

Map* Map_construct(GameContext* context, const char* mapFile);

bool Map_parse(Map* map, const char* source);

void Map_render(Map* map, double delta);

void Map_delete(Map* map);

#endif