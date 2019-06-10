#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

//1. Load all data from our model to local Class Variable
//2. Create the buffers VAO, VBO, EBO
//3. Pass the data to the buffers
//4. Interpret Vertex Attribute

//5. Draw the object


struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};


class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	//vector<Texture> textures;
	unsigned int VAO; //Vertex Array

	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices) // pass the vectors in the mesh via reference. This seems like it could cause some unnecessary copying stuff around in memory
	{
		this->vertices = vertices;
		this->indices = indices;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	// render the mesh
	void Draw(Shader &shader) const
	{

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // render the triangles from an index buffer
		//glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices); // draw ALL vertices
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  Render data  */
	unsigned int VBO, EBO; //// vertex buffer objects, index buffer objects   ..store a large number of vertices in the GPU's 

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO); //genertate vertex array
		glBindVertexArray(VAO);
		
	    glGenBuffers(1, &VBO);      //generate buffer array
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO); //allocate buffer in a specific location of VAO
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),vertices.data(), GL_STATIC_DRAW); //copy user-defined data into the currently bound buffer

		glGenBuffers(1, &EBO);     //generate buffer array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //allocate buffer in a specific location of VAO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0); //Enable the attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //vertex attribute,size, floats, type, stride, offset
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); //layout (location = 1 ) in Shader 
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); //layout (location = 2 ) in Shader

		glBindVertexArray(0);
	}
};
#endif
