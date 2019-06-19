#version 330 core

// Do not compare texture().rgb < vec3() - error you can only compare one element at time


// 3 different methods: Pulse, Fraction, Step 

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D changeColorTexture;


 //Frustrum: 0.1 - 100 

void main() {
   
	if(texture(changeColorTexture,TexCoords).r >= 0.5 && texture(changeColorTexture,TexCoords).g >= 0.5 && texture(changeColorTexture,TexCoords).b >= 0.5)  
	     FragColor = vec4(1.0,0.f,0.f,1.f);
	  else 
	      FragColor = vec4(0.f,0.f,0.f,1.f);
}	    
	     