#pragma once

#include <string>
#include <utility>
#include <unordered_map>

#include "GameModel.h"
#include "audio/Audio.h"
#include "Texture.h"

class GameModel;

class ResourceManager
{
private:
	struct Resource
	{
		unsigned int instances;
		bool keepLoaded;
	};

public:

	ResourceManager(SDL_Renderer* renderer);

	~ResourceManager();

	Audio* getAudio(std::string audioFilename, bool isMusic);

	GameModel* getModel(std::string modelFilename, std::string textureFilename);

	Texture* getTexture(std::string textureFilename);

	enum ResourceTypes
	{
		ModelFile,
		AudioFile,
		TextureFile
	};
	
	void freeResourceInstance(std::string filename, ResourceTypes resourceType);

	void keepResourceLoaded(std::string filename, ResourceTypes resourceType);

	SDL_Renderer* getRenderer() { return renderer; }

	const std::string modelDir, shaderDir, audioDir, textureDir;

private:

	std::unordered_map<std::string, std::pair<GameModel*, Resource>> models;

	std::unordered_map<std::string, std::pair<Audio*, Resource>> audio;

	std::unordered_map<std::string, std::pair<Texture*, Resource>> textures;
	
	SDL_Renderer* renderer;

	
};