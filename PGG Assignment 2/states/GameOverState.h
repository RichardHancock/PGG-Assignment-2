#pragma once

#include <SDL_ttf.h>
#include <glm.hpp> 
#include <gtc/matrix_transform.hpp> 

#include "State.h"
#include "StateManager.h"

#include "../UI.h"
#include "../audio/Music.h"
#include "../Camera.h"
#include "../Entity.h"

class GameOverState : public State
{
public:

	/**
	@brief Create the State.

	@param [in,out] manager Pointer to the current state manager.
	@param [in,out] resourceManager Pointer to Resource manager.
	@param score The Player's final score 
	*/
	GameOverState(StateManager* manager, ResourceManager* resourceManager, unsigned int score);

	~GameOverState();

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
	/** @brief A standard shader that renders and textures a 3D object */
	Shader* standardShader;

	Shader* shader2D;

	/** @brief The camera that holds the View and Projection matrices. */
	Camera* camera;

	Entity* ship;

	Entity* spaceBackground;

	Music* gameOverMusic;

	//UI
	/** @brief The background. */
	UI* BG;
	
	/** @brief The game over text. */
	UI* GameOverText;

	/** @brief Displays the final score. */
	UI* FinalScore;
};