#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/Matrix4.h"
#include "../Math/Shapes.h"

typedef struct Camera
{
    Vector2 panning;
    float zoom;

    Matrix4 projection;
    Matrix4 view;
    Matrix4 screenToWorld;

    Rectangle screenRectangle;
    int windowWidth;
    int windowHeight;
} Camera;

Camera* Camera_construct();
void Camera_resize(Camera* camera, int windowWidth, int windowHeight);
void Camera_updateViewMatrix(Camera* camera);
void Camera_pan(Camera* camera, Vector2 add);
void Camera_zoom(Camera* camera, float add);
bool Camera_screenContains(Camera* camera, Rectangle rect);
void Camera_delete(Camera* camera);

#endif