#include "Game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "Utils/FileMethods.h"
#include "Math/Matrix4.h"
#include "Math/VectorFuncs.h"
#include "Input.h"
#include "Lists/EnemyList.h"
#include "Lists/BulletList.h"
#include "EnemySpawner.h"
#include "Map.h"
#include "Tower.h"
#include "Program.h"

#include <string.h>
#include "Utils/IntStr.h"

GameContext *context;

void updateGameState(double delta);

void Game_initialize()
{
    context = (GameContext *)malloc(sizeof(GameContext));
    glClearColor(0.0, 0.0, 0.0, 1.0f);

    char *vertSrc = readText("res/simple.vs");
    char *fragSrc = readText("res/simple.fs");
    context->shader = Shader_construct(vertSrc, fragSrc);
    free(vertSrc);
    free(fragSrc);

    context->mainAtlas = TextureAtlas_construct(Texture_load("res/sprite_sheet.png"), 7, 3, 128, 128, 16, 16);

    context->font = BMFont_load("res/font.fnt", '?');
    if (context->font == NULL)
    {
        printf("Failed to load font!\n");
        exit(0);
    }

    context->camera = Camera_construct();

    // Initialize map and basic variables
    context->map = Map_construct(context, "res/sample.map");
    context->money = context->map->startingMoney;
    context->hp = context->map->startingHP;
    context->manualControl = false;
    context->selectedCell = NULL;
    context->placingTower = -1;

    // Initialize spawner
    context->spawner = EnemySpawner_construct(context->map);
    context->state = Game;
    EnemySpawner_advanceWave(context->spawner);

    // Initialize camera
    context->camera->panning.x = context->map->cameraX * 128 + 64;
    context->camera->panning.y = context->map->cameraY * 128 + 64;
    context->camera->zoom = 0.5;
    Camera_updateViewMatrix(context->camera);

    // Initialize objects lists
    context->enemyList = EnemyList_construct();
    context->bulletList = BulletList_construct();
    context->particleList = BulletList_construct();

    // Initialize some Graphics/GL stuff
    context->batch = SpriteBatch_construct(100000, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(context->shader);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(2, 0);
}

void Game_resize(int width, int height)
{
    Camera_resize(context->camera, width, height);
    BoundShader_setProjection(context->camera->projection);
}

void Game_update(double delta)
{
    switch (context->state)
    {
    case Game:
        updateGameState(delta);
        break;
    case Pause:
        if (isAnythingPressed())
        {
            context->state = Game;
        }
        break;
    case GameOver:
        if (isAnythingPressed())
        {
            exitProgram();
        }
        break;
    }
}

void Game_render(double delta)
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawGameObjects(delta);
    drawGUI(delta);

    if (context->state != Game)
    {
        drawOverlay(delta);
    }
}

void Game_terminate()
{
    GameContext_delete(context);
}

void GameContext_delete(GameContext *context)
{
    TextureAtlas_delete(context->mainAtlas);
    BMFont_delete(context->font);
    Shader_delete(context->shader);
    SpriteBatch_delete(context->batch);

    Map_delete(context->map);
    EnemyList_delete(context->enemyList);
    BulletList_delete(context->bulletList);
    BulletList_delete(context->particleList);
    EnemySpawner_delete(context->spawner);

    Camera_delete(context->camera);
    free(context);
}

void updateGameState(double delta)
{
    context->consumedMouse = updateGUI(delta);

    updateCamera(delta);
    updateKeyboard(delta);
    updateCells(delta);
    updateLogic(delta);
    updateGameObjects(delta);
}

