#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;
in float Depth;

uniform sampler2D vesselTexture;


void main()
{    
       FragColor = vec4(1.f); //Full Color
}



