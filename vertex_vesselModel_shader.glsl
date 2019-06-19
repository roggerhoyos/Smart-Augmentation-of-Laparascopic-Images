#version 330 core

//Shaders are written in the C-like language GLSL
//declare all the input vertex attributes in the vertex shader with the in 
//A vector in GLSL has a maximum size of 4 and each of its values can be retrieved via vec.x, vec.y, vec.z and vec.w 
//When we're talking specifically about the vertex shader each input variable is also known as a vertex attribute. 
//vecn: the default vector of n floats.
//bvecn: a vector of n booleans.
//ivecn: a vector of n integers.
//uvecn: a vector of n unsigned integers.
//dvecn: a vector of n double components.
//Components of a vector can be accessed via vec.x where x is the first component of the vector. You can use .x, .y, .z and .w to access their first, second, third and fourth component respectively
//GLSL also allows you to use rgba for colors or stpq for texture coordinates, accessing the same components.
//The vertex shader thus requires an extra layout specification for its inputs so we can link it with the vertex data.
//Uniforms are another way to pass data from our application on the CPU to the shaders on the GPU
//but uniforms are slightly different compared to vertex attributes.
//uniforms are global. Global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage in the shader program. 
 //uniforms will keep their values until they're either reset or updated.
//f: the function expects a float as its value
//i: the function expects an int as its value
//ui: the function expects an unsigned int as its value
//3f: the function expects 3 floats as its value
//fv: the function expects a float vector/array as its value


//Attributes
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; //color of the vertex
layout (location = 2) in vec2 aTexCoords; //coordinate of the object texture of the vertex

out vec2 TexCoords; //output a texture to the fragment shader 
out vec3 Pos;

out float Depth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
  //Position = Projection Matrix x View Matrix x  Model Matrix x Local 
  //The output is finally normalized to 
    TexCoords = aTexCoords; 

    gl_Position = projection * view * model * vec4(aPos, 1.0); // set the output of the vertex shade. 4vec
	Pos = vec3(gl_Position);
	Depth = Pos.z;
}