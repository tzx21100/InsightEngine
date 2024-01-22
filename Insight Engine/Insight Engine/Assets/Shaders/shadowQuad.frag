#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;
layout(location = 5) in flat float vEntityID;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

uniform vec4 uLineSegments[128];
uniform int uNoOfLineSegments;
uniform vec2 uResolution;
uniform mat4 uInverseVP; // Inverse of View-Projection Matrix
uniform vec2 uWorldLights[128];
uniform int uNoOfWorldLights;

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
    vec2 center = vec2(0.5, 0.5);


    float dist = length(vTexCoord - center); // distance(u_light, current_pixel); // ndc

    float attenuation;

    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Convert pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // Transform to world coordinates
    vec2 current_pixel = worldPos.xy;

    bool intersectionFound = false;

    // Set the radius to be half of the smaller dimension of the quad
    float radius = min(0.5, 0.5 * length(vec2(1.0, 1.0)));

    // Use a step function to make the light only render in a circle
    float inCircle = step(dist, radius);

    for (int i = 0; i < uNoOfWorldLights; ++i) {
        vec2 worldLightPos = uWorldLights[i];
        for (int j = 0; j < uNoOfLineSegments; ++j) {
            vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
            vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);

            if (doLineSegmentsIntersect(worldLightPos, current_pixel, p0, p1)) {
                // If there's an intersection, set the flag and continue checking other line segments
                intersectionFound = true;
                break;
            }
        }
    }


    

    // Update attenuation based on whether any intersection was found
    if (intersectionFound) {
        attenuation = 0.0;
    } else {
        // Harder falloff with a squared attenuation term
        attenuation = pow(1.0 - dist / radius, 2.0);
    }

    fFragColor = vec4(vColor.rgb, vColor.a * inCircle * attenuation);

    // Apply the attenuation with the step function
    //fFragColor = vec4(0.0, 0.0, 1.0, 1.0);

    int id = int(vEntityID);
    fEntityID = id + 1;
}
