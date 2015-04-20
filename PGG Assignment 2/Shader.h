#pragma once

#include <SDL.h>
#include "glew.h"
#include <string>

/** 
@brief A GLSL shader program. 

Contains a compiled shader program and any uniform locations
*/
class Shader 
{
public:

	/**
	 @brief Constructor.
	
	 @param vShaderFilename Filename of the vertex shader.
	 @param fShaderFilename Filename of the fragment shader.
	 */
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

	/**
	 @brief Gets sampler location.
	 @return The sampler location.
	 */
	GLint getSamplerLoc() { return samplerLocation; }

private:
	///Shader Program
	GLuint program;

	/// Uniform locations
	GLint modelMatLocation, viewMatLocation, projMatLocation, samplerLocation;

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

	/**
	 @brief Initialises the shaders.
	Calls the load function to load the shaders and compile them into a program.
	 @param vShaderFilename Filename of the shader file.
	 @param fShaderFilename Filename of the shader file.
	 */
	void initialiseShaders(std::string vShaderFilename, std::string fShaderFilename);
};