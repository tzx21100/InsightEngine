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
    vec4 final_frag_clr = texture(bg_tex, vTexCoord); // Start with base color from background texture
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Convert pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // Transform to world coordinates
    vec2 current_pixel = worldPos.xy / worldPos.w; // Correct by perspective division

    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 lightDirection = uLightWorldPos[i] - current_pixel;
        float distance = length(lightDirection);
        float radius = uLightRadius[i];

        if (distance < radius) {
            float attenuation = pow(1.0 - (distance / radius), 2.0); // Squared falloff attenuation
            vec4 lightColor = uLightColors[i] * attenuation; // Apply attenuation to light color

            bool inShadow = false;
            for (int j = 0; j < uNoOfLineSegments && !inShadow; ++j) {
                vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
                vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);
                if (doLineSegmentsIntersect(uLightWorldPos[i], current_pixel, p0, p1)) {
                    inShadow = true;
                    lightColor *= 0.5; // Reduce intensity by half if in shadow
                }
            }

            final_frag_clr.rgb += lightColor.rgb; // Add light color to final fragment color
        }
    }

    fFragColor = final_frag_clr;
    float id = texelFetch(id_tex, ivec2(gl_FragCoord.xy), 0).r;
    fEntityID = int(id); // Set entity ID from ID texture
}