#include <SDL_ttf.h>
#include <glm.hpp> 
#include <gtc/matrix_transform.hpp> 

#include "State.h"
#include "StateManager.h"

#include "../Player.h"
#include "../Camera.h"
#include "../TargetManager.h"
#include "../Laser.h"
#include "../audio/SFX.h"
#include "../audio/Music.h"
#include "../UI.h"

#pragma once

/**
@class PlayState
@brief The state which contains the game and all gameplay elements
*/
class PlayState :public State
{
public:
	/**
	@brief Create the State.

	@param [in,out] manager Pointer to the current state manager.
	@param [in,out] resourceManager Pointer to Resource manager.
	*/
	PlayState(StateManager* manager, ResourceManager* resourceManager);

	~PlayState();

	/**
	@brief Handles any events such as keyboard/mouse input
	*/
	bool eventHandler();

	/**
	@brief Update any internal values
	@param float - delta time
	*/
	void update(float dt);

	/**
	@brief Render state
	*/
	void render();

private:
	
	/**
	@brief Check if the player is going to collide with the scenery next frame
	@param float - DeltaTime
	*/
	void collision(float);

	/** @brief Fires a laser and adds it to the lasers vector. */
	void fire();

	/**
	 @brief Updates the lasers and checks for any collisions.
	
	 @param dt The deltatime.
	 */
	void updateLasers(float dt);

	/** @brief Is the player trying to fire */
	bool firing;

	/** @brief The score. */
	unsigned int score;

	/** @brief A standard shader that renders and textures a 3D object */
	Shader* standardShader;

	Shader* shader2D;

	/** @brief The camera that holds the View and Projection matrices. */
	Camera* camera;

	//Objects
	///The Player
	Player* playerShip;
	
	/** @brief Manages the randomly spawning targets */
	TargetManager* targetManager;

	/** @brief Contains the lasers that have been fired. */
	std::vector<Laser*> lasers;

	Audio* laserSFX;

	UI* buttonTest;
};