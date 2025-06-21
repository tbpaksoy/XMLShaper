#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform bool frontFaceRendering;
uniform vec3 frontFaceColor;
uniform vec3 backFaceColor;

out vec4 FragColor;

void main()
{
    if(frontFaceRendering)
    {
        if(gl_FrontFacing)
        {
            FragColor = vec4(frontFaceColor, 1.0);
        }
        else
        {
            FragColor = vec4(backFaceColor, 1.0);
        }
    }
    else
    {
        FragColor = vec4(Color, 1.0);
    }
}