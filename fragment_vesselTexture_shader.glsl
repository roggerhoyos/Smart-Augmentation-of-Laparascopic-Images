#version 330 core


out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D FinalTexture;


void main()
{ 
   if(texture(FinalTexture, TexCoords).r > 0.5f && texture(FinalTexture, TexCoords).g > 0.5f && texture(FinalTexture, TexCoords).b > 0.5f)
           FragColor = vec4(1.f);
        else  if (texture(FinalTexture, TexCoords).r > 0.5)
	            FragColor = vec4(1.f);
				else 
		           FragColor = vec4(0.f,0.f,0.f,1.f);
}



