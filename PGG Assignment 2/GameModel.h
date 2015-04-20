#pragma once

#include <glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"

class ResourceManager;

/// Class to store and display a model
class GameModel
{
public:

	/// Constructor calls InitialiseVAO
	GameModel(std::string modelFilename, ResourceManager* manager);
	~GameModel();

	/**
	 @brief Draws object using the given camera view and projection matrices.
	
	 @param [in,out] modelMatrix The model matrix.
	 @param [in,out] viewMatrix  The view matrix.
	 @param [in,out] projMatrix  The projection matrix.
	 @param [in,out] shader		 Shader used for rendering this model.
	 */
	void draw(glm::mat4& modelMatrix, glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);

private:
	/// Vertex Array Object for model in OpenGL
	GLuint VAO;

	GLuint textureID;

	Texture* texture;

	/// Loads object model into OpenGL
	void initialiseVAO(std::string modelFilename);

	/// Number of vertices in the model
	unsigned int numVertices;

};