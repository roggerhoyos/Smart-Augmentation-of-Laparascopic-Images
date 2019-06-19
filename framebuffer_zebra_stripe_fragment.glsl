#version 330 core

// Do not compare texture().rgb < vec3() - error you can only compare one element at time


// 3 different methods: Pulse, Fraction, Step 

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;
in float Depth;

uniform sampler2D vesselTexture;

uniform float multiplicationFactor = 0.5;

 float width;

 //Frustrum: 0.1 - 100 

void main() {
   
      if (Depth <=30.0 )
	            width = 0.7;
	     else if( Depth>30.0 &&  Depth<=35.0)
	            width = 0.35;
		  else if ( Depth>35.0 &&  Depth<=40.0)
		        width = 0.1;
		  else if (Depth >40)
		        width = 0.0;


		width = Depth * 1.0 / 60;
		width = min(1.0f, width);

		        
        if(fract(Pos.y * multiplicationFactor)<width && width != 0.0)
              FragColor= vec4(1.f,1.f,1.f,1.f);
 		  else if(fract(Pos.y * multiplicationFactor)>width && width != 0.0)
  		      FragColor = vec4(0.f,0.f,0.f,1.f);
			else if (width == 0.0)
			   FragColor =  vec4(0.f,0.f,0.f,0.f);		  
}	    
	     
		 
	  
  
   
//Only Uniform Stripes
//void main()
//{    
//  
//     if(fract(Pos.x * multiplicationFactor)<0.5)
//          FragColor= vec4(1.f,1.f,1.f,1.f);
//  		  else 
//  		     FragColor = vec4(0.f,0.f,0.f,1.f);      
//}




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




//vec4 vakue = texture(franebufferTexture, vec2(0, TexCoords.y))
//if(value.r > 0.5f)	{
// fancy logic
//} else {
// FragColor = 0,0,0



