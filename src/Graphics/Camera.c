#include "Camera.h"

#include <malloc.h>
#include <GL/glew.h>
#include <malloc.h>

Camera* Camera_construct()
{
    return (Camera*)malloc(sizeof(Camera));
}

void Camera_resize(Camera* camera, int windowWidth, int windowHeight)
{
    camera->windowWidth = windowWidth;
    camera->windowHeight = windowHeight;
    camera->screenRectangle = Rectangle_construct(0, 0, camera->windowWidth, camera->windowHeight, 0);
    camera->projection = Matrix4_createOrthographicProjection(0, 0, camera->windowWidth, camera->windowHeight, -1, 1);
}

void Camera_updateViewMatrix(Camera* camera)
{
    camera->view = Matrix4_createTranslation2Ds(camera->windowWidth/2, camera->windowHeight/2);
    camera->view = Matrix4_mul(camera->view, Matrix4_createScale2Ds(camera->zoom, camera->zoom));
    camera->view = Matrix4_mul(camera->view, Matrix4_createTranslation2Ds(-camera->panning.x, -camera->panning.y));
    camera->screenToWorld = Matrix4_inverse(camera->view);
}

void Camera_pan(Camera* camera, Vector2 add)
{
    camera->panning.x += add.x;
    camera->panning.y += add.y;

    Camera_updateViewMatrix(camera);
}

void Camera_zoom(Camera* camera, float add)
{
    camera->zoom += add;
    if (camera->zoom > 1) camera->zoom = 1;
    if (camera->zoom < 0.25) camera->zoom = 0.25;

    Camera_updateViewMatrix(camera);
}

bool Camera_screenContains(Camera* camera, Rectangle rect)
{
    return Rectangle_intersectsRect(camera->screenRectangle, rect);
}

void Camera_delete(Camera* camera)
{
    free(camera);
}