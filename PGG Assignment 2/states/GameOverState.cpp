#include "GameOverState.h"
#include "PlayState.h"
#include "../Utility.h"

GameOverState::GameOverState(StateManager* manager, ResourceManager* resourceManager, 
	unsigned int score)
	: State(manager, resourceManager)
{
	stateName = "Game Over State";

	//Add a delay to stop the player accidentally switching straight back to the play state
	Utility::Timer::createTimer("SpaceKeyDelay", 2.0f);

	

	//Load Standard Shaders
	std::string shadPath = resourceManager->shaderDir;
	standardShader = new Shader(shadPath + "vertex.shader", shadPath + "fragment.shader");
	shader2D = new Shader(shadPath + "2D vertex.shader", shadPath + "2D fragment.shader");

	camera = new Camera();

	ship = new Entity(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.2, 0.2),
		"ship.obj", "ship.png", resourceManager);

	//Randomly selects one of the available background images
	std::string bgImage = "starfield" + std::to_string(Utility::randomInt(1, 3)) + ".png";

	//Create the Background and set it 100 in front of the player
	spaceBackground = new Entity(glm::vec3(0, -40, ship->getPos().z - 100), glm::vec3(Utility::HALF_PI, 0, 0),
		glm::vec3(12), "flatPlane.obj", bgImage, resourceManager);


	//UI
	TTF_Font* font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 28);

	if (font == nullptr)
	{
		Utility::log(Utility::I, "Font in play state failed to load");
	}

	//UI
	BG = new UI(glm::vec2(-0.8f, -0.8f), glm::vec2(1.6f, 1.6f), "buttonBG.png", resourceManager);
	
	GameOverText = new UI(glm::vec2(-0.5f, 0.4f), glm::vec2(1.0f, 0.2f), 
		"Game Over - Press Space to Retry", font, resourceManager);

	//Display Players score
	FinalScore = new UI(glm::vec2(-0.2f, 0.0f), glm::vec2(0.4f, 0.2f), 
		"Score: " + std::to_string(score), font, resourceManager);

	gameOverMusic = new Music("resources/audio/Full On.ogg");
	gameOverMusic->play(0, -1);

}

GameOverState::~GameOverState()
{
	Utility::Timer::stopTimer("SpaceKeyDelay");

	delete gameOverMusic;

	delete BG;
	delete GameOverText;
	delete FinalScore;

	delete ship;
	delete camera;
	delete shader2D;
	delete standardShader;
}

bool GameOverState::eventHandler()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
				break;

			case SDLK_SPACE:
				if (Utility::Timer::hasTimerFinished("SpaceKeyDelay"))
				{
					gameOverMusic->stop(0);
					stateManager->changeState(new PlayState(stateManager, resourceManager));
				}
				break;
			}
			break;

		}
	}

	return true;
}

void GameOverState::update(float dt)
{
	glm::vec3 rotation = ship->getRotation();
	rotation.y += dt * 0.5f;

	while (rotation.y > (3.14159265358979323846 * 2.0))
	{
		rotation.y -= (float)(3.14159265358979323846 * 2.0);
	}

	ship->setRotation(rotation);

	ship->update(dt);

	spaceBackground->update(dt);

	//Offset the camera (Basically zooms out a little)
	glm::vec3 camOffset = ship->getPos();
	camOffset.z += 1;
	camera->updateViewMat(camOffset);
}

void GameOverState::render()
{
	glm::mat4 Projection = camera->getProjMatrix();
	glm::mat4 View = camera->getViewMatrix();

	ship->draw(View, Projection, standardShader);

	spaceBackground->draw(View, Projection, standardShader);

	//2D UI
	BG->draw(shader2D);
	GameOverText->draw(shader2D);
	FinalScore->draw(shader2D);
}