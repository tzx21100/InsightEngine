#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;
layout(location = 5) in flat float vEntityID;

layout(location = 6) in flat float lightWidth;
layout(location = 7) in flat vec2 worldLightPos;

layout(location = 0) out vec4 fFragColor;
layout(location = 1) out int fEntityID;

uniform vec4 uLineSegments[128];
//uniform int uNoOfLineSegments;

// Function to check if two line segments intersect
bool doLineSegmentsIntersect(vec2 p0, vec2 p1, vec2 q0, vec2 q1) {
    vec2 r = p1 - p0;
    vec2 s = q1 - q0;

    float rxs = r.x * s.y - r.y * s.x;
    vec2 qp0 = q0 - p0;
    float qpxr = qp0.x * r.y - qp0.y * r.x;

    // If rxs and qpxr are both 0, the lines are collinear
    if (rxs == 0.0 && qpxr == 0.0) {
        // Check if the lines overlap
        float t0 = dot(qp0, r) / dot(r, r);
        float t1 = t0 + dot(s, r) / dot(r, r);
        return (max(min(t0, t1), 0.0) <= 1.0);
    }

    // If rxs is 0 but qpxr is not 0, the lines are parallel and non-intersecting
    if (rxs == 0.0 && qpxr != 0.0) {
        return false;
    }

    // Calculate the intersection parameters
    float t = (qp0.x * s.y - qp0.y * s.x) / rxs;
    float u = qpxr / rxs;

    // Check if the intersection point is within the parameter ranges of both line segments
    return (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0);
}

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

    //for (int i = 0; i < uNoOfLineSegments; ++i) {
        vec2 p0 = vec2(uLineSegments[0].x, uLineSegments[0].y);
        vec2 p1 = vec2(uLineSegments[0].z, uLineSegments[0].w);

        p0 = (p0 - worldLightPos) / lightWidth + 0.5; // convert points from world pos to UV pos (to calculate against texCoords in UV)
        p1 = (p1 - worldLightPos) / lightWidth + 0.5;

        // Check if the current pixel's line segment intersects with the light line segment
        if (doLineSegmentsIntersect(vTexCoord, center, p0, p1)) {
            // If there's an intersection, cast a shadow (adjust this part based on your requirements)
            attenuation = 0.0;

            //break;  // Exit the loop if there's an intersection
        }
        
        //fFragColor = vec4(vColor.rgb, vColor.a * inCircle * attenuation);
    //}

    // Apply the attenuation with the step function
    fFragColor = vec4(vColor.rgb, vColor.a * inCircle * attenuation);

    int id = int(vEntityID);
    fEntityID = id + 1;
}