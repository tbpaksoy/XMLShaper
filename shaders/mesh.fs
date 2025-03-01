#version 330 core

in vec3 Color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0) * length(viewPos - FragPos);
}