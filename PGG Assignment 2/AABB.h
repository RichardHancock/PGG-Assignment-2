#pragma once

#include "glm.hpp"

class AABB
{
public:

	AABB(glm::vec3 min, glm::vec3 max);

	//void scaleAABB(glm::vec3 scale);

	void update(glm::mat4& modelMatrix);

	bool collides(AABB* other);

	glm::vec4 getMin() { return min; }

	glm::vec4 getMax() { return max; }
private:
	///The initial passed in min max values
	glm::vec4 initMin, initMax;

	glm::vec4 min;

	glm::vec4 max;
};