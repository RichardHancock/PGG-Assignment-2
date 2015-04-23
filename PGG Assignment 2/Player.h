#pragma once

#include "Entity.h"

/** @brief The player ship. */
class Player : public Entity
{
public:
	/**
	 @brief Constructor.
	
	 @param initPosition	 The initial position.
	 @param rotation		 The initial rotation.
	 @param scale			 The initial scale.
	 @param movementArea	 The movement area in XY axis.
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager Resource Manager.
	 */
	Player(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, glm::vec2 movementArea,
		std::string modelFilename, std::string textureFilename, ResourceManager* manager);

	~Player();

	/**
	 @brief Updates the Player.
	
	 @param dt The deltatime.
	 */
	void update(float dt);

	/** @brief Toggle automatic forward movement. */
	void toggleForwardMovement() { allowedToMove = !allowedToMove; }

	/**
	 @brief Handles the inputs described by e.
	
	 @param e The SDL_Event to process.
	 */
	void handleInputs(SDL_Event e);

	/** @brief Reduces the player's lives */
	void hit() { lives--; }

	/**
	 @brief Gets current/remaining lives.
	
	 @return The current life.
	 */
	int getCurrentLife() { return lives; }

	/**
	 @brief Query if this Player is dead.
	
	 @return true if dead, false if not.
	 */
	bool isDead() { return (lives <= 0); }

	/**
	 @brief Gets turret position that lasers should be fired from.
	
	 @return The turret position.
	 */
	glm::vec3 getTurretPos() { return turretPos; }
protected:
	/** @brief Is the Ship currently allowed to move */
	bool allowedToMove;
	
	/** @brief The turret position. Where lasers should be fired from */
	glm::vec3 turretPos;

	/** @brief The forward speed of the ship. */
	const int forwardSpeed = 8;

	/** @brief Calculates the turret position based on the ships position. */
	void recalculateTurretPos();

	/** @brief Handles directional movement. */
	void directionMovement();

	/**
	 @brief Updates the player's xy position, while keeping them withing the movement area
	
	 @param dt The deltatime.
	 */
	void updateXYPos(float dt);

	/** @brief The speed in the XY (Directional) axis. */
	const int speed = 4;

	/** @brief The Player's lives. */
	int lives;

	/** @brief Is the ship moving in a certain direction. */
	bool movingLeft, movingRight, movingUp, movingDown;

	/** @brief The 2 Dimensional velocity. (no Z, only left, right, up, down) */
	glm::vec2 velocity;

	/** @brief The legal movement area. */
	glm::vec2 movementArea;
};