#version 330 core

layout(location = 0) in vec2 a_Position;

void main() {
	gl_Position = a_Position;
}
