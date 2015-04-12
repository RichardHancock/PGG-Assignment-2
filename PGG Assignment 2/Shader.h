#pragma once

#include <SDL.h>
#include "glew.h"
#include <string>

class Shader 
{
public:
	Shader(std::string vShaderFilename, std::string fShaderFilename);
	
	/**
	@brief Get the shader program
	@return GLuint - Shader Program
	*/
	GLuint getProgram() { return program; }

	/**
	@brief Get the location of Model Matrix in the shader
	@return GLint - Shader Location
	*/
	GLint getModelMatLoc() { return modelMatLocation; }

	/**
	@brief Get the location of View Matrix in the shader
	@return GLint - Shader Location
	*/
	GLint getViewMatLoc() { return viewMatLocation; }

	/**
	@brief Get the location of Projection Matrix in the shader
	@return GLint - Shader Location
	*/
	GLint getProjMatLoc() { return projMatLocation; }

private:
	///Shader Program
	GLuint program;

	/// Uniform locations
	GLint modelMatLocation, viewMatLocation, projMatLocation;

	/**
	@brief Loads a shader from a file and returns its contents
	@param std::string - Path to the shader
	@return std::string - Shader file contents
	*/
	std::string loadShaderFromFile(std::string path);
	
	/**
	@brief Checks for any errors in the loaded shader/compile errors
	@param GLint - Shader to check
	@return bool - Compiled Successfully
	*/
	bool checkShaderCompiled(GLint shader);

	void initialiseShaders(std::string vShaderFilename, std::string fShaderFilename);
};