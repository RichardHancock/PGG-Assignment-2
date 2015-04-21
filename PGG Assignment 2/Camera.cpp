#include "Camera.h"
#include "gtc/matrix_transform.hpp"
#include "Utility.h"

Camera::Camera()
{
	// Create a viewing matrix for the camera
	viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, -1.0f, -2.5f));
	viewMatrix = glm::rotate(viewMatrix, Utility::HALF_PI, glm::vec3(0, 1, 0));

	// Construct a projection matrix for the camera
	projMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

void Camera::updateViewMat(glm::vec3 targetPos)
{
	glm::vec3 upDir(0, 1, 0);
	glm::vec3 cameraPos = targetPos;
	cameraPos.y += 1.25f;
	cameraPos.z += 2.0f;
	
	targetPos.z -= 2.0f;

	viewMatrix = glm::lookAt(cameraPos, targetPos, upDir);

	//viewMatrix = glm::rotate(viewMatrix, Utility::HALF_PI, glm::vec3(0, 1, 0));
}
