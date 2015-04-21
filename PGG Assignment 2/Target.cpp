#include "Target.h"
#include "gtc/matrix_transform.hpp"

Target::Target(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{

}

Target::~Target()
{

}

void Target::update(float dt)
{
	
	Entity::update(dt);
}