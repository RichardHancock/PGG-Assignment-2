#pragma once

#include "Entity.h"

/** @brief A target managed by the TargetManager. */
class Target : public Entity
{
public:
	/**
	 @brief Constructor.
	
	 @param initPosition	 The initial position.
	 @param rotation		 The initial rotation.
	 @param scale			 The initial scale.
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager  Resource Manager.
	 */
	Target(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
		std::string textureFilename, ResourceManager* manager);

	~Target();

	/**
	 @brief Updates this Target.
	
	 @param dt The deltatime.
	 */
	void update(float dt);

	/** @brief Destroys this Target. */
	void hit() { dead = true; }

	/**
	 @brief Query if this Target is dead.
	
	 @return true if dead, false if not.
	 */
	bool isDead() { return dead; }

protected:
	/** @brief Is the target dead */
	bool dead;
};