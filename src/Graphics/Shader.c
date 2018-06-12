#include "Shader.h"
#include <GL/glew.h>
#include <stdio.h>

Shader Shader_construct(const char* vertSrc, const char* fragSrc)
{
    unsigned int program, vs, fs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertSrc, NULL);
    glCompileShader(vs);
    int status;
    char log[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderInfoLog(vs, 512, NULL, log);
        printf("ERROR in Vertex Shader:\n%s\n", log);
        glDeleteShader(vs);
        return 0;
    }
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragSrc, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderInfoLog(fs, 512, NULL, log);
        printf("ERROR in Fragment Shader:\n%s\n", log);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("ERROR while Linking:\n%s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("ERROR while Validating:\n%s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    glUseProgram(program);

    return program;
}

void Shader_delete(Shader shader)
{
    glDeleteProgram(shader);
}

void BoundShader_setProjection(Matrix4 mat)
{
    glUniformMatrix4fv(0, 1, GL_FALSE, mat.cells);
}

void BoundShader_setView(Matrix4 mat)
{
    glUniformMatrix4fv(1, 1, GL_FALSE, mat.cells);
}