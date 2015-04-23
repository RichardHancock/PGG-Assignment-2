#include "Laser.h"

Laser::Laser(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{
	//These commands tell the resource manager to keep the laser model and texture in memory.
	// even after the last object using it is deleted.
	manager->keepResourceLoaded(modelFilename, ResourceManager::ModelFile);
	manager->keepResourceLoaded(textureFilename, ResourceManager::TextureFile);


	//Create AABB based on pre-determined values
	aabb = new AABB(glm::vec3(-0.2, -0.2, -0.86), glm::vec3(0.2, 0.2, 0.86));
	
	dead = false;

	//Give the laser 4 seconds of lifespan
	lifespan = new Utility::SimpleTimer(4.0f);
}

Laser::~Laser()
{
	delete lifespan;

	
}

void Laser::update(float dt)
{
	pos.z -= speed * dt;

	Entity::update(dt);

	//Update the lifespan timer
	lifespan->update(dt);

	if (lifespan->hasTimerFinished())
	{
		dead = true;
	}
}