void updateGameObjects(double delta)
{
    BulletListElem *bulIter;
    for (bulIter = context->bulletList->head->next; bulIter != context->bulletList->head; bulIter = bulIter->next)
    {
        Bullet_update(bulIter->data, delta);
    }

    for (bulIter = context->bulletList->head->next; bulIter != context->bulletList->head; bulIter = bulIter->next)
    {
        if (bulIter->data->dead)
        {
            BulletListElem *temp = bulIter;
            bulIter = bulIter->prev;
            BulletList_remove(context->bulletList, temp);
        }
    }

    for (bulIter = context->particleList->head->next; bulIter != context->particleList->head; bulIter = bulIter->next)
    {
        Bullet_update(bulIter->data, delta);
    }

    for (bulIter = context->particleList->head->next; bulIter != context->particleList->head; bulIter = bulIter->next)
    {
        if (bulIter->data->dead)
        {
            BulletListElem *temp = bulIter;
            bulIter = bulIter->prev;
            BulletList_remove(context->particleList, temp);
        }
    }

    EnemyListElem *enemyIter;
    for (enemyIter = context->enemyList->head->next; enemyIter != context->enemyList->head; enemyIter = enemyIter->next)
    {
        Enemy_update(enemyIter->data, delta);
    }

    int x, y;
    for (x = 0; x < context->map->width; x++)
    {
        for (y = 0; y < context->map->height; y++)
        {
            if (context->map->cells[x][y].tower != NULL)
                Tower_update(context->map->cells[x][y].tower, delta);
        }
    }

    for (enemyIter = context->enemyList->head->next; enemyIter != context->enemyList->head; enemyIter = enemyIter->next)
    {
        if (enemyIter->data->dead)
        {
            EnemyListElem *temp = enemyIter;
            enemyIter = enemyIter->prev;
            if (temp->data->hp <= 0)
            {
                context->money += temp->data->money;
            }
            else
            {
                context->hp--;
            }
            EnemyList_remove(context->enemyList, temp);
        }
    }
}

void updateCamera(double delta)
{
    currentMouse.positionWorld = Matrix4_transformVec2(context->camera->screenToWorld, currentMouse.position);

    if (context->manualControl)
    {
        context->camera->panning = context->selectedCell->tower->position;
        Camera_zoom(context->camera, delta * 1.5 * getMouseScrollOffset().y);
    }
    else
    {
        Vector2 pan = Vector2_construct(0, 0);

        if (!context->consumedMouse)
        {
            if (currentMouse.position.x < 32)
            {
                pan.x -= 1024 * delta;
            }
            else if (currentMouse.position.x > context->camera->windowWidth - 32 - 256)
            {
                pan.x += 1024 * delta;
            }

            if (currentMouse.position.y < 32)
            {
                pan.y -= 1024 * delta;
            }
            else if (currentMouse.position.y > context->camera->windowHeight - 32)
            {
                pan.y += 1024 * delta;
            }
        }

        if (isKeyPressed(GLFW_KEY_KP_0) || isKeyPressed(GLFW_KEY_0)) // Resetting camera
        {
            context->camera->panning.x = context->map->cameraX * 128 + 64;
            context->camera->panning.y = context->map->cameraY * 128 + 64;
            context->camera->zoom = 0.5;
            Camera_updateViewMatrix(context->camera);
        }
        else
        {
            Camera_pan(context->camera, pan);
            Camera_zoom(context->camera, delta * 1.5 * getMouseScrollOffset().y);
        }
    }
}

void updateCells(double delta)
{
    int cx = currentMouse.positionWorld.x / 128;
    int cy = currentMouse.positionWorld.y / 128;

    if (currentMouse.positionWorld.x < 0)
        cx = -1;
    if (currentMouse.positionWorld.y < 0)
        cy = -1;

    if (!context->manualControl && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !context->consumedMouse)
    {
        if (cx >= 0 && cx < context->map->width && cy >= 0 && cy < context->map->width && context->map->cells[cx][cy].cellType != Unbuildable)
            context->selectedCell = &context->map->cells[cx][cy];
        else
            context->selectedCell = NULL;

        if (context->placingTower != -1)
            placeTower();
    }
}

