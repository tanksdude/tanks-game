#version 330 core

layout(location = 0) in vec2 a_VertexPosition;
layout(location = 1) in vec3 a_BulletValuesInstanced;
layout(location = 2) in vec4 a_ColorInstanced;

out vec4 v_Color;

uniform mat4 u_ViewMatrix; //camera is static so this is just an identity matrix
uniform mat4 u_ProjectionMatrix;
uniform int u_DrawTransparency;

#define VERTEX_COUNT 32
#define OUTLINE_WIDTH                   0.5
#define DEATH_CIRCLE_RADIUS_INCREASE    2.0
#define DEATH_CIRCLE_RADIUS_MULTIPLIER (9.0 / 8.0)

void main() {
	float BulletLife = a_ColorInstanced.a;
	float BulletX = a_BulletValuesInstanced[0];
	float BulletY = a_BulletValuesInstanced[1];
	float BulletR = a_BulletValuesInstanced[2];
	vec2 pos;

	if (gl_VertexID < (VERTEX_COUNT+2)) {
		//sides of the death circle
		if (bool(u_DrawTransparency)) {
			pos = vec2(BulletX, BulletY);
		} else {
			if (BulletLife >= 1.0 || BulletLife <= 0.0) {
				pos = vec2(BulletX, BulletY);
			} else {
				//simple way:
				/*
				int deathTriangles = int(VERTEX_COUNT * BulletLife);
				if (gl_VertexID - 1 < deathTriangles) {
					pos = vec2(BulletX + ((BulletR + DEATH_CIRCLE_RADIUS_INCREASE) * DEATH_CIRCLE_RADIUS_MULTIPLIER) * a_VertexPosition.x,
					           BulletY + ((BulletR + DEATH_CIRCLE_RADIUS_INCREASE) * DEATH_CIRCLE_RADIUS_MULTIPLIER) * a_VertexPosition.y);
				} else {
					pos = vec2(BulletX, BulletY);
				}
				*/

				//better way:
				float angle = radians(-360.0) * (1.0-BulletLife) * (float(gl_VertexID - 1) / VERTEX_COUNT); //also rotates the center vertex, not important
				pos = vec2(BulletX + ((BulletR + DEATH_CIRCLE_RADIUS_INCREASE) * DEATH_CIRCLE_RADIUS_MULTIPLIER) * (a_VertexPosition.x * cos(angle) - a_VertexPosition.y * sin(angle)),
				           BulletY + ((BulletR + DEATH_CIRCLE_RADIUS_INCREASE) * DEATH_CIRCLE_RADIUS_MULTIPLIER) * (a_VertexPosition.x * sin(angle) + a_VertexPosition.y * cos(angle)));
			}
		}
		v_Color = vec4(1.0, 1.0, 1.0, 1.0);
	} else if (gl_VertexID < (VERTEX_COUNT+2) + (VERTEX_COUNT+1)) {
		//body
		pos = vec2(BulletX + BulletR * a_VertexPosition.x,
		           BulletY + BulletR * a_VertexPosition.y);
		v_Color = vec4(a_ColorInstanced.r, a_ColorInstanced.g, a_ColorInstanced.b, sqrt(BulletLife)); //no need to check u_DrawTransparency
		//if you need to save some compute: sqrt(BulletLife) looks similar to (1.0-(1.0-BulletLife)**2)
	} else {
		//outline
		if (mod(gl_VertexID - ((VERTEX_COUNT+2) + (VERTEX_COUNT+1)), 2) == 0) { //% is "reserved"
			pos = vec2(BulletX + (BulletR - OUTLINE_WIDTH) * a_VertexPosition.x,
			           BulletY + (BulletR - OUTLINE_WIDTH) * a_VertexPosition.y);
		} else {
			pos = vec2(BulletX + (BulletR + OUTLINE_WIDTH) * a_VertexPosition.x,
			           BulletY + (BulletR + OUTLINE_WIDTH) * a_VertexPosition.y);
		}
		v_Color = vec4(0.0, 0.0, 0.0, 1.0);
	}

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(pos, 0.0, 1.0);
}
