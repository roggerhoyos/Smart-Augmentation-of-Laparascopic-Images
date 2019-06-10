#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;
in float Depth;

uniform sampler2D vesselTexture;

uniform int multiplicationFactor = 1;


void main()
{      
     if(fract(Pos.x * multiplicationFactor)<0.5)
          FragColor= vec4(1.f,1.f,1.f,1.f); //White Stripe 
  		  else 
  		     FragColor = vec4(0.f,0.f,0.f,1.f);  //Black Stripe
}





//Method Black
//uniform int multiplicationFactor = 8;
// void main ()
// {  
   //   FragColor =texture(screenTexture,TexCoords) 
//   // Method White 
//   if(texture(zebraTexture,TexCoords).rgb == vec3(1.f)){
//       if(fract(TexCoords.t * multiplicationFactor)<0.5)
//           FragColor= texture(zebraTexture,TexCoords);
//		  else 
//		     FragColor = vec4(0.f,0.f,0.f,1.f);
//    }
//     else 
//      FragColor = vec4(0.f,0.f,0.f,1.f); 
// }





//Method Pink 
//   float pulse(float val, float dst) {
//    return floor(mod(val*dst,1.0)+.5);
//  }
// 
//    void main()
//     {  
//    
//      vec2 cpos = TexCoords;
//	  vec4 color = texture(screenTexture,TexCoords);
//
//	  if(texture(screenTexture,TexCoords).rgb == vec3(1.f))
//	  {
//	       color.g = pulse(cpos.y,8); 
//           FragColor = color;
//        }
//	   else
//	       FragColor = vec4(0.f,0.f,0.f,1.f);
//       
//    }
 


//Method Red
//  void main()
//  {
//    float scale = 8.0;
//    float fr = fract(TexCoords.y * scale);
//    vec4 color = (texture(screenTexture,TexCoords));
//	
//	if(texture(screenTexture,TexCoords).rgb == vec3(1.f)){
//	     color.r = step(0.5, fr);
//		 FragColor = color;
//	   }
//		 else
//		   FragColor = vec4(0.f,0.f,0.f,1.f);
//  }



