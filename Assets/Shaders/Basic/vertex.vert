#version 450 

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_VertexColour;



layout(std140, binding = 0) uniform uboMatrix
{
    mat4 view;
    mat4 proj;
    mat4 viewProj;
    vec4 pos;
};

layout(std140, binding = 1) uniform uboTest
{
    int test;
};

out vec3 vertexCol;
void main()
{
    gl_Position = proj * view * vec4(a_position, 1.0);
    vertexCol = a_VertexColour;
}