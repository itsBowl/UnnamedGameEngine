#version 450
in vec3 vertexCol;
layout(location = 0) out vec4 fragColour;

layout(std140, binding = 2) uniform modelIndex
{
    uint index;
    uint hash;
};

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 debugColor(uint id)
{
    //magic is 1/phi 
    float hue = fract(float(id) * 0.61803398875);
    return hsv2rgb(vec3(hue, 0.8, 1.0));
}


void main()
{
    float hue = fract(float(hash) * 0.61803398875);
    fragColour = vec4(debugColor(hash), 1.0);
}

