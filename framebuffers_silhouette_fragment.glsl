#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D silhouetteTexture;

void main()
{
   
  const float offset = 1.0 / 100.0;  

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	//Edge Detector
    float kernel[9] = float[](
         1,  1,  1,
         1, -8,  1,
         1,  1,  1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(silhouetteTexture, TexCoords.st + offsets[i])); // x=s, y=t. 
    }
    vec3 col = vec3(0.0); //initialization
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i]; //Get the current value of the pixel
    
    FragColor = vec4(col, 1.0);  //Do it  for every pixel
  
} 