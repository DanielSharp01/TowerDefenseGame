#ifndef GAME_H
#define GAME_H

typedef struct Map Map;
typedef struct Cell Cell;
typedef struct EnemySpawner EnemySpawner;
typedef struct EnemyList EnemyList;
typedef struct BulletList BulletList;
typedef struct Map map;

#include "Graphics/TextureAtlas.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"

enum GameState
{
    Game,
    Pause,
    GameOver
};

typedef struct GameContext
{
    //Context variables for easy access
    Shader shader;
    TextureAtlas* mainAtlas;
    BMFont* font;
    SpriteBatch* batch;
    Camera* camera;
    
    //Game state variables
    enum GameState state;
    bool manualControl;
    
    //GUI variables
    bool consumedMouse;

    //Game logic variables
    Cell* selectedCell;
    int placingTower;
    int money;
    int hp;

    //GameObjects
    Map* map;
    EnemyList* enemyList;
    BulletList* bulletList;
    BulletList* particleList;
    EnemySpawner* spawner;

} GameContext;

void GameContext_delete(GameContext* context);

void Game_initialize();
void Game_resize(int width, int height);
void Game_update(double delta);
void Game_render(double delta);
void Game_terminate();

void updateGameState(double delta);
void updateGameObjects(double delta);
void updateCamera(double delta);
void updateCells(double delta);
void updateKeyboard(double delta);
void updateLogic(double delta);

bool updateGUI(double delta);
void placeTower();

void drawGameObjects(double delta);
void drawOverlay(double delta);
void drawGUI(double delta);

#endif