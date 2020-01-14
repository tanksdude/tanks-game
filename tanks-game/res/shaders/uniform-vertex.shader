#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVPM; /* Model View Projection Matrix */

void main() {
	gl_Position = u_MVPM * position;
}