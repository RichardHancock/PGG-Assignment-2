#pragma once

#include "ResourceManager.h"
#include "GameModel.h"
#include <glm.hpp>
#include "AABB.h"

/**
@brief An entity that appears in the Game World.
 */
class Entity
{
public:

	/**
	 @brief Constructor.
	
	 @param initPosition	 The initial position of the Entity.
	 @param rotation		 The initial rotation.
	 @param scale			 The initial scale.
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
	
	 @param [in,out] viewMatrix The view matrix.
	 @param [in,out] projMatrix The projection matrix.
	 @param [in,out] shader	    Shader used for rendering this model.
	 */
	void draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader);

	/**
	 @brief Gets the position.
	
	 @return The position.
	 */
	glm::vec3 getPos() { return pos; }

	/**
	 @brief Sets the position.
	
	 @param newPos The new position.
	 */
	void setPos(glm::vec3 newPos) { pos = newPos; }

	/**
	 @brief Gets the rotation.
	
	 @return The rotation.
	 */
	glm::vec3 getRotation() { return rotation; }

	/**
	 @brief Sets the rotation.
	
	 @param newRot The new rot.
	 */
	void setRotation(glm::vec3 newRot) { rotation = newRot; }

	/**
	 @brief Gets the scale.
	
	 @return The scale.
	 */
	glm::vec3 getScale() { return scale; }

	/**
	 @brief Sets the scale.
	
	 @param newScale The new scale.
	 */
	void setScale(glm::vec3 newScale) { scale = newScale; }

	/**
	 @brief Gets the Axis Aligned Bounding Box.
	
	 @return The AABB, or Null if this Entity does not provide one.
	 */
	AABB* getAABB() { return aabb; }
protected:

	/** @brief Apply rotations to the model matrix. */
	void rotateMat();

	/** @brief Object position vector. */
	glm::vec3 pos;

	/** @brief Angles for rotation. (In Radians) */
	glm::vec3 rotation;

	/** @brief The scale. */
	glm::vec3 scale;

	/** @brief The Axis Aligned Bounding Box. */
	AABB* aabb;

	/** @brief Filename of the model file. */
	std::string modelFilename;

	/** @brief Resource Manager. */
	ResourceManager* resManager;

	/** @brief Object's model matrix This is rebuilt in the update function. */
	glm::mat4 modelMatrix;

	/** @brief The model. */
	GameModel* model;
};