#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;
layout(location = 5) in flat float vEntityID;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

void main()
{
    vec2 center = vec2(0.5, 0.5);
    float distance = length(vTexCoord - center);

    // Set the radius to be half of the smaller dimension of the quad
    float radius = min(0.5, 0.5 * length(vec2(1.0, 1.0)));

    // Use a step function to make the light only render in a circle
    float inCircle = step(distance, radius);

    // Harder falloff with a squared attenuation term
    float attenuation = pow(1.0 - distance / radius, 2.0);

    // Apply the attenuation with the step function
    fFragColor = vec4(vColor.rgb, vColor.a * inCircle * attenuation);

    int id = int(vEntityID);
    fEntityID = id + 1;
}