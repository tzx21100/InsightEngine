#version 450 core
layout(location = 0)  in vec2  aVertexPosition;

uniform mat4 model_to_ndc_xform;

void main()
{
    gl_Position = model_to_ndc_xform * vec4(aVertexPosition, 0.0, 1.0);
}