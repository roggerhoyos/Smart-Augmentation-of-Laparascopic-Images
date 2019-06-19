#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;
in float Depth;

uniform sampler2D depthTexture;


void main()
{    
     
    if (Depth <=30.0 ){
	        FragColor= vec4(1.f,0.f,0.f,1.f);
		}
	     else if( Depth>30.0 &&  Depth<=35.0){
	            FragColor= vec4(0.f,1.f,0.f,1.f);
			}
		    else if ( Depth>35.0 &&  Depth<=40.0){
		           FragColor= vec4(0.f,0.f,1.f,1.f);
				}
		       else if (Depth >40) {
		              FragColor= vec4(0.f,0.f,1.f,1.f);     
					}
}
