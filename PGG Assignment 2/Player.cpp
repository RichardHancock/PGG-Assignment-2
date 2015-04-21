#include "Player.h"

Player::Player(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{
	allowedToMove = false;
}

Player::~Player()
{

}

void Player::update(float dt)
{

	if (allowedToMove)
	{
		pos.z -= forwardSpeed * dt;
	}

	Entity::update(dt);
}