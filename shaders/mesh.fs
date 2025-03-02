#version 330 core

in vec3 Color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform float brightness;
uniform float ambientStrength;


out vec4 FragColor;

void main()
{
    FragColor =  vec4(Color, 1.0) + (brightness + length(viewPos - FragPos) * -1.0) + vec4(vec3(ambientStrength), 1.0);
}