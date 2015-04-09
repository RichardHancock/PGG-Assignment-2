
#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include <glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <string>

/// Class to store and display a model
class GameModel
{
public:

	/// Constructor calls InitialiseVAO and InitialiseShaders
	GameModel();
	~GameModel();

	/// Loads object model into OpenGL
	void InitialiseVAO();

	/// Loads shaders for the object into OpenGL
	void InitialiseShaders();

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

	/// Shader program
	GLuint _program;

	/// Uniform locations
	GLint _shaderModelMatLocation, _shaderViewMatLocation, _shaderProjMatLocation;

	/// Object's model matrix
	/// This is rebuilt in the update function
	glm::mat4 _modelMatrix;

	/// Number of vertices in the model
	unsigned int _numVertices;

	///loads a shader from a file
	std::string loadShaderFromFile(char* path);
};


#endif
