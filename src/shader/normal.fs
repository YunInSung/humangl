#version 400 core

out vec4 color;

uniform vec4 COLOR;

void main(){
	// float light = max(dot(NORMAL, lightPos), 0.0);
	// float ambient = 0.1;

	// color = vec4(COLOR * (light + ambient), 1);
	color = COLOR;
}