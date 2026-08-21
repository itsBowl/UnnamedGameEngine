#version 450
in vec3 vertexCol;
layout(location = 0) out vec4 fragColour;

uniform int test;
uniform float time;

vec3 col1 = vec3(1.f, 0.f, 0.f);
vec3 col2 = vec3(0.f, 1.f, 0.f);
vec3 col3 = vec3(0.f, 0.f, 1.0);
vec3 col4 = vec3(1.f, 1.f, 1.f);


void main()
{
    vec3 vcol = vertexCol;
    switch (test)
    {
        case 1:
            vcol *= col1;
            break;
        case 2:
            vcol *= col2;
            break;
        case 3:
            vcol *= col3;
            break;
        case 4:
            vcol *= col4;
            break;
    }
    fragColour = vec4(vcol, 1.0);
}

