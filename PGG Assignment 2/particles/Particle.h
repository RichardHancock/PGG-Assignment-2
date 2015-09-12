#include "../Entity.h"
#include "../Utility.h"

#pragma once

//Heavily modified version of my PGG 1 Particle System

/**
@brief A particle only created by a ParticleSystem
*/
class Particle : public Entity
{
public:
	/**
	 @brief Create a Particle.
	
	 @param position		 Particles position.
	 @param scale			 The scale.
	 @param velocity		 Initial velocity.
	 @param lifespanTime	 Lifespan of the particle (Almost in seconds)
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the texture file.
	 @param [in,out] manager Resource Manager.
	 */
	Particle(glm::vec3 position, float scale, glm::vec3 velocity, float lifespanTime, 
		std::string modelFilename, std::string textureFilename, ResourceManager* manager);
	
	~Particle();

	/**
	@brief Updates a Particle
	
	Applies velocity and lowers it's lifespan
	@param dt Time since last frame
	*/
	void update(float dt);

	/**
	@brief Checks if the lifespan of the particle has run out
	
	@return bool - True if expired
	*/
	bool hasExpired();

private:
	/// How long the particle will exist (Almost in seconds)
	Utility::SimpleTimer* lifespan;

	/** @brief The velocity. */
	glm::vec3 velocity;
};