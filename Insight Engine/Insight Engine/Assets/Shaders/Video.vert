#version 450 core
layout(location = 0)  in vec2 aVertexPosition;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 uModel_to_ndc_xform;
out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = uModel_to_ndc_xform * vec4(aVertexPosition, 0.0, 1.0);
}