#version 450 core

layout(location = 0) in vec4 vColor; 



layout(location = 1) in vec2 vTexCoord;
// layout(location = 5) in flat float vEntityID;

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
    // texelFetch current color
    // vec2 center = vec2(0.5, 0.5); // -> (camera * world_pos)  then do something to transform into texture coords ==> light pos in texture coordinates



    vec4 final_frag_clr = texture(bg_tex, vTexCoord);
    float attenuation = 1.f;
    
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Convert pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // Transform to world coordinates
    vec2 current_pixel = worldPos.xy;

    bool intersectionFound = false;

    // Use a step function to make the light only render in a circle
    

    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 worldLightPos = uLightWorldPos[i];
        float dist = length(current_pixel - worldLightPos);
        // Set the radius to be half of the smaller dimension of the quad
        float radius = uLightRadius[i] * 0.5; // which is in world pos
        float inCircle = step(dist, radius);

        for (int j = 0; j < uNoOfLineSegments; ++j) {
            vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
            vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);

            if (doLineSegmentsIntersect(worldLightPos, current_pixel, p0, p1)) {
                // If there's an intersection, set the flag and continue checking other line segments
                attenuation *= 0.5f;
            }
        }
        final_frag_clr += vec4(vColor.rgb, vColor.a * inCircle * attenuation);
    }



    // Update attenuation based on whether any intersection was found
    //if (intersectionFound) {
   //     attenuation = 0.0;
    //} else {
    //    // Harder falloff with a squared attenuation term
    //    attenuation = pow(1.0 - dist / radius, 2.0);
    //}
    fFragColor = final_frag_clr; //vec4(vColor.rgb, vColor.a * inCircle * attenuation);
    // Apply the attenuation with the step function
    //fFragColor = vec4(0.0, 0.0, 1.0, 1.0);

    // int id = int(vEntityID);
    float id = texelFetch(id_tex, ivec2(vTexCoord * textureSize(id_tex, 0)), 0).r;
    fEntityID = int(id);
}
