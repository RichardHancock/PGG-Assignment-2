#version 430 core
						
layout(location = 0) in vec4 vPosition;
layout(location = 2) in vec2 vTexture;

out vec2 vTexCoord;

void main()
{
	gl_Position = vPosition;
	
	vTexCoord = vTexture;
}