#include "Shader.h"
#include "Utility.h"
#include <fstream>

Shader::Shader(std::string vShaderFilename, std::string fShaderFilename)
{
	initialiseShaders(vShaderFilename, fShaderFilename);
}

void Shader::initialiseShaders(std::string vShaderFilename, std::string fShaderFilename)
{	
	std::string vShaderRaw = loadShaderFromFile(vShaderFilename);
	std::string fShaderRaw = loadShaderFromFile(fShaderFilename);
	const char* vShaderText = vShaderRaw.c_str();
	const char* fShaderText = fShaderRaw.c_str();

	// The 'program' stores the shaders
	program = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Give GL the source for it
	glShaderSource(vShader, 1, &vShaderText, NULL);
	// Compile the shader
	glCompileShader(vShader);
	// Check it compiled and give useful output if it didn't work!
	if (!checkShaderCompiled(vShader))
	{
		return;
	}
	// This links the shader to the program
	glAttachShader(program, vShader);

	// Same for the fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderText, NULL);
	glCompileShader(fShader);
	if (!checkShaderCompiled(fShader))
	{
		return;
	}
	glAttachShader(program, fShader);

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram(program);
	// Check this worked
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		Utility::log(Utility::E, "Shader linking failed: " + std::string(log));
		//std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete[] log;

		return;
	}

	// We need to get the location of the uniforms in the shaders
	// This is so that we can send the values to them from the application
	// We do this in the following way: 
	modelMatLocation = glGetUniformLocation(program, "modelMat");
	viewMatLocation = glGetUniformLocation(program, "viewMat");
	projMatLocation = glGetUniformLocation(program, "projMat");

}

bool Shader::checkShaderCompiled(GLint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);
		
		Utility::log(Utility::E, "Shader compilation failed: " + std::string(log));
		//std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete[] log;

		return false;
	}
	return true;
}

std::string Shader::loadShaderFromFile(std::string path)
{
	std::string shader;
	std::ifstream file(path, std::ios::in);

	if (!file.is_open())
	{
		Utility::log(Utility::E, "Shader couldn't be opened: " + std::string(path));
		return "";
	}

	std::string line;
	while (!file.eof())
	{
		//file.getline(line,(std::streamsize) 2000);
		std::getline(file, line);
		line.append("\n");
		shader.append(line);
	}
	file.close();

	return shader;

}