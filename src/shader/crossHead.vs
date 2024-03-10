#version 400 core

layout(location = 0) in vec3 VERTEX;

void main(){

	gl_Position = vec4(VERTEX, 1);
}

