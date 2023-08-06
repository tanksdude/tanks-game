#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color; //TODO: replace with color in the vertex

void main() {
	color = u_color;
}
