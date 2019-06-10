#version 330 core

//TextCoord 2 Vector


out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D zebraTexture;


void main()
{  
  FragColor = texture(zebraTexture, TexCoords); //Render Zebra Texture
}




