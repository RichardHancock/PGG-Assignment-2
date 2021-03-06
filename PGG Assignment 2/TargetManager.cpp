#include "TargetManager.h"
#include "Utility.h"

TargetManager::TargetManager(int spacing, glm::vec2 spawnArea, ResourceManager* manager)
	: spacing(spacing), spawnArea(spawnArea), resourceManager(manager)
{
	spawning = false;
	nextSpawnZPos = (float)-spacing - 2;

	//Initialize particle system used for explosions
	particles = new ParticleSystem(glm::vec3(0), 64, resourceManager);

}

TargetManager::~TargetManager()
{
	for (auto target : targets)
	{
		delete target;
	}

	targets.clear();

	delete particles;
}

void TargetManager::update(float dt, float playerZPos)
{
	this->playerZPos = playerZPos;

	//Will contain any indices of targets that need to be deleted
	std::vector<unsigned int> toDelete;

	for (unsigned int i = 0; i < targets.size(); i++)
	{
		//Shorthand
		auto curTarget = targets[i];

		curTarget->update(dt);
		
		//Check if the target has gone past the player, if so delete
		if (curTarget->getPos().z > playerZPos + 10)
		{
			curTarget->hit();
		}

		//Check for dead barrels
		if (curTarget->isDead())
		{
			toDelete.push_back(i);
		}
	}

	//Delete Marked targets
	for (unsigned int i = 0; i < toDelete.size(); i++ )
	{
		unsigned int indexToDelete = toDelete[i] - i;

		delete targets[indexToDelete];
		targets.erase(targets.begin() + indexToDelete);
	}

	toDelete.clear();

	//Check if new target needs to be spawned
	if (targets. size() < 15)
	{
		spawnTarget();
	}

	//Update Explosion Particle System
	particles->update(dt);
}

void TargetManager::draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader)
{
	for (auto target : targets)
	{
		target->draw(viewMatrix, projMatrix, shader);
	}

	particles->render(viewMatrix, projMatrix, shader);
}

void TargetManager::initSpawning()
{
	spawning = true;
	for (unsigned int i = 0; i < 15; i++)
	{
		spawnTarget();
	}
}

bool TargetManager::checkForCollisions(AABB* other)
{
	for (auto target : targets)
	{
		if (target->getAABB()->collides(other))
		{
			//Generate Explosion at the destroyed barrels location
			glm::vec3 offset = target->getPos();
			offset.y += 0.6f;

			particles->setEmitterPos(offset);
			particles->generateNewParticles();

			// Marks the barrel for destruction
			target->hit();
			return true;
		}
	}

	return false;
}

void TargetManager::spawnTarget()
{
	float halfWidth = spawnArea.x / 2;
	float halfHeight = spawnArea.y / 2;

	float x = Utility::randomFloat(-halfWidth, halfWidth);
	float y = Utility::randomFloat(-halfHeight, halfHeight);

	glm::vec3 spawnPos = glm::vec3(x, y, nextSpawnZPos);

	Target* newTarget = new Target(spawnPos, glm::vec3(0), glm::vec3(0.2f), 
		"barrel.obj", "barrel.png", resourceManager);

	targets.push_back(newTarget);

	nextSpawnZPos -= spacing;
}