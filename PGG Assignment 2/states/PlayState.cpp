#include "PlayState.h"
#include "GameOverState.h"
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

	// The area targets will spawn, and that the player can move around
	glm::vec2 playArea(10, 6);

	playerShip = new Player(glm::vec3(0), glm::vec3(0, Utility::HALF_PI, 0), glm::vec3(0.2, 0.2, 0.2),
		playArea, "ship.obj", "ship.png", resourceManager);

	targetManager = new TargetManager(8, playArea, resourceManager);

	//Randomly selects one of the available background images
	std::string bgImage = "starfield" + std::to_string(Utility::randomInt(1, 3)) + ".png";

	//Create the Background and set it 100 in front of the player
	spaceBackground = new Entity(glm::vec3(0, -40, playerShip->getPos().z - 100), glm::vec3(Utility::HALF_PI, 0, 0),
		glm::vec3(12), "flatPlane.obj", bgImage, resourceManager);

	//Sounds/Music
	laserSFX = resourceManager->getAudio("laser.wav", false);
	shipTargetCollisionSFX = resourceManager->getAudio("bong.wav", false);
	targetExplosionSFX = resourceManager->getAudio("explosion.wav", false);
	bgMusic = new Music("resources/audio/Cephalopod.wav");
	
	bgMusic->play(0, -1);

	font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 28);

	if (font == nullptr)
	{
		Utility::log(Utility::I, "Font in play state failed to load");
	}

	BG1 = new UI(glm::vec2(0.5f, 0.7f), glm::vec2(0.45f, 0.25f), "buttonBG.png", resourceManager);
	BG2 = new UI(glm::vec2(-0.95f, 0.7f), glm::vec2(0.45f, 0.25f), "buttonBG.png", resourceManager);
	
	refreshUI();

	//Start the spawning and make the player start moving forward
	playerShip->toggleForwardMovement();
	targetManager->initSpawning();

	firing = false;
	Utility::Timer::createTimer("FireDelay", 0.2f);
}

PlayState::~PlayState()
{
	Utility::Timer::stopTimer("FireDelay");


	laserSFX->stop(0);
	shipTargetCollisionSFX->stop(0);
	targetExplosionSFX->stop(0);

	delete BG1;
	delete BG2;
	delete Text1;
	delete Text2;

	TTF_CloseFont(font);
	font = NULL;

	delete bgMusic;

	resourceManager->freeResourceInstance("bong.wav", ResourceManager::AudioFile);
	resourceManager->freeResourceInstance("laser.wav", ResourceManager::AudioFile);
	resourceManager->freeResourceInstance("explosion.wav", ResourceManager::AudioFile);

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

	//Calculate the backgrounds new Z pos (Could be in a class, but there isn't enough to justify it)
	glm::vec3 bgPos = spaceBackground->getPos();
	bgPos.z = playerShip->getPos().z - 100;
	spaceBackground->setPos(bgPos);

	spaceBackground->update(dt);


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

	if (playerShip->isDead())
	{
		bgMusic->stop(0);
		stateManager->changeState(new GameOverState(stateManager, resourceManager, score));
	}

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

	spaceBackground->draw(View, Projection, standardShader);

	BG1->draw(shader2D);
	BG2->draw(shader2D);
	Text1->draw(shader2D);
	Text2->draw(shader2D);
}

void PlayState::refreshUI()
{
	delete Text1;
	delete Text2;
	
	//Show the Player's current lives
	Text1 = new UI(glm::vec2(0.5f, 0.7f), glm::vec2(0.45f, 0.25f),
		"Lives: " + std::to_string(playerShip->getCurrentLife()), font, resourceManager);

	//Show the Player's current score
	Text2 = new UI(glm::vec2(-0.95f, 0.7f), glm::vec2(0.45f, 0.25f),
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