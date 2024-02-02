#version 450 core

layout(location = 0) in vec2 vTexCoord;

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

void main() {
    vec4 final_frag_clr = texture(bg_tex, vTexCoord);
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Convert pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // Transform to world coordinates
    vec2 current_pixel = worldPos.xy;

    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 lightScreenPos = (uLightWorldPos[i] - current_pixel) / uResolution; // Calculate light's screen position relative to current_pixel
        float distance = length(lightScreenPos);
        float radius = uLightRadius[i] / min(uResolution.x, uResolution.y); // Adjust radius based on screen resolution
        float inCircle = step(distance, radius);

        // Harder falloff with a squared attenuation term
        float attenuation = pow(1.0 - min(distance / radius, 1.0), 2.0) * inCircle;

        bool inShadow = false;
        for (int j = 0; j < uNoOfLineSegments && !inShadow; ++j) {
            vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
            vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);
            if (doLineSegmentsIntersect(uLightWorldPos[i], current_pixel, p0, p1)) {
                inShadow = true; // Fragment is in shadow
            }
        }

        if (!inShadow) {
            final_frag_clr.rgb += uLightColors[i].rgb * attenuation; // Apply light if not in shadow
        }
    }

    fFragColor = final_frag_clr;
    float id = texelFetch(id_tex, ivec2(vTexCoord * textureSize(id_tex, 0)), 0).r;
    fEntityID = int(id) + 1; // Increment ID by 1 as per original shader logic
}