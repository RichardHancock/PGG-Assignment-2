
#include "GameModel.h"

#include <iostream>
#include <fstream>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include "OBJLoader.h"
#include "Vertex.h"


// This does not belong here - should really have a nice shader class etc for sorting all this stuff out!
// Useful little function to just check for compiler errors
bool CheckShaderCompiled( GLint shader )
{
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;

		return false;
	}
	return true;
}








GameModel::GameModel()
{
	// Initialise variables
	_VAO = 0;
	_program = 0;
	_shaderModelMatLocation = _shaderViewMatLocation = _shaderProjMatLocation = 0;
	_numVertices = 0;

	// Create the model
	InitialiseVAO();
	// Create the shaders
	InitialiseShaders();
}

GameModel::~GameModel()
{
	// TODO: destroy VAO, shaders etc
}

void GameModel::InitialiseVAO()
{
		// Creates one VAO
	glGenVertexArrays( 1, &_VAO );
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray( _VAO );

	std::vector<Vertex> tempVertexData;

	OBJLoader::load(tempVertexData,"models/testTri.obj");

	const int vertexCount = tempVertexData.size() * 8;
	_numVertices = vertexCount / 8;
	GLfloat* vertexData = new GLfloat[vertexCount];

	for (unsigned int i = 0; i < tempVertexData.size(); i++)
	{
		unsigned int aPos = i * 8; // Position in the GLfloat array

		vertexData[aPos] = tempVertexData[i].v.x;
		vertexData[aPos + 1] = tempVertexData[i].v.y;
		vertexData[aPos + 2] = tempVertexData[i].v.z;

		vertexData[aPos + 3] = tempVertexData[i].vn.x;
		vertexData[aPos + 4] = tempVertexData[i].vn.y;
		vertexData[aPos + 5] = tempVertexData[i].vn.z;

		vertexData[aPos + 6] = tempVertexData[i].vt.x;
		vertexData[aPos + 7] = tempVertexData[i].vt.y;
		
	}
	//&vector[0]
	//const int verticeCount = tempVertexData.size() * 3;
	//const int normalCount = verticeCount;

	// Variable for storing a VBO
	GLuint vertexBuffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &vertexBuffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertexData, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	// (We will look at this properly in the lectures)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, v)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vn)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vt)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind for neatness, it just makes life easier
	// As a general tip, especially as you're still learning, for each function that needs to do something specific try to return OpenGL in the state you found it in
	// This means you will need to set the states at the beginning of your function and set them back at the end
	// If you don't do this, your function could rely on states being set elsewhere and it's easy to lose track of this as your project grows
	// If you then change the code from elsewhere, your current code could mysteriously stop working properly!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
	
	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	//glDisableVertexAttribArray(0);

}

