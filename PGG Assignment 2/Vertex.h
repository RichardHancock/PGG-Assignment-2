#pragma once

#include "Vec2.h"
#include "Vec3.h"

/** @brief A vertex. */
struct Vertex
{
	Vec3 v; //< Vertex Position
	Vec3 vn; //< Vertex Normal
	Vec2 vt; //< Vertex Texture Coordinate
};