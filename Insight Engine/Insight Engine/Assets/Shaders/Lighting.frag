#version 450 core
layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec4 fFragColor;

void main()
{
    vec2 center = vec2(0.5, 0.5);
    float distance = length(vTexCoord - center);

    // Set the radius to be half of the smaller dimension of the quad
    float radius = min(0.5, 0.5 * length(vec2(1.0, 1.0)));

    // Use a step function to make the light only render in a circle
    float inCircle = step(distance, radius);

    // Apply the original attenuation formula with the step function
    fFragColor = vec4(vColor.rgb, vColor.a * inCircle * (pow(0.01, distance) - 0.01));
}