#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename, 
		std::string textureFilename, ResourceManager* manager);

	~Player();

	void update(float dt);

	void toggleForwardMovement() { allowedToMove = !allowedToMove; }
protected:
	/** @brief Is the Ship currently allowed to move */
	bool allowedToMove;

	const int forwardSpeed = 4;
};