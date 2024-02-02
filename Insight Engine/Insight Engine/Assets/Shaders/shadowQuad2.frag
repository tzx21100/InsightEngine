#version 450 core

layout(location = 0) in vec4 vColor; // Color from vertex shader
layout(location = 1) in vec2 vTexCoord; // Texture coordinates from vertex shader
layout(location = 0) out vec4 fFragColor; // Final fragment color
layout(location = 1) out int fEntityID; // Entity ID for picking

uniform vec4 uLightColors[128]; // Array of light colors
uniform float uLightRadius[128]; // Array of light radii
uniform vec4 uLineSegments[128]; // Array of line segments for shadow casting
uniform int uNoOfLineSegments; // Number of line segments
uniform vec2 uResolution; // Screen resolution
uniform mat4 uInverseVP; // Inverse View-Projection Matrix
uniform vec2 uWorldLights[128]; // World positions of lights
uniform int uNoOfWorldLights; // Number of lights

uniform sampler2D bg_tex; // Background texture
uniform sampler2D id_tex; // Entity ID texture

// Function to check if two line segments intersect
bool doLineSegmentsIntersect(vec2 p0, vec2 p1, vec2 q0, vec2 q1) {
    vec2 r = p1 - p0;
    vec2 s = q1 - q0;
    float crossProduct = r.x * s.y - r.y * s.x;
    if (crossProduct == 0.0) {
        return false; // Parallel lines
    }
    vec2 qMinusP = q0 - p0;
    float t = (qMinusP.x * s.y - qMinusP.y * s.x) / crossProduct;
    float u = (qMinusP.x * r.y - qMinusP.y * r.x) / crossProduct;
    return (t >= 0.0) && (t <= 1.0) && (u >= 0.0) && (u <= 1.0);
}

void main() {
    vec4 texColor = texture(bg_tex, vTexCoord); // Texture color
    vec4 finalColor = texColor; // Initial color is the texture color

    // Convert pixel position to world coordinates
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0);
    vec4 worldPos = uInverseVP * ndcPos; 
    vec2 currentPixel = worldPos.xy;

    // Loop through each light and calculate its effect
    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 lightPos = uWorldLights[i];
        float distToLight = length(currentPixel - lightPos);
        float radius = uLightRadius[i];
        float attenuation = 1.0;

        // Check for shadow
        for (int j = 0; j < uNoOfLineSegments; ++j) {
            vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
            vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);
            if (doLineSegmentsIntersect(lightPos, currentPixel, p0, p1)) {
                attenuation = 0.5; // Some shadowing effect
                break;
            }
        }

        // Calculate light contribution
        float lightIntensity = max(1.0 - (distToLight / radius), 0.0);
        finalColor += vec4(uLightColors[i].rgb * lightIntensity * attenuation, uLightColors[i].a);
    }

    // Set the final color
    fFragColor = finalColor;

    // Retrieve the entity ID from the texture
    ivec2 idCoord = ivec2(gl_FragCoord.xy);
    fEntityID = texelFetch(id_tex, idCoord, 0).r;
}