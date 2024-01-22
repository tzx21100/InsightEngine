#version 450 core
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexCoord;
layout(location = 2) in vec4 aVertexColor;
layout(location = 4) in vec4 aMtxRow1;
layout(location = 5) in vec4 aMtxRow2;
layout(location = 6) in vec4 aMtxRow3;
layout(location = 7) in vec4 aMtxRow4;
layout(location = 10) in float aEntityID;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 5) out flat float vEntityID;


void main()
{
    mat4 model_to_NDC_xform = mat4(aMtxRow1, aMtxRow2, aMtxRow3, aMtxRow4);
    gl_Position = model_to_NDC_xform * vec4(aVertexPosition, 1.0, 1.0);
    vTexCoord = aVertexTexCoord;
	vColor = aVertexColor;
    vEntityID = aEntityID;
}