#version 450 core
layout(location = 0) in vec4 vColor;
layout(location = 0) out vec4 fFragColor;

void main()
{
    fFragColor = vec4(vColor);
}