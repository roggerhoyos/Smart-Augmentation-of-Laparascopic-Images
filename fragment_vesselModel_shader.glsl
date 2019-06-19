#version 330 core
//The fragment shader is all about calculating the color output of your pixels. 
//The fragment shader only requires one output variable and that is a vector of size 4 that defines the final color output that we should calculate ourselves
// fragment shader gets an interpolated colour based on its position on the surface.
//Keep in mind that this will happen with any other vertex buffer attributes that you send to the fragment shader; normals will be interpolated, and texture coordinates will be interpolated to each fragment.
//vertex shader outputs are not sending a constant variable from a vertex shader to all fragment shaders. We use uniform variables for that.
// the fragment shader requires a vec4 color output variable
// If you'd fail to specify an output color in your fragment shader OpenGL will render your object black (or white).
//unsigned color: 0 to 1
//singned color : -1 to 1 
//fragment interpolation
// When rendering a triangle the rasterization stage usually results in a lot more fragments than vertices originally specified.
//The rasterizer then determines the positions of each of those fragments based on where they reside on the triangle shape.
//depth values in screen space are non-linear. high precision for small z-values and a low precision for large z-values



out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;
in float Depth;

uniform sampler2D vesselTexture;


void main()
{    
 
       FragColor = vec4(1.f);
      
     //FragColor = texture(vesselTexture, TexCoords)
	 //FragColor = vec4(vec3(gl_FragCoord.z),1.0); //depth value of the fragment as a color    
}



