#include "Particle.h"

Particle::Particle(glm::vec3 position, float scale, glm::vec3 velocity, float lifespanTime,
	std::string modelFilename, std::string textureFilename, ResourceManager* manager)
		: Entity(position, glm::vec3(0), glm::vec3(scale), modelFilename, textureFilename, manager),
		velocity(velocity)
{
	
	//Keep these open as they will be used often, and rapidly
	manager->keepResourceLoaded(modelFilename, ResourceManager::ModelFile);
	manager->keepResourceLoaded(textureFilename, ResourceManager::TextureFile);

	this->lifespan = new Utility::SimpleTimer(lifespanTime);
}

Particle::~Particle()
{
	delete lifespan;
};

void Particle::update(float dt)
{
	//Apply velocity
	pos += (velocity * dt);

	//Lower lifespan
	lifespan->update(dt);

	Entity::update(dt);
}

bool Particle::hasExpired()
{
	return (lifespan->hasTimerFinished());
}