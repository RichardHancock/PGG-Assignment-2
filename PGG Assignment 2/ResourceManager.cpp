#include "ResourceManager.h"
#include "OBJLoader.h"
#include "audio/Music.h"
#include "audio/SFX.h"

Audio* ResourceManager::getAudio(std::string audioFilename, bool isMusic)
{
	if (audio.count(audioFilename) > 0)
	{
		audio[audioFilename].second.instances++;
		return audio[audioFilename].first;
	}

	Audio* audioFile;
			
	if (isMusic)
	{
		audioFile = new Music(audioFilename);
	}
	else
	{
		audioFile = new SFX(audioFilename);
	}

	Resource resourceData;
	resourceData.instances = 1;
	resourceData.keepLoaded = false;

	audio[audioFilename] = std::make_pair(audioFile, resourceData);
	
	return audioFile;
}

GameModel* ResourceManager::getModel(std::string modelFilename)
{
	if (models.count(modelFilename) > 0)
	{
		models[modelFilename].second.instances++;
		return models[modelFilename].first;
	}

	GameModel* modelData = new GameModel(modelFilename);

	Resource resourceData;
	resourceData.instances = 1;
	resourceData.keepLoaded = false;

	models[modelFilename] = std::make_pair(modelData, resourceData);

	return modelData;
}

void ResourceManager::freeResourceInstance(std::string filename, ResourceTypes resourceType)
{
	//This could possibly be replaced by a template function
	switch (resourceType)
	{
	case ModelFile:
		models[filename].second.instances--;
		
		if (models[filename].second.instances <= 0 && !models[filename].second.keepLoaded)
		{
			delete models[filename].first;
			models.erase(filename);
		}

		break;
	case AudioFile:
		audio[filename].second.instances--;

		if (audio[filename].second.instances <= 0 && !audio[filename].second.keepLoaded)
		{
			delete audio[filename].first;
			audio.erase(filename);
		}

		break;
	case TextureFile:

		break;
	}
}

void ResourceManager::keepResourceLoaded(std::string filename, ResourceTypes resourceType)
{
	//This could possibly be replaced by a template function
	switch (resourceType)
	{
	case ModelFile:
		models[filename].second.keepLoaded = true;

		break;
	case AudioFile:
		audio[filename].second.keepLoaded = true;

		break;
	case TextureFile:

		break;
	}
}