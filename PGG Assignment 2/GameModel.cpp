
#include "GameModel.h"

#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include "OBJLoader.h"
#include "Vertex.h"
#include "Utility.h"

GameModel::GameModel(std::string modelFilename, std::string textureFilename, ResourceManager* manager)
{
	// Initialise variables
	VAO = 0;
	textureID = 0;
	numVertices = 0;
	disableMatUniforms = false;
	this->textureFilename = textureFilename;
	resManager = manager;
	
	Utility::log(Utility::I, "Loading model: " + modelFilename);

	texture = manager->getTexture(textureFilename);

	// Create the model
	initialiseVAO(modelFilename);
}

GameModel::GameModel(std::vector<Vertex> vertexData, std::string textureFilename, ResourceManager* manager)
{
	// Initialise variables
	VAO = 0;
	textureID = 0;
	numVertices = 0;
	disableMatUniforms = true;
	this->textureFilename = textureFilename;
	resManager = manager;

	texture = manager->getTexture(textureFilename);

	// Create the model
	initialiseVAO(vertexData);
}


GameModel::GameModel(std::vector<Vertex> vertexData, std::string text, TTF_Font* font, ResourceManager* manager)
{
	// Initialise variables
	VAO = 0;
	textureID = 0;
	numVertices = 0;
	disableMatUniforms = true;
	resManager = manager;

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);

	// Create the model
	initialiseVAO(vertexData, surface);
	SDL_FreeSurface(surface);
}

GameModel::~GameModel()
{
	//Just incase the below VAO deletion doesn't cover it, delete all other data
	glDeleteBuffers(1, &vertexBuffer);

	glDeleteTextures(1, &textureID);

	//I believe this will delete all associated data, as once a VBO/Texture has nothing referencing it, it will be deleted
	glDeleteVertexArrays(1, &VAO);

	if (texture != nullptr)
	{
		resManager->freeResourceInstance(textureFilename, ResourceManager::TextureFile);
	}
}

void GameModel::initialiseVAO(std::string modelFilename)
{
		// Creates one VAO
	glGenVertexArrays( 1, &VAO );
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray( VAO );

	std::vector<Vertex> tempVertexData;

	OBJLoader::load(tempVertexData, modelFilename);

	const int vertexCount = tempVertexData.size() * 8;
	numVertices = vertexCount / 8;
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

	// Variable for storing a VBO
	vertexBuffer = 0;
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

	GLenum textureFormat;

	SDL_Surface* surface = texture->getRawSurface();

	//Section from http://ubuntuforums.org/archive/index.php/t-1088836.html post 2
	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}
	//End Section

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		textureFormat,
		texture->getRawSurface()->w,
		texture->getRawSurface()->h,
		0,
		textureFormat,
		GL_UNSIGNED_BYTE,
		texture->getRawSurface()->pixels
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

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

void GameModel::initialiseVAO(std::vector<Vertex> vertexData)
{
	// Creates one VAO
	glGenVertexArrays(1, &VAO);
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray(VAO);

	const int vertexCount = vertexData.size() * 8;
	numVertices = vertexCount / 8;
	GLfloat* finalVertexData = new GLfloat[vertexCount];

	for (unsigned int i = 0; i < vertexData.size(); i++)
	{
		unsigned int aPos = i * 8; // Position in the GLfloat array

		finalVertexData[aPos] = vertexData[i].v.x;
		finalVertexData[aPos + 1] = vertexData[i].v.y;
		finalVertexData[aPos + 2] = vertexData[i].v.z;
		
		finalVertexData[aPos + 3] = vertexData[i].vn.x;
		finalVertexData[aPos + 4] = vertexData[i].vn.y;
		finalVertexData[aPos + 5] = vertexData[i].vn.z;
		
		finalVertexData[aPos + 6] = vertexData[i].vt.x;
		finalVertexData[aPos + 7] = vertexData[i].vt.y;

	}

	// Variable for storing a VBO
	vertexBuffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &vertexBuffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, finalVertexData, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	// (We will look at this properly in the lectures)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, v)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vn)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vt)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLenum textureFormat;

	SDL_Surface* surface = texture->getRawSurface();

	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		textureFormat,
		texture->getRawSurface()->w,
		texture->getRawSurface()->h,
		0,
		textureFormat,
		GL_UNSIGNED_BYTE,
		texture->getRawSurface()->pixels
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind for neatness, it just makes life easier
	// As a general tip, especially as you're still learning, for each function that needs to do something specific try to return OpenGL in the state you found it in
	// This means you will need to set the states at the beginning of your function and set them back at the end
	// If you don't do this, your function could rely on states being set elsewhere and it's easy to lose track of this as your project grows
	// If you then change the code from elsewhere, your current code could mysteriously stop working properly!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	//glDisableVertexAttribArray(0);

}

