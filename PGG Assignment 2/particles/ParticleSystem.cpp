#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 emitterPos, int particlesPerCallRate, 
	ResourceManager* resourceManager) : emitterPos(emitterPos), 
	particlesPerCallRate(particlesPerCallRate), resourceManager(resourceManager)
{
		
}

ParticleSystem::~ParticleSystem()
{
	for (auto particle : particles)
	{
		delete particle;
	}

	particles.clear();
}

void ParticleSystem::update(float dt)
{
	std::vector<int> particlesToErase;
	
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i]->update(dt);

		// Check if the particle has exceeded its lifespan
		if (particles[i]->hasExpired())
		{
			// Add the particle to a vector as deleting it in this loop would get messy.
			particlesToErase.push_back(i);
		}
	}

	// Now delete the destroyed particles
	for (unsigned int i = 0; i < particlesToErase.size(); i++)
	{
		delete particles[particlesToErase[i] - i];
		particles.erase(particles.begin() + (particlesToErase[i] - i));
	}

	particlesToErase.clear();
	//Maybe put generateNewParticles here but might be better to have for external calls only
}

void ParticleSystem::setEmitterPos(glm::vec3 newPos)
{
	emitterPos = newPos;
}

void ParticleSystem::render(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader)
{
	for (auto particle : particles)
	{
		particle->draw(viewMatrix, projMatrix, shader);
	}
}

void ParticleSystem::generateNewParticles()
{
	for (int i = 0; i < particlesPerCallRate; i++)
	{

		glm::vec3 randomVelocity;
		randomVelocity.x = Utility::randomFloat(-1.0f, 1.0f);
		randomVelocity.y = Utility::randomFloat(-1.0f, 1.0f);
		randomVelocity.z = Utility::randomFloat(-1.0f, 1.0f);

		Particle* particle = new Particle(emitterPos, 0.05f, randomVelocity, 2, "box.obj", "particle.png", resourceManager);
		
		
		particles.push_back(particle);
	}
}