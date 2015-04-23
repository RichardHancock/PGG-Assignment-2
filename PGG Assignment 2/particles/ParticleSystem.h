#include <vector>

#include "glm.hpp"
#include "Particle.h"

#pragma once

//Heavily modified version of my PGG 1 Particle System

/**
@brief An interface to create, manage and destroy particles for one emitter
*/
class ParticleSystem
{
public:
	/**
	 @brief Creates a Particle System.
	
	 @param emitterPos				 Position of the emitter.
	 @param particlesPerCallRate	 How many particles are created each request.
	 @param [in,out] resourceManager Resource Manager.
	 */
	ParticleSystem(glm::vec3 emitterPos, int particlesPerCallRate, ResourceManager* resourceManager);
	
	
	~ParticleSystem();

	/**
	@brief Update all particles in the system
	
	Deletes expired particles, and runs update on all particles.
	@param float - Time since last frame
	*/
	void update(float dt);

	/**
	@brief Changes Emitter position
	@param  newPos New Position
	*/
	void setEmitterPos(glm::vec3 newPos);

	/**
	@brief Generate some new particles in the system

	Amount is dictated by the "particlesPerTickRate" passed in through the constructor
	*/
	void generateNewParticles();

	/**
	@brief Draws the particles.

	@param [in,out] viewMatrix The view matrix.
	@param [in,out] projMatrix The project matrix.
	@param [in,out] shader       The shader used to render the particles.
	*/
	void render(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);
private:
	/// Position of the Particle Emitter
	glm::vec3 emitterPos;
	
	/// Vector containing all the particles in the system
	std::vector<Particle*> particles;

	/// How many particles will be created every request
	int particlesPerCallRate;

	/** @brief Resource Manager. */
	ResourceManager* resourceManager;
};
