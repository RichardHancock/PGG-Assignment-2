#pragma once

#include <SDL.h>
#include "glew.h"
#include <string>

class Shader
{
public:
	Shader(std::string vShaderFilename, std::string fShaderFilename);

private:
	///Shader Program
	GLuint program;
};