#include <SDL_ttf.h>
#include <glm.hpp> 
#include <gtc/matrix_transform.hpp> 

#include "State.h"
#include "StateManager.h"

#include "../Utility.h"
#include "../Player.h"
#include "../Camera.h"
#include "../TargetManager.h"
#include "../Laser.h"

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

	void fire();

	void updateLasers(float dt);

	bool firing;

	unsigned int score;

	Shader* standardShader;

	Camera* camera;

	//Objects
	///The Player
	Player* playerShip;
	
	TargetManager* targetManager;

	std::vector<Laser*> lasers;
};