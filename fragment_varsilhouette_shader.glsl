#version 330 core

//TextCoord 2 Vector
//

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D silhouetteTexture; //Silhouette


vec2 result;




 void main()
{ 
   
    float dx = 1.0f/1200;  //step on x
	float dy = 1.0f/800;   //step on y
	vec3 sum = vec3(0,0,0);


	for(int i = -18; i< 18; i++) 
	   for(int j = -18;j < 18; j++) 
		  sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).r)/180;  //Frustrum: 0 to 35
   
   for(int i = -7; i< 7; i++) 
	   for(int j = -7;j < 7; j++) 
		  sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).g)/70;	//Frustrum: 35 to 40

	for(int i = -1; i< 1; i++) 
	   for(int j = -1;j < 1; j++) 
		  sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).b)/10;	//Frustrum: 40 to 100 


		FragColor = vec4(sum,1.0);
 } 









 //void main()
//{ 
//  
////  if (texture(silhouetteTexture,TexCoords).r == 1.f ) {     
////	   FragColor = vec4(1.f, 1.f,1.f,1.f); 
////   }
////	 else 
////	   FragColor = vec4(0.f, 0.f,0.f,1.f);
//
//// FragColor = texture(silhouetteTexture,TexCoords);
//	   
//    float dx = 1.0f/1200;  //step on x
//	float dy = 1.0f/800;   //step on y
//	vec3 sum = vec3(0,0,0);
//
//  if (texture(silhouetteTexture,TexCoords).b >= 0.5f ) { 	  
//	for(int i = -3; i< 3; i++) 
//	   for(int j = -3;j < 3; j++) 
//		  sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).rgb)/30;		
//     }
//    else if (texture(silhouetteTexture,TexCoords).g >= 0.5f) {
//	  for(int i = -50; i< 50; i++) 
//	   for(int j = -50;j < 50; j++) 
//		   sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).rgb)/60;		
//	 }
//	  else if (texture(silhouetteTexture,TexCoords).r >= 0.5f) {
//	   for(int i = -90; i< 90; i++) 
//	     for(int j = -90;j < 90; j++) 
//		  sum += (texture(silhouetteTexture, TexCoords + vec2(i * dx, j * dy)).rgb)/90;
//	   }
//	    
//		FragColor = vec4(sum,1.0);
// } 
//