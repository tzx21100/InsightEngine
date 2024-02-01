#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

uniform vec4 uLightColors[128]; // light color + intensity (as alpha value)
uniform float uLightRadius[128]; // light radiuses
uniform vec2 uLightWorldPos[128]; // light world positions
uniform int uNoOfWorldLights; // light count
uniform vec4 uLineSegments[128]; // line segment world positions (x,y) to (z,w)
uniform int uNoOfLineSegments; // line segment count
uniform vec2 uResolution; // resolution of game
uniform mat4 uInverseVP; // Inverse of View-Projection Matrix

uniform sampler2D bg_tex; // background framebuffer texture
uniform sampler2D id_tex; // ID framebuffer texture
uniform sampler2D shadowMap; // Shadow map texture

// Function to check if two line segments intersect
bool doLineSegmentsIntersect(vec2 p0, vec2 p1, vec2 q0, vec2 q1) {
    vec2 r = p1 - p0;
    vec2 s = q1 - q0;

    float crossProduct = r.x * s.y - r.y * s.x;
    vec2 qMinusP = q0 - p0;

    float t = (qMinusP.x * s.y - qMinusP.y * s.x) / crossProduct;
    float u = (qMinusP.x * r.y - qMinusP.y * r.x) / crossProduct;

    return (crossProduct != 0.0) && (t >= 0.0) && (t <= 1.0) && (u >= 0.0) && (u <= 1.0);
}

void main()
{
    vec2 texY = vTexCoord;
    texY.y = 1.0 - texY.y;
    vec4 final_frag_clr = texture(bg_tex, texY);


    fFragColor = final_frag_clr;
    return;
    float attenuation = 1.0;

    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Convert pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // Transform to world coordinates
    vec2 current_pixel = worldPos.xy;

    bool intersectionFound = false;

    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 worldLightPos = uLightWorldPos[i];
        float dist = length(current_pixel - worldLightPos);
        float radius = uLightRadius[i] * 0.5; // which is in world pos
        float inCircle = step(dist, radius);

        vec4 lightColor = uLightColors[i]; // Get the light color and intensity
        vec3 attenuationColor = lightColor.rgb * lightColor.a; // Calculate the attenuation color

        for (int j = 0; j < uNoOfLineSegments; ++j) {
            vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
            vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);

            if (doLineSegmentsIntersect(worldLightPos, current_pixel, p0, p1)) {
                attenuationColor *= 0.5; // Apply attenuation for shadows
            }
        }

        final_frag_clr.rgb += vColor.rgb * attenuationColor * inCircle;
    }

    fFragColor = final_frag_clr;
    // fFragColor = vec4(1.0,0,0,1.0);
    float id = texelFetch(id_tex, ivec2(vTexCoord * textureSize(id_tex, 0)), 0).r;
    fEntityID = int(id);
}
