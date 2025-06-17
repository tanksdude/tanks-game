#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_ColorInstanced;
layout(location = 2) in float a_BulletLifeInstanced;
layout(location = 3) in mat4 a_ModelMatrixInstanced;

out vec4 v_Color;

uniform mat4 u_ViewMatrix; //camera is static so this is just an identity matrix
uniform mat4 u_ProjectionMatrix;

#define VERTEX_COUNT 32

void main() {
	vec2 pos;
	if (gl_VertexID < (VERTEX_COUNT+2)) {
		//sides of the death circle
		if (a_BulletLifeInstanced >= 1.0 || a_BulletLifeInstanced <= 0.0) {
			pos = vec2(0.0, 0.0);
		} else {
			//simple way:
			/*
			int deathTriangles = int(VERTEX_COUNT * a_BulletLifeInstanced);
			if (gl_VertexID - 1 < deathTriangles) {
				pos = a_Position;
			} else {
				pos = vec2(0.0, 0.0);
			}
			*/

			//better way:
			float angle = radians(-360.0) * (1.0-a_BulletLifeInstanced) * (float(gl_VertexID - 1) / VERTEX_COUNT); //also rotates the center vertex, not important
			pos = vec2(a_Position.x * cos(angle) - a_Position.y * sin(angle), a_Position.x * sin(angle) + a_Position.y * cos(angle));
		}
		v_Color = vec4(1.0, 1.0, 1.0, 1.0);
	} else if (gl_VertexID < (VERTEX_COUNT+2) + (VERTEX_COUNT+1)) {
		//body
		pos = a_Position;
		v_Color = a_ColorInstanced;
	} else {
		//outline
		pos = a_Position;
		v_Color = vec4(0.0, 0.0, 0.0, 1.0);
	}

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * a_ModelMatrixInstanced * vec4(pos, 0.0, 1.0);
}
