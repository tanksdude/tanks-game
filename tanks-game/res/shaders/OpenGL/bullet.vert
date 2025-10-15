#version 330 core

layout(location = 0) in vec2 a_VertexPosition;
layout(location = 1) in vec4 a_ColorInstanced;
layout(location = 2) in mat4 a_ModelMatrixInstanced;

out vec4 v_Color;

uniform mat4 u_ViewMatrix; //camera is static so this is just an identity matrix
uniform mat4 u_ProjectionMatrix;
uniform int u_DrawTransparency;

#define VERTEX_COUNT 32

void main() {
	vec2 pos;
	float BulletLife = a_ColorInstanced.a;
	if (gl_VertexID < (VERTEX_COUNT+2)) {
		//sides of the death circle
		if (bool(u_DrawTransparency)) {
			pos = vec2(0.0, 0.0);
		} else {
			if (BulletLife >= 1.0 || BulletLife <= 0.0) {
				pos = vec2(0.0, 0.0);
			} else {
				//simple way:
				/*
				int deathTriangles = int(VERTEX_COUNT * BulletLife);
				if (gl_VertexID - 1 < deathTriangles) {
					pos = a_VertexPosition;
				} else {
					pos = vec2(0.0, 0.0);
				}
				*/

				//better way:
				float angle = radians(-360.0) * (1.0-BulletLife) * (float(gl_VertexID - 1) / VERTEX_COUNT); //also rotates the center vertex, not important
				pos = vec2(a_VertexPosition.x * cos(angle) - a_VertexPosition.y * sin(angle), a_VertexPosition.x * sin(angle) + a_VertexPosition.y * cos(angle));
			}
		}
		v_Color = vec4(1.0, 1.0, 1.0, 1.0);
	} else if (gl_VertexID < (VERTEX_COUNT+2) + (VERTEX_COUNT+1)) {
		//body
		pos = a_VertexPosition;
		v_Color = vec4(a_ColorInstanced.r, a_ColorInstanced.g, a_ColorInstanced.b, sqrt(BulletLife)); //no need to check u_DrawTransparency
	} else {
		//outline
		pos = a_VertexPosition;
		v_Color = vec4(0.0, 0.0, 0.0, 1.0);
	}

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * a_ModelMatrixInstanced * vec4(pos, 0.0, 1.0);
}
