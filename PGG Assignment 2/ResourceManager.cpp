#include "ResourceManager.h"
#include "OBJLoader.h"
#include "audio/Music.h"
#include "audio/SFX.h"

ResourceManager::ResourceManager(SDL_Renderer* renderer)
	: renderer(renderer), modelDir("resources/models/"), audioDir("resources/audio/"), 
	shaderDir("resources/shaders/"), textureDir("resources/textures/")
{

}

Audio* ResourceManager::getAudio(std::string audioFilename, bool isMusic)
{
	audioFilename = audioDir + audioFilename;

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

GameModel* ResourceManager::getModel(std::string modelFilename, std::string textureFilename)
{
	modelFilename = modelDir + modelFilename;

	if (models.count(modelFilename) > 0)
	{
		models[modelFilename].second.instances++;
		return models[modelFilename].first;
	}

	GameModel* modelData = new GameModel(modelFilename, textureFilename, this);

	Resource resourceData;
	resourceData.instances = 1;
	resourceData.keepLoaded = false;

	models[modelFilename] = std::make_pair(modelData, resourceData);

	return modelData;
}

Texture* ResourceManager::getTexture(std::string textureFilename)
{
	textureFilename = textureDir + textureFilename;

	if (textures.count(textureFilename) > 0)
	{
		textures[textureFilename].second.instances++;
		return textures[textureFilename].first;
	}

	Texture* textureData = new Texture(textureFilename, renderer, true);

	Resource resourceData;
	resourceData.instances = 1;
	resourceData.keepLoaded = false;

	textures[textureFilename] = std::make_pair(textureData, resourceData);

	return textureData;
}

void ResourceManager::freeResourceInstance(std::string filename, ResourceTypes resourceType)
{
	//This could possibly be replaced by a template function
	switch (resourceType)
	{
	case ModelFile:
		filename = modelDir + filename;

		models[filename].second.instances--;
		
		if (models[filename].second.instances <= 0 && !models[filename].second.keepLoaded)
		{
			delete models[filename].first;
			models.erase(filename);
		}

		break;
	case AudioFile:
		filename = audioDir + filename;

		audio[filename].second.instances--;

		if (audio[filename].second.instances <= 0 && !audio[filename].second.keepLoaded)
		{
			delete audio[filename].first;
			audio.erase(filename);
		}

		break;
	case TextureFile:
		filename = textureDir + filename;

		textures[filename].second.instances--;

		if (textures[filename].second.instances <= 0 && !textures[filename].second.keepLoaded)
		{
			delete textures[filename].first;
			textures.erase(filename);
		}
		break;
	}
}

void ResourceManager::keepResourceLoaded(std::string filename, ResourceTypes resourceType)
{
	//This could possibly be replaced by a template function
	switch (resourceType)
	{
	case ModelFile:
		models[modelDir + filename].second.keepLoaded = true;

		break;
	case AudioFile:
		audio[audioDir + filename].second.keepLoaded = true;

		break;
	case TextureFile:
		textures[textureDir + filename].second.keepLoaded = true;

		break;
	}
}