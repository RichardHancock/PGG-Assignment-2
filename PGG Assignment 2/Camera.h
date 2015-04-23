#pragma once

#include <glm.hpp>

/** @brief A camera that holds and manipulates the View and Projection Matrices. */
class Camera
{
public:
	/** @brief Default constructor. */
	Camera();

	/**
	 @brief Updates the view matrix to look at targetPos.
	
	 @param targetPos Target position.
	 */
	void updateViewMat(glm::vec3 targetPos);

	/**
	 @brief Gets projection matrix.
	
	 @return The projection matrix.
	 */
	glm::mat4 getProjMatrix() { return projMatrix; }

	/**
	 @brief Gets view matrix.
	
	 @return The view matrix.
	 */
	glm::mat4 getViewMatrix() { return viewMatrix; }

private:

	/// @brief The view matrix.
	glm::mat4 viewMatrix;

	/** @brief The project matrix. */
	glm::mat4 projMatrix;
};