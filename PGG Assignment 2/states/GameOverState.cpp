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
	ship->update(dt);

	camera->updateViewMat(ship->getPos());
}

void GameOverState::render()
{
	

	ship->draw(camera->getViewMatrix(), camera->getProjMatrix(), standardShader);

	BG->draw(shader2D);
	GameOverText->draw(shader2D);
	FinalScore->draw(shader2D);
}