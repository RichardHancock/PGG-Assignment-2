#include "Target.h"

Target::Target(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{
	aabb = new AABB(glm::vec3(-2.2, -0.1, -2.2), glm::vec3(2.2, 6.2, 2.2));
	dead = false;
}

Target::~Target()
{

}

void Target::update(float dt)
{
	
	Entity::update(dt);
}