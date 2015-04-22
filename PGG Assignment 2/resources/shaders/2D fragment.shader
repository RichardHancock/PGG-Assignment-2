#version 430 core

in vec2 vTexCoord;

uniform sampler2D gSampler;

out vec4 fragColour;

void main()
{
	//vec2 flipTexCoord = vec2(vTexCoord.x, 1.0 - vTexCoord.y);
	//fragColour = texture2D(gSampler, flipTexCoord);
	fragColour = texture2D(gSampler, vTexCoord);
}