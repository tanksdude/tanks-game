#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix; //camera is static so it's just an identity matrix
uniform mat4 u_ProjectionMatrix;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * position;
}
