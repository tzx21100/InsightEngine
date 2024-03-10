#version 450 core

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

uniform vec4 uLightColors[128]; // light color + intensity (as alpha value)
uniform float uLightRadius[128]; // light radiuses
uniform vec2 uLightWorldPos[128]; // light world positions
uniform int uNoOfWorldLights; // light count
uniform vec4 uLineSegments[512]; // line segment world positions (x,y) to (z,w)
uniform int uNoOfLineSegments; // line segment count
uniform vec2 uResolution; // resolution of game
uniform mat4 uInverseVP; // Inverse of View-Projection Matrix
uniform int uShaderEffect;
uniform float uTimeElapsed;
uniform float uShaderEffectTimer;
uniform float uShaderIntensity;

uniform sampler2D bg_tex; // background framebuffer texture
uniform isampler2D id_tex; // ID framebuffer texture

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
    return fract(sin(uTimeElapsed));
}

float random_generator(vec2 param)
{
    return fract(sin(dot(param, vec2(12.9898, 78.233))) * 43758.5453) * uTimeElapsed;
}

vec2 applyLensDistortion(vec2 uv, float distortionAmount) {
    vec2 centeredUV = uv - 0.5;
    float dist = dot(centeredUV, centeredUV);
    vec2 distortedUV = uv + centeredUV * dist * distortionAmount;
    return distortedUV;
}

float filmReelNoise(vec2 uv, float time) {
    float noise = sin(dot(uv + time, vec2(12.9898, 78.233))) * 43758.5453;
    return fract(noise);
}

