
#include "GameModel.h"

#include <iostream>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include "OBJLoader.h"
#include "Vertex.h"

GameModel::GameModel(Shader* shader) 
	: shader(shader)
{
	// Initialise variables
	_VAO = 0;
	_numVertices = 0;

	// Create the model
	InitialiseVAO();
}

GameModel::~GameModel()
{
	// TODO: destroy VAO, shaders etc
}

void GameModel::InitialiseVAO()
{
		// Creates one VAO
	glGenVertexArrays( 1, &_VAO );
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray( _VAO );

	std::vector<Vertex> tempVertexData;

	OBJLoader::load(tempVertexData,"models/testTri.obj");

	const int vertexCount = tempVertexData.size() * 8;
	_numVertices = vertexCount / 8;
	GLfloat* vertexData = new GLfloat[vertexCount];

	for (unsigned int i = 0; i < tempVertexData.size(); i++)
	{
		unsigned int aPos = i * 8; // Position in the GLfloat array

		vertexData[aPos] = tempVertexData[i].v.x;
		vertexData[aPos + 1] = tempVertexData[i].v.y;
		vertexData[aPos + 2] = tempVertexData[i].v.z;

		vertexData[aPos + 3] = tempVertexData[i].vn.x;
		vertexData[aPos + 4] = tempVertexData[i].vn.y;
		vertexData[aPos + 5] = tempVertexData[i].vn.z;

		vertexData[aPos + 6] = tempVertexData[i].vt.x;
		vertexData[aPos + 7] = tempVertexData[i].vt.y;
		
	}
	//&vector[0]
	//const int verticeCount = tempVertexData.size() * 3;
	//const int normalCount = verticeCount;

	// Variable for storing a VBO
	GLuint vertexBuffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &vertexBuffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertexData, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	// (We will look at this properly in the lectures)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, v)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vn)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vt)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind for neatness, it just makes life easier
	// As a general tip, especially as you're still learning, for each function that needs to do something specific try to return OpenGL in the state you found it in
	// This means you will need to set the states at the beginning of your function and set them back at the end
	// If you don't do this, your function could rely on states being set elsewhere and it's easy to lose track of this as your project grows
	// If you then change the code from elsewhere, your current code could mysteriously stop working properly!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
	
	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	//glDisableVertexAttribArray(0);

}

void GameModel::Update( float deltaTs )
{
	// update the rotation angle of our cube
	_rotation.y += deltaTs * 0.5f;
	while( _rotation.y > (3.14159265358979323846 * 2.0) )
	{
		_rotation.y -= (float)(3.14159265358979323846 * 2.0);
	}

	// Build the model matrix!
	// First we start with an identity matrix
	// This is created with the command: glm::mat4(1.0f)
	// Next, we translate this matrix according to the object's _position vector:
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position );
	_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
	// Next, we rotate this matrix in the y-axis by the object's y-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0,1,0) );
	// And there we go, model matrix is ready!

}

void GameModel::Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix)
{
	// Ok, here I like to indent drawing calls - it's just a personal style, you may not like it and that's fine ;)
	// Generally you will need to be activating and deactivating OpenGL states
	// I just find it visually easier if the activations / deactivations happen at different tab depths
	// This can help when things get more complex

	// Activate the shader program
	glUseProgram(shader->getProgram());

		// Activate the VAO
		glBindVertexArray( _VAO );

			// Send matrices to the shader as uniforms like this:
			glUniformMatrix4fv(shader->getModelMatLoc(), 1, GL_FALSE, glm::value_ptr(_modelMatrix) );
			glUniformMatrix4fv(shader->getViewMatLoc(), 1, GL_FALSE, glm::value_ptr(viewMatrix) );
			glUniformMatrix4fv(shader->getProjMatLoc(), 1, GL_FALSE, glm::value_ptr(projMatrix) );


			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, _numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
	
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}