#version 400 core

layout(location = 0) in vec3 VERTEX;
layout(location = 1) in vec4 JOINT1;
layout(location = 2) in vec4 JOINT2;
layout(location = 3) in vec4 JOINT3;
layout(location = 4) in vec4 JOINT4;

layout (std140) uniform Matrices
{
    mat4 PROJECTION;
    mat4 VIEW;
};

uniform mat4 MODEL;

void main(){
	mat4 MVP = PROJECTION * VIEW * MODEL;
	mat4 JOINT = mat4(JOINT1, JOINT2, JOINT3, JOINT4);
    gl_Position = MVP * JOINT * vec4(VERTEX, 1);
}