void main() {

    vec4 final_frag_clr = texture(bg_tex, vTexCoord); // Base color from background texture
    vec4 ndcPos = vec4(2.0 * (gl_FragCoord.xy / uResolution) - 1.0, 0.0, 1.0); // Pixel position to NDC
    vec4 worldPos = uInverseVP * ndcPos; // NDC to world coordinates
    vec2 current_pixel = worldPos.xy / worldPos.w; // Correct by perspective division
    float dist = 1.0;

    for (int i = 0; i < uNoOfWorldLights; ++i) 
    {
        vec2 lightDirection = uLightWorldPos[i] - current_pixel;
        float distance = length(lightDirection);
        float radius = uLightRadius[i];

        // tutorial shader effect
        if (uShaderEffect == 1){
            radius = radius * 1;
        }

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

                    if (uShaderEffect==0) {
                        lightContribution *= 0.0; // Shadow attenuation
                    }
                    else {
                        lightContribution *= 0.0;
                    }
                    inShadow = true;
                }
            }

            // Blend light contribution using custom blend function
            final_frag_clr.rgb = blendLight(final_frag_clr.rgb, lightContribution);
            if ((distance / radius) < dist) dist = distance / radius;
        }
    }
    
    if (uShaderEffect == 0)
    {
        fFragColor = final_frag_clr;
    }
    else if (uShaderEffect == 1)
    {
        fFragColor = final_frag_clr * (1.0 - dist / 2.5); // light circle around player
    }
    else if (uShaderEffect==2) // black and white chase level
    {
        // type of light is normal
        // float avr = (final_frag_clr.x + final_frag_clr.y + final_frag_clr.z) / 3.0;
        // avr *= 1.0;
        // fFragColor = vec4(vec3(avr), final_frag_clr.a);

        // if (mod(gl_FragCoord.x, random_generator(vTexCoord) * 10.0) < 0.2 || mod(gl_FragCoord.y, random_generator(vTexCoord) * 10.0) < 0.1)
        //     fFragColor = vec4(random_generator(vTexCoord));
   
        
        // Calculate screen coordinates and normalized distance from the center
        vec2 screenCoord = gl_FragCoord.xy / uResolution.xy;
        vec2 centerCoord = screenCoord - vec2(0.5, 0.5); // Center is (0.5, 0.5) in normalized coords
        float distFromCenter = length(centerCoord); // Distance from the center
        float edgeDist = min(min(screenCoord.x, 1.0 - screenCoord.x), min(screenCoord.y, 1.0 - screenCoord.y));

        // Increase distortion towards the edges
        float intensity = smoothstep(0.0, 0.1, edgeDist); // Reversed the smoothstep parameters to correctly increase intensity towards edges

        // Apply more noticeable warping and displacement effects
        float warpEffect = sin(uTimeElapsed * 5.0 + screenCoord.y * 20.0) * 0.02; // Increased effect
        float displaceEffect = cos(uTimeElapsed * 5.0 + screenCoord.x * 20.0) * 0.02; // Using cos for a varied effect and increased displacement
        
        // Distorted coordinates
        vec2 distortedCoord = screenCoord + vec2(warpEffect, displaceEffect) * (1.0 - intensity);

        // Sample the background texture with distorted coordinates
        vec4 bgTexColor = texture(bg_tex, distortedCoord);

        // Convert to grayscale
        float avr = (bgTexColor.r + bgTexColor.g + bgTexColor.b) / 3.0;
        vec3 grayscale = vec3(avr);

        // Interpolate between grayscale and original color based on the distance and uShaderIntensity
        float radiusOfMonochrome = 1 - uShaderIntensity; // 1 = full screen, 0.5 = half
        float colorRestoreFactor = smoothstep(0.0, radiusOfMonochrome, distFromCenter);
        vec3 finalColor = mix(final_frag_clr.rgb, grayscale, colorRestoreFactor);

        // Flashing side borders
        float borderWidth = 0.05;
        float flashIntensity = abs(sin(uTimeElapsed * 5.0));
        float borderIntensity = (1.0 - smoothstep(0.0, borderWidth, edgeDist)) * flashIntensity;
        vec3 borderColor = mix(finalColor, vec3(0.0, 0.0, 0.0), borderIntensity);

        // Output the final color with applied effects
        fFragColor = vec4(borderColor, final_frag_clr.a);
    }
    else if (uShaderEffect == 3) { // static glitch
        // Determine glitch activation based on uTimeElapsed
        float glitchCycle = 3.0; // Glitch every 5 seconds
        float glitchDuration = 0.2; // Glitch is active for 0.2 seconds
        float currentTime = mod(uTimeElapsed, glitchCycle);
        bool isGlitchActive = currentTime < glitchDuration;

        // Apply lens distortion and chromatic aberration
        vec2 uv = applyLensDistortion(vTexCoord, 0.02);
        float aberrationOffset = isGlitchActive ? 0.02 : 0.005; // Increase offset if glitch is active
        vec3 colorR = texture(bg_tex, uv + vec2(aberrationOffset, 0.0)).rgb;
        vec3 colorG = texture(bg_tex, uv).rgb;
        vec3 colorB = texture(bg_tex, uv - vec2(aberrationOffset, 0.0)).rgb;
        vec3 chromaticAberrationColor = vec3(colorR.r, colorG.g, colorB.b);

        // Optionally, apply film reel noise based on glitch activation
        float noiseIntensity = isGlitchActive ? 0.2 : 0.05; // Increase noise if glitch is active
        float noise = filmReelNoise(uv, uTimeElapsed) * noiseIntensity;
        vec3 noiseColor = vec3(noise);

        // Combine chromatic aberration, noise, and final color from lighting calculations
        vec3 finalColorWithEffects = final_frag_clr.rgb + chromaticAberrationColor + noiseColor;

        // Output the final color, applying the glitch effect conditionally
        fFragColor = vec4(finalColorWithEffects, final_frag_clr.a);
    }
    else if (uShaderEffect == 4 && uShaderEffectTimer >= 0.0) {
        vec3 colorEffect = vec3(0.0);
        vec2 uv = vTexCoord;
        vec2 position = (gl_FragCoord.xy / uResolution.xy) * 2.0 - 1.0;
        position.x *= uResolution.x / uResolution.y; // Aspect correction

        float cycleLength = 6.0; // Total effect duration
        float convergeDuration = 4.0 * 0.75; // Duration of the convergence phase
        float fadeOutDuration = 2.0; // Not directly used since fading is tied to the timer

        // Directly use uShaderEffectTimer to determine the phase
        bool isConverging = uShaderEffectTimer > cycleLength - convergeDuration;
        
        float intensity;
        if (isConverging) {
            // During convergence, calculate intensity based on the timer, inversely proportional
            intensity = uShaderEffectTimer / convergeDuration;
        } else {
            // After convergence, start fading out based on the remaining timer
            intensity = uShaderEffectTimer / fadeOutDuration;
        }

        // Ensure intensity is clamped to [0, 1] to prevent under/overflow
        intensity = clamp(intensity, 0.0, 1.0);

        for (int i = 0; i < 3; i++) {
            float z = uShaderEffectTimer * 0.1 + float(i) * 0.07;
            float l = length(position);
            vec2 p = position;

            float directionMultiplier = isConverging ? -1.0 : 1.0;
            float effectStrength = sin(z) + 1.0;

            p += (position / max(l, 0.001)) * directionMultiplier * effectStrength * intensity * abs(sin(l * 9.0 - z * 2.0));
            colorEffect[i] = 0.01 / length(mod(p, 1.0) - 0.5);
        }

        // Retrieve the base color from the texture
        vec4 baseColor = texture(bg_tex, uv);

        // Combine the colorEffect with the base color, modulated by intensity
        vec4 finalColor = vec4(colorEffect, 1.0) * intensity + baseColor;
        fFragColor = finalColor;
    } 
    

    // Entity ID output remains unchanged
    int id = texture(id_tex, vTexCoord).x;
    fEntityID = int(id);
}