#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

//uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix; //camera is static so this is just an identity matrix
uniform mat4 u_ProjectionMatrix;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 0.0, 1.0);
	v_Color = a_Color;
}
