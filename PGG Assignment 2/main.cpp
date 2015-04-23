#include <SDL.h>
#include "SDL_mixer.h"

#include "dependencies/glew.h"

#include <windows.h>

#include <glm.hpp> // This is the main GLM header
#include <gtc/matrix_transform.hpp> // This one lets us use matrix transformations

#include "ResourceManager.h"
#include "Utility.h"
#include "states/StateManager.h"
#include "states/PlayState.h"


//This forces NVIDIA hybrid GPU's (Intel and Nvidia integrated) to use the high performance NVidia chip rather than the Intel.
//This was recommended by NVidia's policies: http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

// An initialisation function, mainly for GLEW
// This will also print to console the version of OpenGL we are using
bool InitGL()
{

	// GLEW has a problem with loading core OpenGL
	// See here: https://www.opengl.org/wiki/OpenGL_Loading_Library
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return true;
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Something went very wrong in initialisation, all we can do is exit
		Utility::log(Utility::E,"Whoops! Something went very wrong, cannot initialise SDL");
		return -1;
	}

	TTF_Init();

	Mix_Init(MIX_INIT_OGG);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Utility::log(Utility::E, "SDL_mixer init failed: " + std::string(Mix_GetError()));
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int winPosX = SDL_WINDOWPOS_CENTERED;
	int winPosY = SDL_WINDOWPOS_CENTERED;
	int winWidth = 640;
	int winHeight = 480;
	SDL_Window *window = SDL_CreateWindow("PGG Assignment 2 by Richard Hancock",  // The first parameter is the window title
		winPosX, winPosY,
		winWidth, winHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	// Call our initialisation function to set up GLEW and print out some GL info to console
	if (!InitGL())
	{
		return -1;
	}


	unsigned int lastTime = SDL_GetTicks();

	glEnable(GL_DEPTH_TEST);

	Utility::randomInit();

	//Create the Resource manager that loads resources
	ResourceManager* resourceManager = new ResourceManager(renderer);

	StateManager* manager = new StateManager(winWidth, winHeight);
	manager->addState(new PlayState(manager, resourceManager));

	bool go = true;
	while (go)
	{
		unsigned int current = SDL_GetTicks();
		
		float deltaTs = (float)(current - lastTime) / 1000.0f;

		// Now that we've done this we can use the current time as the next frame's previous time
		lastTime = current;

		//Event Handling
		go = manager->eventHandler();
		
		//Update
		manager->update(deltaTs);
		
		// Draw our world
		// 
		// Specify the colour to clear the framebuffer to
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// This writes the above colour to the colour part of the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		manager->render();

		// This tells the renderer to actually show its contents to the screen
		// We'll get into this sort of thing at a later date - or just look up 'double buffering' if you're impatient :P
		SDL_GL_SwapWindow(window);
		

		// Limiter in case we're running really quick
		if (deltaTs < (1.0f / 50.0f))	// not sure how accurate the SDL_Delay function is..
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTs)*1000.0f));
		}
	}

	// If we get outside the main game loop, it means our user has requested we exit

	manager->popLastState();
	delete manager;

	delete resourceManager;

	// Our cleanup phase, hopefully fairly self-explanatory ;)
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
