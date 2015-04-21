#pragma once

#include "ResourceManager.h"
#include "GameModel.h"
#include <glm.hpp>

class Entity
{
public:

	/**
	 @brief Constructor.
	
	 @param initPosition	 The initial position of the Entity.
	 @param rotation		 The rotation.
	 @param scale			 The scale.
	 @param modelFilename    Filename of the model file.
	 @param textureFilename  Filename of the model's texture.
	 @param [in,out] manager Resource manager.
	 */

	Entity(glm::vec3 initPosition, glm::vec3 rotation , glm::vec3 scale, std::string modelFilename, 
		std::string textureFilename, ResourceManager* manager);

	~Entity();

	/**
	 @brief Updates the Entity.
	
	 @param dt The deltatime.
	 */
	virtual void update(float dt);

	/**
	@brief Draws object using the given camera view and projection matrices.

	@param [in,out] viewMatrix  The view matrix.
	@param [in,out] projMatrix  The projection matrix.
	@param [in,out] shader		 Shader used for rendering this model.
	*/
	void draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);

	glm::vec3 getPos() { return pos; }

	glm::vec3 getRotation() { return rotation; }

	glm::vec3 getScale() { return scale; }

protected:

	void rotateMat();

	/// Object position vector
	glm::vec3 pos;

	/// Euler angles for rotation
	glm::vec3 rotation;
	
	/** @brief The scale. */
	glm::vec3 scale;

	/// Object's model matrix
	/// This is rebuilt in the update function
	glm::mat4 modelMatrix;

	/** @brief The model used for this entity. */
	GameModel* model;
};