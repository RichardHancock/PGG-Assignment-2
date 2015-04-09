#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Vertex.h"
#include "Vec2.h"
#include "Vec3.h"

namespace OBJLoader
{

	struct MultiIndicies
	{
		unsigned int v;
		unsigned int vn;
		unsigned int vt;
	};

	enum Type
	{
		V,
		VP,
		F,
		VN,
		VT,
		Ignored = -1, //< Comments etc...
		Unhandled = -2, //< Types that might get handled in the future (MTL format)
	};


	Type getType(std::string typeToConvert);

	void readFace(std::stringstream &line, std::vector<MultiIndicies> &storage);

	void readFloats(std::stringstream &line, std::vector<Vec3> &storage);

	void readFloats(std::stringstream &line, std::vector<Vec2> &storage);

	bool load(std::vector<Vertex> &vertexData, std::string filename);

	Vec3 calculateSurfaceNormal(Vec3 vertex);
}