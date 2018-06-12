#version 430 core
const vec4 vertices[4] = { vec4(0, 0, 0.0, 1.0),
                           vec4(0, 1, 0.0, 1.0),
                           vec4(1, 0, 0.0, 1.0),
                           vec4(1, 1, 0.0, 1.0) };

layout (location = 0) in mat4 model;
layout (location = 4) in vec4 color;
layout (location = 5) in vec4 uv; //top left, bottom right

layout (location = 0) uniform mat4 projection;
layout (location = 1) uniform mat4 view;

out vec4 _color;
out vec2 _uv;

void main()
{
    gl_Position = projection * view * model * vertices[gl_VertexID];
    _color = color;
    if (gl_VertexID == 0)
    {
        _uv = vec2(uv.x, uv.y);
    }
    else if (gl_VertexID == 1)
    {
        _uv = vec2(uv.x, uv.w);
    }
    else if (gl_VertexID == 2)
    {
        _uv = vec2(uv.z, uv.y);
    }
    else
    {
        _uv = vec2(uv.z, uv.w);
    }
}