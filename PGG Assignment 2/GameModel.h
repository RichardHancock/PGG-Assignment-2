#pragma once

#include <glm.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include "dependencies/glew.h"
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Vertex.h"

class ResourceManager;

/// Class to store and display a model
class GameModel
{
public:

	/// Constructor calls InitialiseVAO
	GameModel(std::string modelFilename, std::string textureFilename, ResourceManager* manager);

	GameModel(std::vector<Vertex> vertexData, std::string text, TTF_Font* font, ResourceManager* manager);

	GameModel(std::vector<Vertex> vertexData, std::string textureFilename, ResourceManager* manager);
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

	bool disableMatUniforms;

	/// Loads object model into OpenGL
	void initialiseVAO(std::string modelFilename);

	void initialiseVAO(std::vector<Vertex> vertexData);

	void initialiseVAO(std::vector<Vertex> vertexData, SDL_Surface* surface);

	/// Number of vertices in the model
	unsigned int numVertices;

};