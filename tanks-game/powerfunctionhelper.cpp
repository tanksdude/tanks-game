#pragma once
#include "powerfunctionhelper.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "collisionhandler.h"
#include "constants.h"

bool PowerFunctionHelper::wallhackGenericBullet(Bullet*, Wall*) {
	return false; //anticlimactic
}

bool PowerFunctionHelper::wallhackGenericTank(Tank*, Wall*) {
	return false;
}

bool PowerFunctionHelper::bounceGeneric(Bullet* b, Wall* w) {
	if (!CollisionHandler::partiallyCollided(b, w)) {
		return false;
	}

	if (b->y - w->y <= (w->h / w->w) * (b->x - w->x)) { //I think this is bottom right
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //bottom
			b->y -= (b->y + b->r - w->y) * 2; //b->y = w->y - b->r
			b->angle = b->angle * -1;
		}
		else { //right
			b->x += (w->x + w->w - (b->x - b->r)) * 2; //b->x = w->x + w->w + w->r
			b->angle = PI - b->angle;
		}
	}
	else { //top left?
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //left
			b->x -= (b->x + b->r - w->x) * 2; //b->x = w->x - b->r
			b->angle = PI - b->angle;
		}
		else { //top
			b->y += (w->y + w->h - (b->y - b->r)) * 2; //b->y = w->y + w->h + b->r
			b->angle = b->angle * -1;
		}
	}
	//TODO: ensure bullet is not actually in wall; move bullet to edge of relevant wall

	return false; //this means the bullet bounced, not that it should be deleted //TODO: currently not true; make it true
}

bool PowerFunctionHelper::bounceGenericWithCorners(Bullet* b, Wall* w) { //not the default because bullets move too quickly on average

	return false;
}

bool PowerFunctionHelper::bounceEdgeGeneric(Bullet* b) {
	//TODO: ensure this works
	if (b->x + b->r > GAME_WIDTH) { //right edge
		b->x -= (b->x + b->r - GAME_WIDTH) * 2;
		b->angle = PI - b->angle;
		return true;
	} else if (b->x - b->r < 0){ //left edge
		b->x += -(b->x - b->r) * 2;
		b->angle = PI - b->angle;
		return true;
	}
	if (b->y + b->r > GAME_HEIGHT) { //top edge
		b->y += (GAME_HEIGHT - (b->y - b->r)) * 2;
		b->angle = b->angle * -1;
	} else if (b->y - b->r < 0) { //bottom edge
		b->y -= (b->y + b->r) * 2;
		b->angle = b->angle * -1;
	}

	return false;
}

void PowerFunctionHelper::equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>* cannonPoints, short num) {
	for (int i = cannonPoints->size() - 1; i >= 0; i--) {
		int end = (i + 1) % cannonPoints->size();
		double angle_diff;
		if (end == 0) {
			angle_diff = 2*PI - (cannonPoints->at(i).angle - cannonPoints->at(end).angle);
		} else {
			angle_diff = cannonPoints->at(end).angle - cannonPoints->at(i).angle;
		}

		for (int j = 0; j < num - 1; j++) {
			double newAngle = angle_diff/num * (j+1);
			CannonPoint temp = CannonPoint(newAngle + cannonPoints->at(i).angle);
			cannonPoints->insert(cannonPoints->begin() + i + j + 1, temp);

			if (end != 0) {
				end++;
			}
		}
	}
}

bool PowerFunctionHelper::homingGeneric(Bullet*, double) {
	//fix: deal with later
	return false;
}
