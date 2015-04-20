#pragma once

#include <string>
#include <utility>
#include <unordered_map>

#include "GameModel.h"
#include "audio/Audio.h"

class ResourceManager
{
private:
	struct Resource
	{
		unsigned int instances;
		bool keepLoaded;
	};

public:

	Audio* getAudio(std::string audioFilename, bool isMusic);

	GameModel* getModel(std::string modelFilename);

	enum ResourceTypes
	{
		ModelFile,
		AudioFile,
		TextureFile
	};
	
	void freeResourceInstance(std::string filename, ResourceTypes resourceType);

	void keepResourceLoaded(std::string filename, ResourceTypes resourceType);



private:

	std::unordered_map<std::string, std::pair<GameModel*, Resource>> models;

	std::unordered_map<std::string, std::pair<Audio*, Resource>> audio;

	//std::unordered_map<std::string, std::pair<Texture, Resource>> textures;
};