void GameModel::InitialiseShaders()
{
	// OpenGL doesn't provide any functions for loading shaders from file
	// I'm feeling lazy, so just hardcoding them here
	/*
	// This is the vertex shader
	const GLchar *vShaderText = "#version 430 core\n\
						layout(location = 0) in vec4 vPosition;\n\
						layout(location = 1) in vec3 vNormalIn;\n\
						layout(location = 2) in vec2 vTexture;\n\
						 \n\
						 uniform mat4 modelMat;\n\
						 uniform mat4 invModelMat;\n\
						 uniform mat4 viewMat;\n\
						 uniform mat4 projMat;\n\
						 \n\
						 uniform vec4 worldSpaceLightPos = {1,0.8,1,1};\n\
						 \n\
						 out vec3 vNormalV;\n\
						 out vec3 lightDirV;\n\
						 \n\
						 void main()\n\
						 {\n\
								gl_Position = projMat * viewMat * modelMat * vPosition;\n\
								\n\
								vec4 eyeSpaceVertPos = viewMat * modelMat * vPosition;\n\
								vec4 eyeSpaceLightPos = viewMat * worldSpaceLightPos;\n\
								\n\
								lightDirV =  normalize( vec3(eyeSpaceLightPos) - vec3(eyeSpaceVertPos) );\n\
								\n\
								vNormalV = mat3(viewMat * modelMat) * vNormalIn;\n\
						 }";

	// This is the fragment shader
	const GLchar *fShaderText = "#version 430 core\n\
								in vec3 lightDirV;\n\
								in vec3 vNormalV;\n\
								\n\
								uniform vec3 lightColour = {1,1,1};\n\
								uniform vec3 emissiveColour = {0,0,0};\n\
								uniform vec3 ambientColour  = {0.3f,0.3f,0.6f};\n\
								uniform vec3 diffuseColour  = {0.8f,0.1f,0.1f};\n\
								uniform vec3 specularColour = {0.0f,0.0f,0.0f};\n\
								uniform float shininess     = 70.0f;\n\
								uniform float alpha         = 1.0f;\n\
								\n\
								out vec4 fragColour;\n\
								\n\
								void main()\n\
								{\n\
									vec3 lightDir = normalize( lightDirV );\n\
									vec3 vNormal = normalize( vNormalV );\n\
									\n\
										vec3 diffuse = diffuseColour * lightColour * max( dot( vNormal, lightDir ), 0);\n\
										\n\
										fragColour = vec4( emissiveColour + ambientColour + diffuse, alpha);\n\
								}";
						 // 
						 /*
									vec3 ambient = vec3();
									vec3 diffuse = vec3(1.0f,0.3f,0.3f) * max(dot( normalize(lightDir), normalize(vNormal) ),0);
									fColor = vec4( ambient + diffuse, 1.0f);
									*/
	std::string vShaderRaw = loadShaderFromFile("shaders/vertex.shader");
	std::string fShaderRaw = loadShaderFromFile("shaders/fragment.shader");
	const char* vShaderText = vShaderRaw.c_str();
	const char* fShaderText = fShaderRaw.c_str();

	// The 'program' stores the shaders
	_program = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	// Give GL the source for it
	glShaderSource( vShader, 1, &vShaderText, NULL );
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( !CheckShaderCompiled( vShader ) )
	{
		return;
	}
	// This links the shader to the program
	glAttachShader( _program, vShader );

	// Same for the fragment shader
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		return ;
	}
	glAttachShader( _program, fShader );

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( _program );
	// Check this worked
	GLint linked;
	glGetProgramiv( _program, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _program, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return;
	}

	// We need to get the location of the uniforms in the shaders
	// This is so that we can send the values to them from the application
	// We do this in the following way: 
	_shaderModelMatLocation = glGetUniformLocation( _program, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _program, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _program, "projMat" );

}


void GameModel::Update( float deltaTs )
{
	// update the rotation angle of our cube
	_rotation.y += deltaTs * 0.5f;
	while( _rotation.y > (3.14159265358979323846 * 2.0) )
	{
		_rotation.y -= (float)(3.14159265358979323846 * 2.0);
	}

	// Build the model matrix!
	// First we start with an identity matrix
	// This is created with the command: glm::mat4(1.0f)
	// Next, we translate this matrix according to the object's _position vector:
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position );
	_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
	// Next, we rotate this matrix in the y-axis by the object's y-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0,1,0) );
	// And there we go, model matrix is ready!

}

void GameModel::Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix)
{
	// Ok, here I like to indent drawing calls - it's just a personal style, you may not like it and that's fine ;)
	// Generally you will need to be activating and deactivating OpenGL states
	// I just find it visually easier if the activations / deactivations happen at different tab depths
	// This can help when things get more complex

	// Activate the shader program
	glUseProgram( _program );

		// Activate the VAO
		glBindVertexArray( _VAO );

			// Send matrices to the shader as uniforms like this:
			glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrix) );
			glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix) );
			glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix) );


			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, _numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
	
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}

std::string GameModel::loadShaderFromFile(char* path)
{
	std::string shader;
	std::ifstream file(path, std::ios::in);

	if (!file.is_open())
	{
		std::cout << "Error: Shader couldn't be opened: " << path << std::endl;
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
	//const char* convertedShader = shader.c_str();
	return shader;

}