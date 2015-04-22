#include "PlayState.h"

PlayState::PlayState(StateManager* manager, ResourceManager* resourceManager)
	: State(manager, resourceManager)
{
	stateName = "Play State";

	score = 0;

	//Load Standard Shader
	std::string shadPath = resourceManager->shaderDir;
	standardShader = new Shader(shadPath + "vertex.shader", shadPath + "fragment.shader");

	camera = new Camera();

	playerShip = new Player(glm::vec3(0), glm::vec3(0, Utility::HALF_PI, 0), glm::vec3(0.2, 0.2, 0.2),
		"ship.obj", "ship.png", resourceManager);

	targetManager = new TargetManager(8, glm::vec2(10, 6), resourceManager);

	playerShip->toggleForwardMovement();
	targetManager->initSpawning();
	firing = false;
	Utility::Timer::createTimer("FireDelay", 0.2f);
}

PlayState::~PlayState()
{

}

bool PlayState::eventHandler()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYUP:

			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
				break;

			case SDLK_SPACE:
				firing = false;
				break;

			default:
				playerShip->handleInputs(e);
				break;
			}
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				firing = true;
				break;

			default:
				playerShip->handleInputs(e);
				break;
			}
			break;

		}
	}

	return true;
}

void PlayState::update(float dt)
{
	if (firing && Utility::Timer::hasTimerFinished("FireDelay"))
		fire();

	playerShip->update(dt);

	Utility::Timer::update(dt);

	targetManager->update(dt, playerShip->getPos().z);

	camera->updateViewMat(playerShip->getPos());

	//Collision
	if (targetManager->checkForCollisions(playerShip->getAABB()))
	{
		Utility::log(Utility::I, "Colliding");
		playerShip->hit();
	}

	//Updates Lasers and checks for collisions
	updateLasers(dt);

}

void PlayState::render()
{
	glm::mat4 Projection = camera->getProjMatrix();
	glm::mat4 View = camera->getViewMatrix();

	// Draw the object using the given view (which contains the camera orientation) and projection (which contains information about the camera 'lense')
	playerShip->draw(View, Projection, standardShader);
	//barrel->draw(View, Projection, standardShader);
	for (auto laser : lasers)
	{
		laser->draw(View, Projection, standardShader);
	}

	targetManager->draw(View, Projection, standardShader);

	//glm::mat4 Projection2D = glm::ortho(0, winWidth, winHeight, 0);
}

void PlayState::collision(float dt)
{
	
}

void PlayState::fire()
{
	glm::vec3 laserPos = playerShip->getTurretPos();
	glm::vec3 rotation = glm::vec3(Utility::HALF_PI, 0, 0);
	glm::vec3 scale = glm::vec3(0.2, 0.2, 0.2);
	lasers.push_back(new Laser(laserPos, rotation, scale, "laser.obj",
		"laser.png", resourceManager));

	Utility::Timer::createTimer("FireDelay", 1.0f);
}

void PlayState::updateLasers(float dt)
{
	std::vector<unsigned int> toDelete;

	for (unsigned int i = 0; i < lasers.size(); i++)
	{
		auto curLaser = lasers[i];

		curLaser->update(dt);

		if (targetManager->checkForCollisions(curLaser->getAABB()))
		{
			Utility::log(Utility::I, " Laser Colliding");
			score += 10;
			curLaser->hit();
		}

		if (curLaser->isDead())
		{
			toDelete.push_back(i);
		}
	}

	//Delete Marked lasers
	for (unsigned int i = 0; i < toDelete.size(); i++)
	{
		unsigned int indexToDelete = toDelete[i] - i;

		delete lasers[indexToDelete];
		lasers.erase(lasers.begin() + indexToDelete);
	}

	toDelete.clear();
}