void updateKeyboard(double delta)
{
    if (isKeyPressed(GLFW_KEY_SPACE) && !context->spawner->running)
    {
        context->spawner->running = true;
    }

    // Range targeting
    if ((isKeyPressed(GLFW_KEY_KP_1) || isKeyPressed(GLFW_KEY_1)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        context->manualControl = false;
        context->selectedCell->tower->focus = NULL;
        if (context->selectedCell->tower->aiFunc == Tower_AI_Closest)
            context->selectedCell->tower->aiFunc = Tower_AI_Furthest;
        else
            context->selectedCell->tower->aiFunc = Tower_AI_Closest;
    }

    // Health targeting
    if ((isKeyPressed(GLFW_KEY_KP_2) || isKeyPressed(GLFW_KEY_2)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        context->manualControl = false;
        context->selectedCell->tower->focus = NULL;
        if (context->selectedCell->tower->aiFunc == Tower_AI_HighestHP)
            context->selectedCell->tower->aiFunc = Tower_AI_LowestHP;
        else
            context->selectedCell->tower->aiFunc = Tower_AI_HighestHP;
    }

    // Speed targeting
    if ((isKeyPressed(GLFW_KEY_KP_3) || isKeyPressed(GLFW_KEY_3)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        context->manualControl = false;
        context->selectedCell->tower->focus = NULL;
        if (context->selectedCell->tower->aiFunc == Tower_AI_Fastest)
            context->selectedCell->tower->aiFunc = Tower_AI_Slowest;
        else
            context->selectedCell->tower->aiFunc = Tower_AI_Fastest;
    }

    // Range upgrade
    if ((isKeyPressed(GLFW_KEY_KP_4) || isKeyPressed(GLFW_KEY_4)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rangeUpgrLev);
        if (context->selectedCell->tower->rangeUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
        {
            Tower_upgradeRange(context->selectedCell->tower);
            context->money -= cost;
        }
    }

    // Damage upgrade
    if ((isKeyPressed(GLFW_KEY_KP_5) || isKeyPressed(GLFW_KEY_5)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        int cost = Tower_upgradeCostFunc(context->selectedCell->tower->damageUpgrLev);
        if (context->selectedCell->tower->damageUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
        {
            Tower_upgradeDamage(context->selectedCell->tower);
            context->money -= cost;
        }
    }

    // Rate of fire upgrade
    if ((isKeyPressed(GLFW_KEY_KP_6) || isKeyPressed(GLFW_KEY_6)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rateUpgrLev);
        if (context->selectedCell->tower->rateUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
        {
            Tower_upgradeRate(context->selectedCell->tower);
            context->money -= cost;
        }
    }

    if (context->manualControl)
    {
        if (isKeyPressed(GLFW_KEY_ESCAPE))
        {
            context->manualControl = false;
            context->selectedCell->tower->aiFunc = context->selectedCell->tower->lastAiFunc;
        }
    }
    else
    {
        if (isKeyPressed(GLFW_KEY_ESCAPE))
        {
            context->state = Pause;
            return;
        }

        // Buy tower
        if (isKeyPressed(GLFW_KEY_T))
        {
            placeTower();
        }

        // Sell
        if (isKeyPressed(GLFW_KEY_S) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
        {
            context->money += context->selectedCell->tower->sellCost;
            Tower_delete(context->selectedCell->tower);
        }

        // Manual control
        if ((isKeyPressed(GLFW_KEY_KP_MULTIPLY) || isKeyPressed(GLFW_KEY_C)) && context->selectedCell != NULL && context->selectedCell->tower != NULL)
        {
            context->selectedCell->tower->focus = NULL;
            context->manualControl = true;
            context->selectedCell->tower->lastAiFunc = context->selectedCell->tower->aiFunc;
            context->selectedCell->tower->aiFunc = NULL;
        }
    }
}

bool updateGUI(double delta)
{
    int xoffs = context->camera->windowWidth - 256;
    Vector2 mp = currentMouse.position;
    if (!context->spawner->running)
    {
        Rectangle swaveButton = Rectangle_construct(xoffs - 200, 32, 160, 32, 0);
        if (Rectangle_containsPoint(swaveButton, mp))
        {
            if (isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
            {
                context->spawner->running = true;
            }
            return true;
        }
    }
    if (mp.x < context->camera->windowWidth - 256)
        return false;
    if (context->manualControl)
        return false;

    if (context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        // Range upgrade
        Rectangle upgradeButton1 = Rectangle_construct(xoffs + 16, 276, 64, 64, 0);
        if (Rectangle_containsPoint(upgradeButton1, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rangeUpgrLev);
            if (context->selectedCell->tower->rangeUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
            {
                Tower_upgradeRange(context->selectedCell->tower);
                context->money -= cost;
            }
        }

        // Damage upgrade
        Rectangle upgradeButton2 = Rectangle_construct(xoffs + 96, 276, 64, 64, 0);
        if (Rectangle_containsPoint(upgradeButton2, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->damageUpgrLev);
            if (context->selectedCell->tower->damageUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
            {
                Tower_upgradeDamage(context->selectedCell->tower);
                context->money -= cost;
            }
        }

        // Rate of fire upgrade
        Rectangle upgradeButton3 = Rectangle_construct(xoffs + 176, 276, 64, 64, 0);
        if (Rectangle_containsPoint(upgradeButton3, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rateUpgrLev);
            if (context->selectedCell->tower->rateUpgrLev < TOWER_MAX_UPGRADE_LEVEL && context->money >= cost)
            {
                Tower_upgradeRate(context->selectedCell->tower);
                context->money -= cost;
            }
        }

        // Range targeting
        Rectangle aiFuncButton1 = Rectangle_construct(xoffs + 16, 356, 64, 64, 0);
        if (Rectangle_containsPoint(aiFuncButton1, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            context->selectedCell->tower->focus = NULL;
            if (context->selectedCell->tower->aiFunc == Tower_AI_Closest)
                context->selectedCell->tower->aiFunc = Tower_AI_Furthest;
            else
                context->selectedCell->tower->aiFunc = Tower_AI_Closest;
        }

        // Health targeting
        Rectangle aiFuncButton2 = Rectangle_construct(xoffs + 96, 356, 64, 64, 0);
        if (Rectangle_containsPoint(aiFuncButton2, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            context->selectedCell->tower->focus = NULL;
            if (context->selectedCell->tower->aiFunc == Tower_AI_HighestHP)
                context->selectedCell->tower->aiFunc = Tower_AI_LowestHP;
            else
                context->selectedCell->tower->aiFunc = Tower_AI_HighestHP;
        }

        // Speed targeting
        Rectangle aiFuncButton3 = Rectangle_construct(xoffs + 176, 356, 64, 64, 0);
        if (Rectangle_containsPoint(aiFuncButton3, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            context->selectedCell->tower->focus = NULL;
            if (context->selectedCell->tower->aiFunc == Tower_AI_Fastest)
                context->selectedCell->tower->aiFunc = Tower_AI_Slowest;
            else
                context->selectedCell->tower->aiFunc = Tower_AI_Fastest;
        }

        // Manual control
        Rectangle mcontrolButton = Rectangle_construct(xoffs + 96, 436, 64, 64, 0);
        if (Rectangle_containsPoint(mcontrolButton, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            context->selectedCell->tower->focus = NULL;
            context->manualControl = true;
            context->selectedCell->tower->lastAiFunc = context->selectedCell->tower->aiFunc;
            context->selectedCell->tower->aiFunc = NULL;
        }

        // Sell
        Rectangle sellButton = Rectangle_construct(xoffs + 96, 516, 64, 64, 0);
        if (Rectangle_containsPoint(sellButton, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            context->money += context->selectedCell->tower->sellCost;
            Tower_delete(context->selectedCell->tower);
        }
    }
    else if (context->selectedCell == NULL || context->selectedCell->cellType == Buildable)
    {
        Rectangle towerButton = Rectangle_construct(xoffs + 96, 132, 64, 64, 0);
        if (Rectangle_containsPoint(towerButton, mp) && isMButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            if (context->selectedCell == NULL)
            {
                if (context->placingTower == 0)
                    context->placingTower = -1;
                else if (context->money >= TOWER_COST)
                    context->placingTower = 0;
            }
            else
                placeTower();
        }
    }

    return true;
}

void updateLogic(double delta)
{
    // Enemy spawning
    EnemySpawner_update(context->spawner, delta);
    if (context->spawner->remainingEnemies == 0 && context->enemyList->count == 0)
    {
        EnemySpawner_advanceWave(context->spawner);
    }

    // Game over check
    if (context->hp <= 0)
    {
        context->state = GameOver;
    }
}

void placeTower()
{
    if (context->selectedCell != NULL && context->selectedCell->cellType == Buildable && context->money >= TOWER_COST)
    {
        context->money -= TOWER_COST;
        Tower_construct(context, context->selectedCell);
    }

    context->placingTower = -1;
}

void drawGameObjects(double delta)
{
    SpriteBatch_map(context->batch);
    Map_render(context->map, delta);

    BulletListElem *bulIter;
    for (bulIter = context->bulletList->head->next; bulIter != context->bulletList->head; bulIter = bulIter->next)
    {
        Bullet_draw(bulIter->data, delta);
    }

    for (bulIter = context->particleList->head->next; bulIter != context->particleList->head; bulIter = bulIter->next)
    {
        Bullet_draw(bulIter->data, delta);
    }

    EnemyListElem *enemyIter;
    for (enemyIter = context->enemyList->head->next; enemyIter != context->enemyList->head; enemyIter = enemyIter->next)
    {
        Enemy_draw(enemyIter->data, delta);
    }

    int x, y;
    for (x = 0; x < context->map->width; x++)
    {
        for (y = 0; y < context->map->height; y++)
        {
            if (context->map->cells[x][y].tower != NULL)
                Tower_draw(context->map->cells[x][y].tower, delta);
        }
    }

    SpriteBatch_unmap(context->batch);
    BoundShader_setView(context->camera->view);
    Texture_bind(context->mainAtlas->base);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, context->batch->instanceCount);
}

void drawOverlay(double delta)
{
    SpriteBatch_map(context->batch);
    SpriteBatch_drawRectangle(context->batch, Vector2_construct(0.921875, 0.15625), 0, 0, context->camera->windowWidth, context->camera->windowHeight, Vector4_construct(0.0, 0.0, 0.0, 0.8));
    SpriteBatch_unmap(context->batch);
    BoundShader_setView(Matrix4_createIdentity());
    Texture_bind(context->mainAtlas->base);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, context->batch->instanceCount);

    int *str = (int *)malloc(sizeof(int) * 32);

    SpriteBatch_map(context->batch);

    if (context->state == Pause)
        IntStr_fromCStr(str, "PAUSED");
    else
        IntStr_fromCStr(str, "GAME OVER");
    BMFont_prepareString(context->font, str);
    SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(context->camera->windowWidth / 2, context->camera->windowHeight / 2 * 0.8), 1.2, Vector4_construct(1, 1, 1, 1), 0.5, 0.5);

    if (context->state == Pause)
        IntStr_fromCStr(str, "Press anything to continue");
    else
        IntStr_fromCStr(str, "Press anything to exit");
    BMFont_prepareString(context->font, str);
    SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(context->camera->windowWidth / 2, context->camera->windowHeight / 2 * 0.8 + 36), 0.8, Vector4_construct(1, 1, 1, 1), 0.5, 0.5);

    SpriteBatch_unmap(context->batch);
    BoundShader_setView(Matrix4_createIdentity());
    Texture_bind(context->font->texture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, context->batch->instanceCount);

    free(str);
}

void drawGUI(double delta)
{
    int xoffs = context->camera->windowWidth - 256;
    Vector4 color;

    SpriteBatch_map(context->batch);

    if (!context->spawner->running)
    {
        // Start wave button box
        SpriteBatch_drawRectangle(context->batch, Vector2_construct(0.921875, 0.15625), xoffs - 200, 32, 160, 32, Vector4_construct(0.01, 0.01, 0.01, 1));
        SpriteBatch_drawRectangleOutline(context->batch, Vector2_construct(0.921875, 0.15625), xoffs - 200, 32, 160, 32, 1, Vector4_construct(1, 1, 1, 1));
    }

    if (!context->manualControl)
    {
        // Menu box
        SpriteBatch_drawRectangle(context->batch, Vector2_construct(0.921875, 0.15625), xoffs, 0, 256, context->camera->windowHeight, Vector4_construct(0.01, 0.01, 0.01, 1));
        // Menu box side
        SpriteBatch_drawRectangle(context->batch, Vector2_construct(0.921875, 0.15625), xoffs, 0, 1, context->camera->windowHeight, Vector4_construct(1, 1, 1, 1));

        // Dollar sign
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[1][1], Vector2_construct(xoffs + 24, 56), Vector2_construct(64, 64), 0.25, 0, Vector4_construct(1, 1, 1, 1));
        // Heart
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][1], Vector2_construct(xoffs + 24, 88), Vector2_construct(64, 64), 0.25, 0, Vector4_construct(1, 1, 1, 1));

        // 1st Horizontal Divider
        SpriteBatch_drawRectangle(context->batch, Vector2_construct(0.921875, 0.15625), xoffs, 116, 256, 1, Vector4_construct(1, 1, 1, 1));
    }
    if (!context->manualControl && context->selectedCell != NULL && context->selectedCell->cellType == Spawner)
    {
        // Description box
        SpriteBatch_drawRectangleOutline(context->batch, Vector2_construct(0.921875, 0.15625), xoffs + 16, 132, 224, 92, 0.75, Vector4_construct(0.3, 0.3, 0.3, 0.3));

        // Remaining enemies - icon
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[4][0], Vector2_construct(xoffs + 36, 160), Vector2_construct(64, 64), 0.34, 0, Vector4_construct(0.9, 0.1, 0.1, 0.9));

        // Spawn rate - icon
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[1][2], Vector2_construct(xoffs + 36, 196), Vector2_construct(64, 64), 0.28, 0, Vector4_construct(1, 1, 1, 1));
    }
    else if (!context->manualControl && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        // Description box
        SpriteBatch_drawRectangleOutline(context->batch, Vector2_construct(0.921875, 0.15625), xoffs + 16, 132, 224, 128, 0.75, Vector4_construct(0.3, 0.3, 0.3, 0.3));

        // Range - icon
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[6][1], Vector2_construct(xoffs + 36, 160), Vector2_construct(64, 64), 0.28, 0, Vector4_construct(1, 1, 1, 1));

        // Damage - icon
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][2], Vector2_construct(xoffs + 36, 196), Vector2_construct(64, 64), 0.28, 0, Vector4_construct(1, 1, 1, 1));

        // Fire rate - icon
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[1][2], Vector2_construct(xoffs + 36, 232), Vector2_construct(64, 64), 0.28, 0, Vector4_construct(1, 1, 1, 1));

        // Upgrades
        // Range
        color = Vector4_construct(0.1, 0.1, 0.1, 0.1);
        if (context->selectedCell->tower->rangeUpgrLev == TOWER_MAX_UPGRADE_LEVEL)
            color = Vector4_construct(0.9, 0.9, 0.1, 0.5);
        else if (context->money < Tower_upgradeCostFunc(context->selectedCell->tower->rangeUpgrLev))
            color = Vector4_construct(0.9, 0.1, 0.1, 0.5);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 48, 308), Vector2_construct(64, 64), 0.5, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[6][1], Vector2_construct(xoffs + 66, 326), Vector2_construct(64, 64), 0.2, 0, Vector4_construct(0.9, 0.9, 0.9, 0.9));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[2][1], Vector2_construct(xoffs + 48, 308), Vector2_construct(64, 64), 0.35, 0, Vector4_construct(0.9, 0.9, 0.1, 0.9));

        // Damage
        color = Vector4_construct(0.1, 0.1, 0.1, 0.1);
        if (context->selectedCell->tower->damageUpgrLev == TOWER_MAX_UPGRADE_LEVEL)
            color = Vector4_construct(0.9, 0.9, 0.1, 0.5);
        else if (context->money < Tower_upgradeCostFunc(context->selectedCell->tower->damageUpgrLev))
            color = Vector4_construct(0.9, 0.1, 0.1, 0.5);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 128, 308), Vector2_construct(64, 64), 0.5, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][2], Vector2_construct(xoffs + 146, 326), Vector2_construct(64, 64), 0.2, 0, Vector4_construct(0.9, 0.9, 0.9, 0.9));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[2][1], Vector2_construct(xoffs + 128, 308), Vector2_construct(64, 64), 0.35, 0, Vector4_construct(0.9, 0.9, 0.1, 0.9));

        // Rate of fire
        color = Vector4_construct(0.1, 0.1, 0.1, 0.1);
        if (context->selectedCell->tower->rateUpgrLev == TOWER_MAX_UPGRADE_LEVEL)
            color = Vector4_construct(0.9, 0.9, 0.1, 0.5);
        else if (context->money < Tower_upgradeCostFunc(context->selectedCell->tower->rateUpgrLev))
            color = Vector4_construct(0.9, 0.1, 0.1, 0.5);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 208, 308), Vector2_construct(64, 64), 0.5, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[1][2], Vector2_construct(xoffs + 226, 326), Vector2_construct(64, 64), 0.2, 0, Vector4_construct(0.9, 0.9, 0.9, 0.9));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[2][1], Vector2_construct(xoffs + 208, 308), Vector2_construct(64, 64), 0.35, 0, Vector4_construct(0.9, 0.9, 0.1, 0.9));

        // Targeting AI
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 48, 388), Vector2_construct(64, 64), 0.5, 0,
                          (context->selectedCell->tower->aiFunc == Tower_AI_Closest || context->selectedCell->tower->aiFunc == Tower_AI_Furthest) ? Vector4_construct(0.9, 0.9, 0.1, 0.5) : Vector4_construct(0.1, 0.1, 0.1, 0.1));

        if (context->selectedCell->tower->aiFunc == Tower_AI_Furthest)
            color = Vector4_construct(0.1, 0.9, 0.1, 0.9);
        else
            color = Vector4_construct(0.9, 0.1, 0.1, 0.9);

        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[5][1], Vector2_construct(xoffs + 66, 406), Vector2_construct(64, 64), 0.25, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[3][1], Vector2_construct(xoffs + 48, 388), Vector2_construct(64, 64), 0.35, 0, color);

        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 128, 388), Vector2_construct(64, 64), 0.5, 0,
                          (context->selectedCell->tower->aiFunc == Tower_AI_LowestHP || context->selectedCell->tower->aiFunc == Tower_AI_HighestHP) ? Vector4_construct(0.9, 0.9, 0.1, 0.5) : Vector4_construct(0.1, 0.1, 0.1, 0.1));

        if (context->selectedCell->tower->aiFunc == Tower_AI_LowestHP)
            color = Vector4_construct(0.9, 0.1, 0.1, 0.9);
        else
            color = Vector4_construct(0.1, 0.9, 0.1, 0.9);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[4][1], Vector2_construct(xoffs + 146, 406), Vector2_construct(64, 64), 0.25, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[3][1], Vector2_construct(xoffs + 128, 388), Vector2_construct(64, 64), 0.35, 0, color);

        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 208, 388), Vector2_construct(64, 64), 0.5, 0,
                          (context->selectedCell->tower->aiFunc == Tower_AI_Slowest || context->selectedCell->tower->aiFunc == Tower_AI_Fastest) ? Vector4_construct(0.9, 0.9, 0.1, 0.5) : Vector4_construct(0.1, 0.1, 0.1, 0.1));

        if (context->selectedCell->tower->aiFunc == Tower_AI_Slowest)
            color = Vector4_construct(0.9, 0.1, 0.1, 0.9);
        else
            color = Vector4_construct(0.1, 0.9, 0.1, 0.9);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][1], Vector2_construct(xoffs + 226, 406), Vector2_construct(64, 64), 0.25, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[3][1], Vector2_construct(xoffs + 208, 388), Vector2_construct(64, 64), 0.35, 0, color);

        // Take control button
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 128, 468), Vector2_construct(64, 64), 0.5, 0, Vector4_construct(0.1, 0.1, 0.1, 0.1));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[3][1], Vector2_construct(xoffs + 128, 468), Vector2_construct(64, 64), 0.35, 0, Vector4_construct(0.9, 0.1, 0.1, 0.9));

        // Sell button
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 128, 548), Vector2_construct(64, 64), 0.5, 0, Vector4_construct(0.1, 0.1, 0.1, 0.1));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[1][1], Vector2_construct(xoffs + 128, 538), Vector2_construct(64, 64), 0.35, 0, Vector4_construct(0.9, 0.9, 0.1, 0.9));
    }
    else if (!context->manualControl && (context->selectedCell == NULL || context->selectedCell->cellType == Buildable))
    {
        // Item 0
        color = Vector4_construct(0.1, 0.1, 0.1, 0.1);
        if (context->placingTower == 0)
            color = Vector4_construct(0.9, 0.9, 0.1, 0.5);
        else if (context->money < TOWER_COST)
            color = Vector4_construct(0.9, 0.1, 0.1, 0.5);
        else
            color = Vector4_construct(0.1, 0.1, 0.1, 0.1);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[0][0], Vector2_construct(xoffs + 128, 164), Vector2_construct(64, 64), 0.5, 0, color);
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[2][0], Vector2_construct(xoffs + 128, 154), Vector2_construct(64, 64), 0.4, 0, Vector4_construct(0.1, 0.9, 0.1, 1));
        SpriteBatch_drawS(context->batch, context->mainAtlas->subimages[3][0], Vector2_construct(xoffs + 128, 154), Vector2_construct(40, 64), 0.4, 0, Vector4_construct(0.1, 0.9, 0.1, 1));
    }

    SpriteBatch_unmap(context->batch);
    BoundShader_setView(Matrix4_createIdentity());
    Texture_bind(context->mainAtlas->base);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, context->batch->instanceCount);

    int *str = (int *)malloc(sizeof(int) * 32);
    SpriteBatch_map(context->batch);

    if (!context->spawner->running)
    {
        IntStr_fromCStr(str, "Start wave");
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs - 120, 48), 0.7, Vector4_construct(1, 1, 1, 1), 0.5, 0.5);
    }

    if (!context->manualControl)
    {
        // Wave
        IntStr_fromCStr(str, "Wave ");
        IntStr_appendInt(str, context->spawner->wave);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 14, 16), 0.625, Vector4_construct(1, 1, 1, 1));

        // Money
        IntStr_fromInt(str, context->money);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 48, 48), 0.625, Vector4_construct(1, 1, 1, 1));

        // Lives
        IntStr_fromInt(str, context->hp);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 48, 80), 0.625, Vector4_construct(1, 1, 1, 1));
    }

    if (!context->manualControl && context->selectedCell != NULL && context->selectedCell->cellType == Spawner)
    {
        IntStr_fromInt(str, context->spawner->remainingEnemies);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 60, 152), 0.6, Vector4_construct(1, 1, 1, 1));

        float spawns = context->spawner->rate;
        IntStr_fromInt(str, (int)spawns);
        IntStr_appendCStr(str, ".");
        IntStr_appendInt(str, (int)(spawns * 10) % 10);
        IntStr_appendInt(str, (int)(spawns * 100 + 0.5) % 10);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 60, 188), 0.6, Vector4_construct(1, 1, 1, 1));
    }
    else if (!context->manualControl && context->selectedCell != NULL && context->selectedCell->tower != NULL)
    {
        IntStr_fromInt(str, context->selectedCell->tower->range);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 60, 152), 0.6, Vector4_construct(1, 1, 1, 1));

        float dmg = context->selectedCell->tower->damage;
        IntStr_fromInt(str, (int)dmg);
        IntStr_appendCStr(str, ".");
        IntStr_appendInt(str, (int)(dmg * 10) % 10);
        IntStr_appendInt(str, (int)(dmg * 100 + 0.5) % 10);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 60, 188), 0.6, Vector4_construct(1, 1, 1, 1));

        float rof = context->selectedCell->tower->rateOfFire;
        IntStr_fromInt(str, (int)rof);
        IntStr_appendCStr(str, ".");
        IntStr_appendInt(str, (int)(rof * 10) % 10);
        IntStr_appendInt(str, (int)(rof * 100 + 0.5) % 10);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawString(context->batch, context->font, str, Vector2_construct(xoffs + 60, 224), 0.6, Vector4_construct(1, 1, 1, 1));

        if (context->selectedCell->tower->rangeUpgrLev < TOWER_MAX_UPGRADE_LEVEL)
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rangeUpgrLev);
            IntStr_fromCStr(str, "$");
            IntStr_appendInt(str, cost);
            BMFont_prepareString(context->font, str);
            SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs + 20, 326), 0.5,
                                        (context->money >= cost) ? Vector4_construct(1, 1, 1, 1) : Vector4_construct(0.7, 0.1, 0.1, 0.7), 0, 0.5);
        }

        if (context->selectedCell->tower->damageUpgrLev < TOWER_MAX_UPGRADE_LEVEL)
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->damageUpgrLev);
            IntStr_fromCStr(str, "$");
            IntStr_appendInt(str, cost);
            BMFont_prepareString(context->font, str);
            SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs + 100, 326), 0.5,
                                        (context->money >= cost) ? Vector4_construct(1, 1, 1, 1) : Vector4_construct(0.7, 0.1, 0.1, 0.7), 0, 0.5);
        }

        if (context->selectedCell->tower->rateUpgrLev < TOWER_MAX_UPGRADE_LEVEL)
        {
            int cost = Tower_upgradeCostFunc(context->selectedCell->tower->rateUpgrLev);
            IntStr_fromCStr(str, "$");
            IntStr_appendInt(str, cost);
            BMFont_prepareString(context->font, str);
            SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs + 180, 326), 0.5,
                                        (context->money >= cost) ? Vector4_construct(1, 1, 1, 1) : Vector4_construct(0.7, 0.1, 0.1, 0.7), 0, 0.5);
        }

        IntStr_fromCStr(str, "$");
        IntStr_appendInt(str, context->selectedCell->tower->sellCost);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs + 128, 566), 0.5, Vector4_construct(1, 1, 1, 1), 0.5, 0.5);
    }
    else if (!context->manualControl && (context->selectedCell == NULL || context->selectedCell->cellType == Buildable))
    {
        // Item 1 (cost)
        IntStr_fromCStr(str, "$");
        IntStr_appendInt(str, TOWER_COST);
        BMFont_prepareString(context->font, str);
        SpriteBatch_drawStringAlign(context->batch, context->font, str, Vector2_construct(xoffs + 128, 182), 0.5,
                                    (context->money >= TOWER_COST) ? Vector4_construct(1, 1, 1, 1) : Vector4_construct(0.7, 0.1, 0.1, 0.7), 0.5, 0.5);
    }

    SpriteBatch_unmap(context->batch);
    BoundShader_setView(Matrix4_createIdentity());
    Texture_bind(context->font->texture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, context->batch->instanceCount);

    free(str);
}