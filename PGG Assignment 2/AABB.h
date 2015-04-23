#pragma once

#include "glm.hpp"

/** @brief A 3 dimensional Axis Aligned Bounding Box. */
class AABB
{
public:
	/**
	 @brief Constructor.
	
	 @param min The minimum.
	 @param max The maximum.
	 */
	AABB(glm::vec3 min, glm::vec3 max);

	/**
	 @brief Updates the AABB. Also applies the passed in transform. 
	
	 @param [in,out] modelMatrix The model matrix.
	 */
	void update(glm::mat4& modelMatrix);

	/**
	 @brief Checks for collision between this and another AABB.
	
	 @param [in,out] The other AABB.
	
	 @return true if it collides, false if none.
	 */
	bool collides(AABB* other);

	/**
	 @brief Gets the minimum position of the box.
	
	 @return The minimum position.
	 */
	glm::vec4 getMin() { return min; }

	/**
	 @brief Gets the maximum position of the box.
	
	 @return The maximum position.
	 */
	glm::vec4 getMax() { return max; }
private:
	///The initial passed in min max values
	glm::vec4 initMin, initMax;

	/** @brief The minimum position of the box. */
	glm::vec4 min;

	/** @brief The maximum position of the box. */
	glm::vec4 max;
};