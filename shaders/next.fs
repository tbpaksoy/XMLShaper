#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform float time;
uniform float drawDistance;

bool isInRange()
{
    return length(FragPos) < drawDistance;
}

void main()
{
    if(isInRange()) 
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}