#pragma once

#include <string>
#include <utility>
#include <unordered_map>

#include "GameModel.h"
#include "audio/Audio.h"
#include "Texture.h"

// Forward Declaration
class GameModel;

/**
@brief Manager for resources of most types.

Loads and manages any resources.
*/
class ResourceManager
{
private:
	/** 
	@brief A resource info struct. 
	
	Contains how many objects are using this resource. 
	Contains whether the file should be kept loaded even when no object is using it.
	*/
	struct Resource
	{
		unsigned int instances;
		bool keepLoaded;
	};

public:
	/**
	 @brief Constructor.
	
	 @param [in,out] renderer Resource Manager.
	 */
	ResourceManager(SDL_Renderer* renderer);

	~ResourceManager();

	/**
	 @brief Gets a audio object, loads the audio if not already loaded.
	
	 @param audioFilename Filename of the audio file.
	 @param isMusic		  true if this audio is music, rather than a sfx.
	
	 @return null if it fails, else the audio.
	 */
	Audio* getAudio(std::string audioFilename, bool isMusic);

	/**
	 @brief Gets a model, loads the model if not already loaded.
	
	 @param modelFilename   Filename of the model file.
	 @param textureFilename Filename of the texture file.
	
	 @return null if it fails, else the model.
	 */
	GameModel* getModel(std::string modelFilename, std::string textureFilename);

	/**
	 @brief Gets a texture, loads the texture if not already loaded.
	
	 @param textureFilename Filename of the texture file.
	
	 @return null if it fails, else the texture.
	 */
	Texture* getTexture(std::string textureFilename);

	/** @brief Values that represent resource types. */
	enum ResourceTypes
	{
		ModelFile,
		AudioFile,
		TextureFile
	};
	
	/**
	 @brief Free resource instance. Will delete the asset if nothing else is using it.
	
	 @param filename	 Filename of the resource.
	 @param resourceType Type of the resource.
	 */
	void freeResourceInstance(std::string filename, ResourceTypes resourceType);

	/**
	 @brief Keep resource loaded even after its no longer being used.
	
	 @param filename	 Filename of the resource.
	 @param resourceType Type of the resource.
	 */
	void keepResourceLoaded(std::string filename, ResourceTypes resourceType);
	
	/**
	 @brief Gets the renderer.
	
	 @return null if it fails, else the renderer.
	 */
	SDL_Renderer* getRenderer() { return renderer; }

	/** @brief The directory for the relevant asset type */
	const std::string modelDir, shaderDir, audioDir, textureDir;

private:
	/** @brief The models. */
	std::unordered_map<std::string, std::pair<GameModel*, Resource>> models;

	/** @brief The audio. */
	std::unordered_map<std::string, std::pair<Audio*, Resource>> audio;

	/** @brief The textures. */
	std::unordered_map<std::string, std::pair<Texture*, Resource>> textures;

	/** @brief The renderer. */
	SDL_Renderer* renderer;

	
};