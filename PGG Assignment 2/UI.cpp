#include "UI.h"
#include <vector>

UI::UI(glm::vec2 pos, glm::vec2 dimensions, std::string textureFilename, ResourceManager* manager)
{
	std::vector<Vertex> vertexData = createVertices(pos, dimensions);

	uiElement = new GameModel(vertexData, textureFilename, manager);
}

UI::UI(glm::vec2 pos, glm::vec2 dimensions, std::string text, TTF_Font* font, ResourceManager* manager)
{
	std::vector<Vertex> vertexData = createVertices(pos, dimensions);

	uiElement = new GameModel(vertexData, text, font, manager);
}

std::vector<Vertex> UI::createVertices(glm::vec2 pos, glm::vec2 dimensions)
{
	std::vector<Vertex> vertexData;
	glm::vec2 max(pos.x + dimensions.x, pos.y + dimensions.y);

	//Create two triangles to make up the UI element
	float vertices[] =
	{
		pos.x, pos.y,
		pos.x, max.y,
		max.x, max.y,
		pos.x, pos.y,
		max.x, pos.y,
		max.x, max.y
	};

	float vt[] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	for (unsigned int i = 0; i < 6; i++)
	{
		//pos in float arrays 
		unsigned int curPos = i * 2;
		Vertex vert;
		vert.v.x = vertices[curPos];
		vert.v.y = vertices[curPos + 1];
		vert.v.z = 0;

		vert.vn = Vec3(0, 0, 0);

		vert.vt.x = vt[curPos];
		vert.vt.y = vt[curPos + 1];
		
		vertexData.push_back(vert);
	}

	return vertexData;
}

void UI::draw(Shader* shader)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	uiElement->draw(glm::mat4(1), glm::mat4(1), glm::mat4(1), shader);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
}