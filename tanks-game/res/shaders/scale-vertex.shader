#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_SM; /* Scaling Matrix */

void main() {
	gl_Position = u_SM * position; /* projection should have been calculated on CPU beforehand; scale then translate then project */
}