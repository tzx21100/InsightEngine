#version 450 core
layout(location = 0) in vec4  vColor;
layout(location = 1) in vec2  vTexCoord;
layout(location = 2) in flat float vTexID;
layout(location = 3) in vec2  vAnimDim;
layout(location = 4) in vec2  vAnimIndex;
layout(location = 5) in flat float vEntityID;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

uniform sampler2D uTex2d[128];
  
void main()
{
    bool textured = false;
    if (vTexID >= 0) textured = true;
    if (!textured)
    {
        fFragColor = vec4(vColor); // Use vColor if no texture is bound
    }
    else
    {
        int texIdx = int(vTexID);
        fFragColor = texture(uTex2d[texIdx], vec2(vTexCoord.x * vAnimDim.x, vTexCoord.y * vAnimDim.y) + vec2(vAnimDim.x * vAnimIndex.x, vAnimDim.y * vAnimIndex.y));
        fFragColor.a *= vColor.a;
    }
    int id = int(vEntityID);
    fEntityID = id + 1;
}