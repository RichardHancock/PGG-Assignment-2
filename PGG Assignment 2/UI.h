#pragma once

#include "dependencies/glew.h"
#include "glm.hpp"
#include <string>

#include "SDL_ttf.h"
#include "Shader.h"
#include "Vertex.h"
#include "GameModel.h"

class UI
{
public:
	UI(glm::vec2 pos, glm::vec2 dimensions, std::string textureFilename, ResourceManager* manager);

	UI(glm::vec2 pos, glm::vec2 dimensions, std::string text, TTF_Font* font, ResourceManager* manager);

	std::vector<Vertex> createVertices(glm::vec2 pos, glm::vec2 dimensions);

	void draw(Shader* shader);

private:

	GameModel* uiElement;
};