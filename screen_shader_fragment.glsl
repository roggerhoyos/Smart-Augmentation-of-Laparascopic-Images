#version 330 core

out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D zebraTexture;
uniform sampler2D silhouetteTexture;



void main()
{  
   //Combine Silhoutte and Zebra Stripe  
  FragColor = mix(texture(silhouetteTexture, TexCoords),texture(zebraTexture,TexCoords),0.5); //50% each 
}

