#pragma once

#include <glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <string>
#include "Shader.h"

/// Class to store and display a model
class GameModel
{
public:

	/// Constructor calls InitialiseVAO
	GameModel(Shader* shader);
	~GameModel();

	/// Loads object model into OpenGL
	void InitialiseVAO();

	/// Currently just updates rotation to make the model rotate
	void Update( float deltaTs );

	/// Draws object using the given camera view and projection matrices
	void Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix);

	/// For setting the position of the model
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}

protected:

	/// Object position vector
	glm::vec3 _position;

	/// Euler angles for rotation
	glm::vec3 _rotation;

	/// Vertex Array Object for model in OpenGL
	GLuint _VAO;

	///Shader used for rendering this model
	Shader* shader;

	/// Object's model matrix
	/// This is rebuilt in the update function
	glm::mat4 _modelMatrix;

	/// Number of vertices in the model
	unsigned int _numVertices;

};