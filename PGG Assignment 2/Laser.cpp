#include "Laser.h"

Laser::Laser(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{
	//aabb = new AABB(glm::vec3(-0.2, -0.86, -0.2), glm::vec3(0.2, 0.86, 0.2));
	aabb = new AABB(glm::vec3(-0.2, -0.2, -0.86), glm::vec3(0.2, 0.2, 0.86));
	
	dead = true;
}

Laser::~Laser()
{

}

void Laser::update(float dt)
{
	pos.z -= speed * dt;

	Entity::update(dt);
}