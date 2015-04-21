#include "Target.h"
#include "gtc/matrix_transform.hpp"

Target::Target(glm::vec3 initPosition, std::string modelFilename, std::string textureFilename,
	ResourceManager* manager) 
	: Entity(initPosition, modelFilename, textureFilename, manager)
{

}

Target::~Target()
{

}

void Target::update(float dt)
{
	modelMatrix = glm::translate(glm::mat4(1.0f), pos);
	modelMatrix = glm::scale(modelMatrix , glm::vec3(0.2, 0.2, 0.2));
	modelMatrix = glm::rotate(modelMatrix, Utility::HALF_PI, glm::vec3(0, 1, 0));
}