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
uniform float uGlobalTime;

void main()
{
    vec4 col;
    bool textured = false;
    int texIdx = int(vTexID);
    vec2 textureUV = vec2(vTexCoord.x * vAnimDim.x, vTexCoord.y * vAnimDim.y) + vec2(vAnimDim.x * vAnimIndex.x, vAnimDim.y * vAnimIndex.y);

    if (vTexID >= 0) textured = true;
    if (!textured)
    {
        col = vec4(vColor); // Use vColor if no texture is bound
    }
    else
    {
        col = texture(uTex2d[texIdx], textureUV);
        col *= vColor;

        float displacement = 0.0008;
        float effectDuration = 2.0;

        // Time-dependent global displacement offsets
        float globalXOffset = sin(uGlobalTime * 10.0) * displacement;
        float globalYOffset = cos(uGlobalTime * 10.0) * displacement;

        // Apply the global displacement to texture coordinates
        vec2 glitchTextureUV = textureUV + vec2(globalXOffset, globalYOffset);

        // Sample the color channels with misregistration
        float rChannel = texture(uTex2d[texIdx], glitchTextureUV).r;
        float gChannel = texture(uTex2d[texIdx], glitchTextureUV).g;
        float bChannel = texture(uTex2d[texIdx], textureUV).b;

        // Apply the glitch effect
        col = vec4(rChannel, gChannel, bChannel, col.a);
    }
    int id = int(vEntityID);
    fEntityID = id + 1;
    fFragColor = col;
}
