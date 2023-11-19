#version 450 core
layout(location = 0) in vec2  aVertexPosition;
layout(location = 2) in vec4  aVertexColor;
layout(location = 4) in vec4  aMtxRow1;
layout(location = 5) in vec4  aMtxRow2;
layout(location = 6) in vec4  aMtxRow3;
layout(location = 7) in vec4  aMtxRow4;

layout(location = 0) out vec4  vColor;

void main()
{
    mat4 model_to_NDC_xform = mat4(aMtxRow1, aMtxRow2, aMtxRow3, aMtxRow4);
    gl_Position = model_to_NDC_xform * vec4(aVertexPosition, 1.0, 1.0);
	vColor = aVertexColor;
}