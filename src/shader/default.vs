#version 400 core

layout(location = 0) in vec2 VERTEX;
layout(location = 1) in vec2 TEXCORa;
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;
// layout (location = 3) in vec3 aTangent;
// layout (location = 4) in vec3 aBitangent;
// layout (location = 5) in vec3 aColor;

out vec2 TEXCOR;

void main() {
	gl_Position = vec4(VERTEX, 0, 1);
	TEXCOR = TEXCORa;
	// gl_Position = vec4(aPos, 1);
	// TEXCOR = aTexCoords;
}