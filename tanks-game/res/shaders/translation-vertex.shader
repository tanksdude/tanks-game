#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_TM; /* Translation Matrix */

void main() {
	gl_Position = u_TM * position; /* projection should have been calculated on CPU beforehand; translate then project */
}