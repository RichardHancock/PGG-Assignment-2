#pragma once

#include "Entity.h"

class Target : public Entity
{
public:

	Target(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
		std::string textureFilename, ResourceManager* manager);

	~Target();

	void update(float dt);


};