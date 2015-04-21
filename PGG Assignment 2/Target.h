#pragma once

#include "Entity.h"

class Target : public Entity
{
public:

	Target(glm::vec3 initPosition, std::string modelFilename, std::string textureFilename,
		ResourceManager* manager);

	~Target();

	void update(float dt);


};