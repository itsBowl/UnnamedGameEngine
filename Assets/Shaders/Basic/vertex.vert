#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_VertexColour;

//notes:
//GLSL uniform = HLSL cbuffer
layout(std140, binding = 0) uniform CameraData
{
    mat4 view;
    mat4 proj;
    mat4 viewProj;
    vec4 pos;
};

layout(std430, binding = 1) readonly buffer modelMatrix
{
    mat4 matrix[];
};

layout(std140, binding = 2) uniform modelIndex
{
    uint index;
    uint hash;
};

out vec3 vertexCol;
void main()
{
    mat4 model = matrix[index + uint(gl_InstanceID)];
    gl_Position = proj * view * model * vec4(a_position, 1.0);
    vertexCol = a_VertexColour;
}