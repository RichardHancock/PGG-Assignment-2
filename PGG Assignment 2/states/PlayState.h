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
	
	/** @brief Refresh user interface. (Updates the score and lives UI elements) */
	void refreshUI();

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

	/** @brief A standard shader that renders and textures a 2D object. */
	Shader* shader2D;

	/** @brief The camera that holds the View and Projection matrices. */
	Camera* camera;

	//Objects
	/** @brief The player ship. */
	Player* playerShip;
	
	/** @brief Manages the randomly spawning targets */
	TargetManager* targetManager;

	/** @brief Contains the lasers that have been fired. */
	std::vector<Laser*> lasers;

	/** @brief The space background Plane. */
	Entity* spaceBackground;

	//Audio
	/** @brief The laser sfx. */
	Audio* laserSFX;

	/** @brief The ship collision sfx. */
	Audio* shipTargetCollisionSFX;

	/** @brief Target explosion sfx. */
	Audio* targetExplosionSFX;

	/** @brief The background music. */
	Music* bgMusic;

	/** @brief The font. */
	TTF_Font* font;

	/** @brief The background for the left UI panel. */
	UI* BG1;

	/** @brief The background for the right UI panel. */
	UI* BG2;

	/** @brief The text for the left UI panel. */
	UI* Text1;

	/** @brief The text for the right UI panel. */
	UI* Text2;
};