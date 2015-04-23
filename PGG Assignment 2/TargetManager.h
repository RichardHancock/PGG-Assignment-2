#pragma once

#include "Target.h"
#include "ResourceManager.h"

/**
@brief Manager for targets. 

Manages the life of targets and determines the spawn location
*/
class TargetManager
{
public:

	/**
	 @brief Constructor.
	
	 @param spacing			 The spacing between targets in the z axis.
	 @param spawnArea		 The spawn area (Width, Height).
	 @param [in,out] manager Resource Manager.
	 */
	TargetManager(int spacing, glm::vec2 spawnArea, ResourceManager* manager);

	~TargetManager();

	/**
	 @brief Updates this TargetManager.
	
	 @param dt		   The deltatime.
	 @param playerZPos The player z coordinate position.
	 */
	void update(float dt, float playerZPos);

	/**
	 @brief Draws the targets.
	
	 @param [in,out] viewMatrix The view matrix.
	 @param [in,out] projMatrix The project matrix.
	 @param [in,out] shader       The shader used to render the targets.
	 */
	void draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);

	/** @brief Toggles the spawning of targets. */
	void toggleSpawning() { spawning = !spawning; }

	/** @brief Initialises the spawning. (spawns a full array of targets) */
	void initSpawning();

	/**
	 @brief Check for collisions between the targets and the passed in AABB.
	
	 @param [in,out] AABB to test against.
	
	 @return true if collision occurred, false if none.
	 */
	bool checkForCollisions(AABB* other);
private:

	/** @brief Spawn a target. */
	void spawnTarget();

	/** @brief Resource Manager */
	ResourceManager* resourceManager;

	/** @brief The player's last knows Z coordinate position. */
	float playerZPos;

	/** @brief The next z coordinate spawn position. */
	float nextSpawnZPos;

	/** @brief Are targets being spawned */
	bool spawning;
	
	/** @brief The targets. */
	std::vector<Target*> targets;

	/** @brief The spacing between targets in the z axis. */
	const int spacing;

	/** @brief The spawn area. */
	const glm::vec2 spawnArea;

};