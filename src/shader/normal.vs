#version 400 core

layout(location = 0) in vec3 VERTEX;
layout(location = 1) in vec3 aNORMAL;

uniform mat4 MODEL;

layout (std140) uniform Matrices
{
    mat4 PROJECTION;
    mat4 VIEW;
};

out vec3 NORMAL;

void main(){

	mat4 MVP = PROJECTION * VIEW * MODEL;
	gl_Position = MVP * vec4(VERTEX, 1);

	NORMAL = mat3(transpose(inverse(MODEL))) * aNORMAL;
}

