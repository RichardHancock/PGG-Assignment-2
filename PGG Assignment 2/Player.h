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

	void handleInputs(SDL_Event e);

	void hit() { lives--; }

	int getCurrentLife() { return lives; }

	bool isDead() { return (lives <= 0); }
protected:
	/** @brief Is the Ship currently allowed to move */
	bool allowedToMove;

	const int forwardSpeed = 2;

	void directionMovement();

	const int speed = 4;

	int lives;

	/** @brief Is the ship moving in a certain direction. */
	bool movingLeft, movingRight, movingUp, movingDown;

	/** @brief The 2 Dimensional velocity. (no Z, only left, right, up, down) */
	glm::vec2 velocity;
};