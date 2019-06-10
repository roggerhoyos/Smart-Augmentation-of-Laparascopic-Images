#version 330 core

//Attributes
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; //color of the vertex
layout (location = 2) in vec2 aTexCoords; //coordinate of the object texture of the vertex

out vec2 TexCoords; //output a texture to the fragment shader 
out vec3 Pos; //Position 

out float Depth; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
  //Position = Projection Matrix x View Matrix x  Model Matrix x Local 
    TexCoords = aTexCoords; 

    gl_Position = projection * view * model * vec4(aPos, 1.0); // set the output of the vertex shade. 4vec
	Pos = vec3(gl_Position);
	Depth = Pos.z;
}