#ifndef SHADER_H
#define SHADER_H

#include "../Math/Matrix4.h"
#include "Texture.h"

typedef unsigned int Shader;

Shader Shader_construct(const char* vertSrc, const char* fragSrc);
void Shader_delete(Shader shader);
void BoundShader_setProjection(Matrix4 mat);
void BoundShader_setView(Matrix4 mat);

#endif