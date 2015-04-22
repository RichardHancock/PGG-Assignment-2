#include "PlayState.h"
#include "../Utility.h"

PlayState::PlayState(StateManager* manager, ResourceManager* resourceManager)
	: State(manager, resourceManager)
{
	stateName = "Play State";

	score = 0;

	//Load Standard Shader
	std::string shadPath = resourceManager->shaderDir;
	standardShader = new Shader(shadPath + "vertex.shader", shadPath + "fragment.shader");

	shader2D = new Shader(shadPath + "2D vertex.shader", shadPath + "2D fragment.shader");

	camera = new Camera();

	playerShip = new Player(glm::vec3(0), glm::vec3(0, Utility::HALF_PI, 0), glm::vec3(0.2, 0.2, 0.2),
		"ship.obj", "ship.png", resourceManager);

	targetManager = new TargetManager(8, glm::vec2(10, 6), resourceManager);

	laserSFX = resourceManager->getAudio("laser.wav", false);
	shipTargetCollisionSFX = resourceManager->getAudio("bong.wav", false);
	targetExplosionSFX = resourceManager->getAudio("explosion.wav", false);
	bgMusic = resourceManager->getAudio("Cephalopod.wav", true);

	bgMusic->play(0, -1);

	font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 28);

	if (font == nullptr)
	{
		Utility::log(Utility::I, "Font in play state failed to load");
	}

	buttonBG1 = new UI(glm::vec2(0.5f, 0.7f), glm::vec2(0.45f, 0.25f), "buttonBG.png", resourceManager);
	buttonBG2 = new UI(glm::vec2(-0.95f, 0.7f), glm::vec2(0.45f, 0.25f), "buttonBG.png", resourceManager);
	
	refreshUI();

	//Start the spawning and make the player start moving forward
	playerShip->toggleForwardMovement();
	targetManager->initSpawning();

	firing = false;
	Utility::Timer::createTimer("FireDelay", 0.2f);
}

PlayState::~PlayState()
{
	bgMusic->stop(0);
	laserSFX->stop(0);
	shipTargetCollisionSFX->stop(0);
	targetExplosionSFX->stop(0);

	delete buttonBG1;
	delete buttonBG2;
	delete buttonText1;
	delete buttonText2;

	TTF_CloseFont(font);

	delete bgMusic;
	delete laserSFX;
	delete shipTargetCollisionSFX;
	delete targetExplosionSFX;

	delete targetManager;
	delete playerShip;
	delete camera;
	delete shader2D;
	delete standardShader;
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
		playerShip->hit();
		shipTargetCollisionSFX->play(0, 0);

		refreshUI();
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

	buttonBG1->draw(shader2D);
	buttonBG2->draw(shader2D);
	buttonText1->draw(shader2D);
	buttonText2->draw(shader2D);
}

void PlayState::refreshUI()
{
	delete buttonText1;
	delete buttonText2;
	
	//Show the Player's current lives
	buttonText1 = new UI(glm::vec2(0.5f, 0.7f), glm::vec2(0.45f, 0.25f),
		"Lives: " + std::to_string(playerShip->getCurrentLife()), font, resourceManager);

	//Show the Player's current score
	buttonText2 = new UI(glm::vec2(-0.95f, 0.7f), glm::vec2(0.45f, 0.25f),
		"Score: " + std::to_string(score), font, resourceManager);
}

void PlayState::fire()
{
	glm::vec3 laserPos = playerShip->getTurretPos();
	glm::vec3 rotation = glm::vec3(Utility::HALF_PI, 0, 0);
	glm::vec3 scale = glm::vec3(0.2, 0.2, 0.2);
	lasers.push_back(new Laser(laserPos, rotation, scale, "laser.obj",
		"laser.png", resourceManager));
	
	laserSFX->play(0, 0);

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
			score++;
			curLaser->hit();

			targetExplosionSFX->play(0, 0);
			refreshUI();
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