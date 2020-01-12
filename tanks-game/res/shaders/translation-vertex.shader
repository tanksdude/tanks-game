#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVPM; /* Model View Projection Matrix */
uniform mat4 u_TM; /* Translation Matrix */

void main() {
	gl_Position = u_MVPM * u_TM * position; /* translate then project */
}