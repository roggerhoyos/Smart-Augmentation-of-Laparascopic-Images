#version 330 core

//TextCoord 2 Vector


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D silhouetteTexture;
uniform sampler2D zebraTexture;




void main()
{  
  
  
  // FragColor = texture( silhouetteTexture, TexCoords);
  //  FragColor = texture(zebraTexture,TexCoords);

   FragColor = mix(texture(silhouetteTexture, TexCoords),texture(zebraTexture,TexCoords),0.5);

	if(texture(silhouetteTexture, TexCoords).r >= 0.5 && texture(zebraTexture,TexCoords).rgb == vec3(0.f))
	    FragColor = texture(zebraTexture,TexCoords);

	if(texture(silhouetteTexture, TexCoords).r >= 0.5 && texture(zebraTexture,TexCoords).rgb == vec3(1.f))
	    FragColor = texture(zebraTexture,TexCoords);
}





























//	if(texture(silhouetteTexture, TexCoords).r == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(0.f))
//	    FragColor = texture(zebraTexture,TexCoords);


//	if(texture(silhouetteTexture, TexCoords).r == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(1.f))
//	    FragColor = texture(zebraTexture,TexCoords);
//
//	
//	if(texture(silhouetteTexture, TexCoords).r == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(0.f))
//	    FragColor = texture(zebraTexture,TexCoords);
//
//	
//	if(texture(silhouetteTexture, TexCoords).g == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(1.f))
//	    FragColor = texture(zebraTexture,TexCoords);
//
//	
//	if(texture(silhouetteTexture, TexCoords).g == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(0.f))
//	    FragColor = texture(zebraTexture,TexCoords);
//
//
//   if(texture(silhouetteTexture, TexCoords).b == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(1.f))
//	    FragColor = texture(zebraTexture,TexCoords);
//
//	
//	if(texture(silhouetteTexture, TexCoords).b == 1.0 && texture(zebraTexture,TexCoords).rgb == vec3(0.f))
//	    FragColor = texture(zebraTexture,TexCoords);


//void main()
//{  
//   
//   if(texture( silhouetteTexture, TexCoords).r == 1  || texture( silhouetteTexture, TexCoords).g == 1  || texture( silhouetteTexture, TexCoords).b == 1 )
//        FragColor = vec4(1.f,1.f,1.f,1.f);
//    else 
//      FragColor = vec4(0.f,0.f,0.f,1.f);
//}