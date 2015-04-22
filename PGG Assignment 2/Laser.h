#pragma once

#include "Entity.h"
#include "Utility.h"

class Laser : public Entity
{
public:

	Laser(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
		std::string textureFilename, ResourceManager* manager);

	~Laser();

	void update(float dt);

	void hit() { dead = true; }

	bool isDead() { return dead; }
private:
	Utility::SimpleTimer* lifespan;

	bool dead;

	const int speed = 16;

};