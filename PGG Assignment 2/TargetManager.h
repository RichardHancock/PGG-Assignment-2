#pragma once

#include "Target.h"
#include "ResourceManager.h"

class TargetManager
{
public:

	TargetManager(int spacing, glm::vec2 spawnArea, ResourceManager* manager);

	~TargetManager();

	void update(float dt, float playerZPos);

	void draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);

	void toggleSpawning() { spawning = !spawning; }

	void initSpawning();

	bool checkForCollisions(AABB* other);
private:

	void spawnTarget();

	ResourceManager* resourceManager;

	float playerZPos;

	float nextSpawnZPos;

	bool spawning;

	std::vector<Target*> targets;

	const int spacing;

	const glm::vec2 spawnArea;

};