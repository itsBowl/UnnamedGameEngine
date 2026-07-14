#version 450 

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_VertexColour;

uniform int test;
out vec3 vertexCol;
void main()
{
    gl_Position = vec4(a_position, 1.0);
    vertexCol = a_VertexColour;
}