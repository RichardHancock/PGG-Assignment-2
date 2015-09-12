#include "dependencies/SDL.h"
#include "dependencies/SDL_image.h"
#include <string>

#include "Vec2.h"
#include "Utility.h"

#pragma once

//Used in other assignments, small modifications

/**
@class Texture
@brief Encapsulates an SDL_Texture

Contains the ability to draw, load, etc.. Textures.
*/
class Texture
{
public:
	/**
	@brief Create a Texture
	@param path Path to image
	@param renderer SDL_Renderer
	@param keepSurface Should the surface be kept after the texture is created.
	*/
	Texture(std::string path, SDL_Renderer* renderer, bool keepSurface);

	/**
	@brief Create a Texture
	@param surface A surface to convert into a texture (This is freed internally)
	@param renderer SDL_Renderer
	@param keepSurface Should the surface be kept after the texture is created.
	*/
	Texture(SDL_Surface* surface, SDL_Renderer* renderer, bool keepSurface);

	~Texture();

	/**
	@brief Gets the Dimensions of the stored texture
	@return Vec2 - Dimensions of Sprite
	*/
	Vec2 Texture::getDimensions();

	/**
	@brief Draw the Sprite
	@param pos Position to draw at
	*/
	void Texture::draw(Vec2 pos);

	/**
	@brief Draw the Sprite
	@param pos Position to draw at
	@param facingRight Whether the sprite shouldn't be flipped (True doesn't flip, false does)
	*/
	void Texture::draw(Vec2 pos, bool facingRight);

	/**
	@brief Draw the Sprite
	@param pos Position to draw at
	@param rotation Rotation of the sprite in degrees (Around middle of sprite)
	*/
	void Texture::draw(Vec2 pos, float rotation);

	/**
	@brief Draw the Sprite
	@param pos Position to draw at
	@param rotation Rotation of the sprite in degrees around pivot point
	@param pivot Pivot point to rotate around
	*/
	void Texture::draw(Vec2 pos, float rotation, Vec2 pivot);

	/**
	@brief Load Image
	@param path Path to image
	@param renderer SDL_Renderer
	*/
	bool Texture::load(std::string path, SDL_Renderer* renderer);

	/**
	@brief Load Image
	@param surface A surface to convert into a texture (This is freed internally)
	@param renderer SDL_Renderer
	*/
	bool Texture::load(SDL_Surface* surface, SDL_Renderer* renderer);

	/**
	@brief Apply a tint to the texture (Warning this applied to all textures sharing if this is a pointer)
	@param colour Colour tint to apply
	*/
	void Texture::setColourTint(SDL_Colour colour);

	/**
	@brief Get the renderer used for this sprite
	@return SDL_Renderer* - The renderer used in the creation of this Texture
	*/
	SDL_Renderer* Texture::getRenderer();

	/**
	 @brief Gets raw texture.
	
	 @return null if it fails, else the raw texture.
	 */
	SDL_Texture* getRawTexture() { return texture; }

	/**
	 @brief Gets raw surface.
	
	 @return null if it fails, else the raw surface.
	 */
	SDL_Surface* getRawSurface() { return surface; }
private:
	/// Stored Dimensions of the Texture
	Vec2 dimensions;

	/// Texture data
	SDL_Texture* texture;

	/// Renderer
	SDL_Renderer* currentRenderer;

	/** @brief true to keep surface. */
	const bool keepSurface;

	/** @brief The surface. */
	SDL_Surface* surface;

	/**
	@brief Create a texture from a passed in surface 
	@param surface Image Surface
	@return bool - Whether it was successful
	*/
	bool Texture::createTexture(SDL_Surface& surface);
};

