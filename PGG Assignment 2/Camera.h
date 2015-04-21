#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera();

	void updateViewMat(glm::vec3 targetPos);

	glm::mat4 getProjMatrix() { return projMatrix; }

	glm::mat4 getViewMatrix() { return viewMatrix; }

private:

	/// @brief The view matrix.
	glm::mat4 viewMatrix;

	glm::mat4 projMatrix;
};