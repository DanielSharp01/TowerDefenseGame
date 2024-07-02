#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Math/Vector2.h"
#include "Math/Matrix4.h"

typedef struct KeyboardState
{
    bool keys[GLFW_KEY_LAST + 1];
} KeyboardState;

typedef struct MouseState
{
    Vector2 position;
    Vector2 positionWorld;
    Vector2 scrollOffset;
    bool buttons[GLFW_MOUSE_BUTTON_LAST + 1];
} MouseState;

extern KeyboardState lastKeyboard;
extern KeyboardState currentKeyboard;

extern MouseState lastMouse;
extern MouseState currentMouse;

void clearKeyboardState(KeyboardState *state);
void clearMouseState(MouseState *state);

bool isKeyDown(int key);
bool isKeyUp(int key);
bool isKeyPressed(int key);
bool isKeyReleased(int key);

bool isMButtonDown(int button);
bool isMButtonUp(int button);
bool isMButtonPressed(int button);
bool isMButtonReleased(int button);

bool isAnythingPressed();

Vector2 getMousePosition();
Vector2 getMousePositionWorld();
Vector2 getMouseScrollOffset();

#endif