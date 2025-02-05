#version 450 core
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexCoord;

layout(location = 0) out vec2 vTexCoord;


void main()
{
    gl_Position = vec4(aVertexPosition, 1.0, 1.0);
    vTexCoord = aVertexTexCoord;
}