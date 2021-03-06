#include "../dependencies/SDL.h"
#include <string>
#include "../ResourceManager.h"

#pragma once
class StateManager;

//Reference: Pete Allen's Game State Example
//Used in other assignments

/**
@class State
@brief A game state that contains one screen of the game such as a Menu or the game itself
*/
class State
{
public:

	/**
	 @brief Create the State.
	
	 @param [in,out] manager Pointer to the current state manager.
	 @param [in,out] resourceManager Pointer to Resource manager.
	 */
	State(StateManager* manager, ResourceManager* resourceManager);

	virtual ~State() {}

	/**
	@brief Handles any events such as keyboard/mouse input
	@return Continue the game
	*/
	virtual bool eventHandler() = 0;

	/**
	@brief Update any internal values
	@param dt delta time
	*/
	virtual void update(float dt) = 0;

	/**
	@brief Render any sprites relevant to the state
	*/
	virtual void render() = 0;

	/**
	@brief Get this states name
	@return std::string - Name
	*/
	std::string getStateName() { return stateName; }
protected:
	///Pointer to state manager
	StateManager* stateManager;

	/// Resource Manager
	ResourceManager* resourceManager;

	///This states name
	std::string stateName;
};