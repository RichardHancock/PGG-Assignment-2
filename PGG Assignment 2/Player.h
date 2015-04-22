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

	glm::vec3 getTurretPos() { return turretPos; }
protected:
	/** @brief Is the Ship currently allowed to move */
	bool allowedToMove;

	glm::vec3 turretPos;

	const int forwardSpeed = 2;

	void recalculateTurretPos();

	void directionMovement();

	const int speed = 4;

	int lives;

	/** @brief Is the ship moving in a certain direction. */
	bool movingLeft, movingRight, movingUp, movingDown;

	/** @brief The 2 Dimensional velocity. (no Z, only left, right, up, down) */
	glm::vec2 velocity;
};