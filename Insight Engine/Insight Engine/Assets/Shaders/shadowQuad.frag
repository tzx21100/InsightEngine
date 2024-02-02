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
uniform int type_of_light;
uniform float ilovetime;

uniform sampler2D bg_tex; // background framebuffer texture
uniform isampler2D id_tex; // ID framebuffer texture
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

vec3 blendLight(vec3 base, vec3 added) {
    // Simple additive blend mode with a safeguard against overexposure
    return min(base + added, vec3(1.0));
}

float generate_rand(vec2 uv)
{
    return fract(sin(ilovetime));
}

float real_random_thing(vec2 some_parameter)
{
    return fract(sin(dot(some_parameter, vec2(12.9898, 78.233))) * 43758.5453) * ilovetime;
}

void main() {
    vec4 final_frag_clr = texture(bg_tex, vTexCoord); // Base color from background texture
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // NDC to world coordinates
    vec2 current_pixel = worldPos.xy / worldPos.w; // Correct by perspective division
    int render = 0;
    float dist = 1.0;

    for (int i = 0; i < uNoOfWorldLights; ++i) 
    {
        vec2 lightDirection = uLightWorldPos[i] - current_pixel;
        float distance = length(lightDirection);
        float radius = uLightRadius[i];

        // change to sepia
        if (type_of_light == 1){
            radius = radius * 8.0;
        }
        //end of change to sepia 

        if (distance < radius) {
            float attenuation = pow(1.0 - (distance / radius), 2.0); // Squared falloff attenuation
            float intensity = uLightColors[i].a; // Intensity from alpha
            vec3 lightContribution = uLightColors[i].rgb * attenuation * intensity; // Apply attenuation and intensity

            bool inShadow = false;
            for (int j = 0; j < uNoOfLineSegments && !inShadow; ++j) 
            {
                vec2 p0 = vec2(uLineSegments[j].x, uLineSegments[j].y);
                vec2 p1 = vec2(uLineSegments[j].z, uLineSegments[j].w);
                if (doLineSegmentsIntersect(uLightWorldPos[i], current_pixel, p0, p1)) {

                    if (type_of_light==0){
                    lightContribution *= 0.0; // Shadow attenuation
                    }
                    else{
                        lightContribution*=0.8;
                    }
                    inShadow = true;
                }
            }

            // Blend light contribution using custom blend function
            final_frag_clr.rgb = blendLight(final_frag_clr.rgb, lightContribution);
            ++render;

            if ((distance / radius) < dist)
                dist = distance / radius;
        }
    }

    if (type_of_light == 0)
    {
         fFragColor = final_frag_clr;    

    }
    if (type_of_light == 1)
    {
        if (render == 0)
        {
            fFragColor = fFragColor;
        }
        else
        {
            fFragColor = final_frag_clr * (1.0 - dist);
        }
        
    }
    if (type_of_light==2)
    {
        // type of light is normal
        float avr = (final_frag_clr.x + final_frag_clr.y + final_frag_clr.z) / 3.0;
        avr *= 3.0;
        fFragColor = vec4(vec3(avr), final_frag_clr.a);

        if (mod(gl_FragCoord.x, real_random_thing(vTexCoord) * 10.0) < 8 || mod(gl_FragCoord.y, real_random_thing(vTexCoord) * 10.0) < 2)
            fFragColor = vec4(real_random_thing(vTexCoord));
    }
    if (type_of_light==3)
    {
        fFragColor = final_frag_clr;    
    }

    int id = texture(id_tex, vTexCoord).x;
    fEntityID = int(id); // Entity ID from ID texture
}