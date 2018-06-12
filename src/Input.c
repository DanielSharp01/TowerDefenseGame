#include "Input.h"

void clearKeyboardState(KeyboardState* state)
{
    int i;
    for (i = 0; i < GLFW_KEY_LAST + 1; i++)
        state->keys[i] = false;
}

void clearMouseState(MouseState* state)
{
    int i;
    state->position.x = 0;
    state->position.y = 0;
    state->scrollOffset.x = 0;
    state->scrollOffset.y = 0;
    for (i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
        state->buttons[i] = false;
}

bool isKeyDown(int key)
{
    return currentKeyboard.keys[key];
}

bool isKeyUp(int key)
{
    return !currentKeyboard.keys[key];
}

bool isKeyPressed(int key)
{
    return !lastKeyboard.keys[key] && currentKeyboard.keys[key];
}

bool isKeyReleased(int key)
{
    return lastKeyboard.keys[key] && !currentKeyboard.keys[key];
}

bool isMButtonDown(int button)
{
    return currentMouse.buttons[button];
}

bool isMButtonUp(int button)
{
    return !currentMouse.buttons[button];
}

bool isMButtonPressed(int button)
{
    return !lastMouse.buttons[button] && currentMouse.buttons[button];
}

bool isMButtonReleased(int button)
{
    return lastMouse.buttons[button] && !currentMouse.buttons[button];
}

bool isAnythingPressed()
{
    int i;
    for (i = 0; i < GLFW_KEY_LAST + 1; i++)
        if (isKeyPressed(i)) return true;

    for (i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
        if (isMButtonPressed(i)) return true;
    
    return false;
}

Vector2 getMousePosition()
{
    return currentMouse.position;
}

Vector2 getMouseScrollOffset()
{
    return Vector2_sub(currentMouse.scrollOffset, lastMouse.scrollOffset);
}

Vector2 getMousePositionWorld()
{
    return currentMouse.positionWorld;
}