void GameModel::initialiseVAO(std::vector<Vertex> vertexData, SDL_Surface* surface)
{
	// Creates one VAO
	glGenVertexArrays(1, &VAO);
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray(VAO);

	const int vertexCount = vertexData.size() * 8;
	numVertices = vertexCount / 8;
	GLfloat* finalVertexData = new GLfloat[vertexCount];

	for (unsigned int i = 0; i < vertexData.size(); i++)
	{
		unsigned int aPos = i * 8; // Position in the GLfloat array

		finalVertexData[aPos] = vertexData[i].v.x;
		finalVertexData[aPos + 1] = vertexData[i].v.y;
		finalVertexData[aPos + 2] = vertexData[i].v.z;

		finalVertexData[aPos + 3] = vertexData[i].vn.x;
		finalVertexData[aPos + 4] = vertexData[i].vn.y;
		finalVertexData[aPos + 5] = vertexData[i].vn.z;

		finalVertexData[aPos + 6] = vertexData[i].vt.x;
		finalVertexData[aPos + 7] = vertexData[i].vt.y;

	}

	// Variable for storing a VBO
	vertexBuffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &vertexBuffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, finalVertexData, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	// (We will look at this properly in the lectures)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, v)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vn)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vt)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLenum textureFormat;

	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		textureFormat,
		surface->w,
		surface->h,
		0,
		textureFormat,
		GL_UNSIGNED_BYTE,
		surface->pixels
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind for neatness, it just makes life easier
	// As a general tip, especially as you're still learning, for each function that needs to do something specific try to return OpenGL in the state you found it in
	// This means you will need to set the states at the beginning of your function and set them back at the end
	// If you don't do this, your function could rely on states being set elsewhere and it's easy to lose track of this as your project grows
	// If you then change the code from elsewhere, your current code could mysteriously stop working properly!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	//glDisableVertexAttribArray(0);

}

void GameModel::draw(glm::mat4& modelMatrix, glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader)
{
	// Ok, here I like to indent drawing calls - it's just a personal style, you may not like it and that's fine ;)
	// Generally you will need to be activating and deactivating OpenGL states
	// I just find it visually easier if the activations / deactivations happen at different tab depths
	// This can help when things get more complex

	// Activate the shader program
	glUseProgram(shader->getProgram());

		// Activate the VAO
		glBindVertexArray( VAO );

			if (!disableMatUniforms)
			{
				// Send matrices to the shader as uniforms like this:
				glUniformMatrix4fv(shader->getModelMatLoc(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
				glUniformMatrix4fv(shader->getViewMatLoc(), 1, GL_FALSE, glm::value_ptr(viewMatrix));
				glUniformMatrix4fv(shader->getProjMatLoc(), 1, GL_FALSE, glm::value_ptr(projMatrix));
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			//GLuint uniform_texture = glGetUniformLocation(shader->getProgram(), "textureInput");
			glUniform1i(shader->getSamplerLoc(), 0);
			
			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
	
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}