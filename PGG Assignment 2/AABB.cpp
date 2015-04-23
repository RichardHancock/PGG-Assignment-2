#include "AABB.h"
#include "gtc/matrix_transform.hpp"
#include "Utility.h"

AABB::AABB(glm::vec3 min, glm::vec3 max)
{
	initMin = glm::vec4(min.x, min.y, min.z, 1.0f);
	initMax = glm::vec4(max.x, max.y, max.z, 1.0f);
}

void AABB::update(glm::mat4& modelMatrix)
{
	//cant handle rotation transforms
	min = modelMatrix * initMin;
	max = modelMatrix * initMax;
}

bool AABB::collides(AABB* other)
{
	//Function adapted from: http://www.miguelcasillas.com/?p=30

	return(
		//X
		max.x > other->getMin().x &&
		min.x < other->getMax().x &&
		//Y
		max.y > other->getMin().y &&
		min.y < other->getMax().y &&
		//Z 
		max.z > other->getMin().z &&
		min.z < other->getMax().z
		);
}
