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

//Forward Declaration
class ResourceManager;

/// Class to store and display a model
class GameModel
{
public:

	/**
	 @brief Constructor.
	
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager Resource Manager.
	 */
	GameModel(std::string modelFilename, std::string textureFilename, ResourceManager* manager);

	/**
	 @brief Constructor.
	
	 @param vertexData		Array of vertices.
	 @param text			 The text string to render.
	 @param [in,out] font    The font to use.
	 @param [in,out] manager  Resource Manager.
	 */
	GameModel(std::vector<Vertex> vertexData, std::string text, TTF_Font* font, ResourceManager* manager);

	/**
	 @brief Constructor.
	
	 @param vertexData		 Array of vertices.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager  Resource Manager..
	 */
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

	/** @brief Buffer for vertex data. */
	GLuint vertexBuffer;

	/** @brief Identifier for the texture. */
	GLuint textureID;

	/** @brief The texture data. */
	Texture* texture;

	/** @brief Filename of the texture file. */
	std::string textureFilename;

	/** @brief true to disable, false to enable the matrix uniforms. (Is used to avoid passing matrices to 2D UI objects)*/
	bool disableMatUniforms;

	/**
	 @brief Initialises the VAO and VBOs from the passed in OBJ filename. Also loads in the texture.
	
	 @param modelFilename Filename of the model file.
	 */
	void initialiseVAO(std::string modelFilename);

	/**
	 @brief Initialises the VAO and VBOs from the passed in Vertices. Also loads in the texture.
	
	 @param vertexData Array of vertices.
	 */
	void initialiseVAO(std::vector<Vertex> vertexData);

	/**
	 @brief Initialises the VAO and VBOs from the passed in Vertices. Also loads in the texture from the passed in surface.
	
	 @param vertexData		 Array of vertices.
	 @param [in,out] surface Surface to use as the texture.
	 */
	void initialiseVAO(std::vector<Vertex> vertexData, SDL_Surface* surface);

	/// Number of vertices in the model
	unsigned int numVertices;

	/** @brief Resource Manager. */
	ResourceManager* resManager;

};