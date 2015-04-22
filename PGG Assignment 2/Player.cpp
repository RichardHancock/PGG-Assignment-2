#include "Player.h"

Player::Player(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, std::string modelFilename,
	std::string textureFilename, ResourceManager* manager)
	: Entity(initPosition, rotation, scale, modelFilename, textureFilename, manager)
{
	allowedToMove = false;

	//These values were determined by looking through the OBJ file
	aabb = new AABB(glm::vec3(-2.2, -0.2, -3.85), glm::vec3(10.0, 3.5, 3.85));
	//aabb = new AABB(glm::vec3(-3.85, -0.2, -2.2), glm::vec3(3.85, 3.5, 10.0));
	
	lives = 3;
}

Player::~Player()
{

}

void Player::update(float dt)
{

	directionMovement();

	if (allowedToMove)
	{
		pos.z -= forwardSpeed * dt;
	}

	pos.x += velocity.x * dt;
	pos.y += velocity.y * dt;

	velocity = glm::vec2(0);

	Entity::update(dt);
}

void Player::directionMovement()
{
	if (movingLeft && !movingRight)
	{
		velocity.x = (float)(-speed);
	}
	if (movingRight && !movingLeft)
	{
		velocity.x = (float)speed;
	}

	if (movingUp && !movingDown)
	{
		velocity.y = (float)speed;
	}
	if (movingDown && !movingUp)
	{
		velocity.y = (float)(-speed);
	}
}

void Player::handleInputs(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_DOWN:
		case SDLK_s:
			movingDown = true;
			break;

		case SDLK_UP:
		case SDLK_w:
			movingUp = true;
			break;

		case SDLK_LEFT:
		case SDLK_a:
			movingLeft = true;
			break;

		case SDLK_RIGHT:
		case SDLK_d:
			movingRight = true;
			break;
		}
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_DOWN:
		case SDLK_s:
			movingDown = false;
			break;

		case SDLK_UP:
		case SDLK_w:
			movingUp = false;
			break;

		case SDLK_LEFT:
		case SDLK_a:
			movingLeft = false;
			break;

		case SDLK_RIGHT:
		case SDLK_d:
			movingRight = false;
			break;
		}
		break;
	}
}