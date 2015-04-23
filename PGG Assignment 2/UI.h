#pragma once

#include "dependencies/glew.h"
#include "glm.hpp"
#include <string>

#include "SDL_ttf.h"
#include "Shader.h"
#include "Vertex.h"
#include "GameModel.h"

/** @brief An OpenGL user interface element. */
class UI
{
public:
	/**
	 @brief Constructor.
	
	 @param pos				 The position, between (-1.0 - 1.0, -1.0 - 1.0).
	 @param dimensions		 The dimensions.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager Resource Manager.
	 */
	UI(glm::vec2 pos, glm::vec2 dimensions, std::string textureFilename, ResourceManager* manager);

	/**
	 @brief Constructor.
	
	 @param pos				 The position, between (-1.0 - 1.0, -1.0 - 1.0).
	 @param dimensions		 The dimensions.
	 @param text			 The text string to render.
	 @param [in,out] font    The font to use.
	 @param [in,out] manager Resource Manager.
	 */
	UI(glm::vec2 pos, glm::vec2 dimensions, std::string text, TTF_Font* font, ResourceManager* manager);

	~UI();

	/**
	 @brief Creates the vertices for a 2D panel.
	
	 @param pos		   The position, between (-1.0 - 1.0, -1.0 - 1.0).
	 @param dimensions The dimensions.
	
	 @return The new vertices.
	 */
	std::vector<Vertex> createVertices(glm::vec2 pos, glm::vec2 dimensions);

	/**
	 @brief Draws the UI element using the given shader.
	
	 @param [in,out] shader A 2D shader.
	 */
	void draw(Shader* shader);

private:
	/** @brief The model data for the UI. */
	GameModel* uiElement;
};