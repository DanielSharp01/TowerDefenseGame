#version 430 core

in vec4 _color;
in vec2 _uv;

layout (location = 2) uniform sampler2D tex;

void main()
{
    vec4 texCol = texture(tex, _uv);
    texCol *= texCol.a;

    gl_FragColor = texCol * _color;
}