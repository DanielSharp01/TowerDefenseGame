#include "Program.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "Input.h"
#include <stdbool.h>

void resizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow *window, int button, int action, int mods);
void mousePosCallback(GLFWwindow *window, double x, double y);
void mouseScrollCallback(GLFWwindow *window, double xoffs, double yoffs);

double lastTime;
bool running;

int main(void)
{
    srand(time(NULL));
    GLFWwindow *window;
    if (!glfwInit())
    {
        printf("GLFW failed to initialize!");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 8);
    window = glfwCreateWindow(1280, 720, "Neon Defense", NULL, NULL);
    if (!window)
    {
        printf("GLFW failed to create a window!");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW failed to initialize!");
        return -1;
    }

    glEnable(GL_MULTISAMPLE);

    lastTime = glfwGetTime();
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);

    clearKeyboardState(&lastKeyboard);
    clearKeyboardState(&currentKeyboard);
    clearMouseState(&lastMouse);
    clearMouseState(&currentMouse);

    glfwSwapInterval(1);

    Game_initialize();
    resizeCallback(window, 1280, 720);

    running = true;
    while (!glfwWindowShouldClose(window) && running)
    {
        double currentTime = glfwGetTime();
        Game_update(currentTime - lastTime);
        Game_render(currentTime - lastTime);

        lastTime = currentTime;
        lastKeyboard = currentKeyboard;
        lastMouse = currentMouse;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Game_terminate();
    glfwTerminate();

    return 0;
}

void exitProgram()
{
    running = false;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    currentKeyboard.keys[key] = (action != GLFW_RELEASE);
}

void mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    currentMouse.buttons[button] = (action != GLFW_RELEASE);
}

void mousePosCallback(GLFWwindow *window, double x, double y)
{
    currentMouse.position.x = x;
    currentMouse.position.y = y;
}

void mouseScrollCallback(GLFWwindow *window, double xoffs, double yoffs)
{
    currentMouse.scrollOffset.x += xoffs;
    currentMouse.scrollOffset.y += yoffs;
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Game_resize(width, height);
}