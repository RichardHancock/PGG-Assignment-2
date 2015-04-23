#pragma once

#include "Entity.h"
#include "Utility.h"

/** @brief A laser fired by the Player class. */
class Laser : public Entity
{
public:

	/**
	 @brief Constructor.
	
	 @param initPosition	 The initial position.
	 @param rotation		 The initial rotation.
	 @param scale			 The initial scale.
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager Resource Manager.
	 */
	Laser(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
		std::string textureFilename, ResourceManager* manager);

	~Laser();

	/**
	 @brief Updates this laser.
	
	 @param dt The deltatime.
	 */
	void update(float dt);

	/** @brief Destroy the laser */
	void hit() { dead = true; }

	/**
	 @brief Query if this Laser is dead.
	
	 @return true if dead, false if not.
	 */
	bool isDead() { return dead; }
private:
	/** @brief Timer counting down the lasers lifespan. */
	Utility::SimpleTimer* lifespan;
	
	/** @brief Is the laser dead */
	bool dead;

	/** @brief The laser's speed. */
	const int speed = 16;

};