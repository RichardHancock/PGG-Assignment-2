#pragma once

#include "Entity.h"

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

	bool dead;

	const int speed = 8;

};