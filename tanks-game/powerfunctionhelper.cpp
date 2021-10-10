#include "powerfunctionhelper.h"
#include "collisionhandler.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "tankmanager.h"
#include <iostream>

bool PowerFunctionHelper::bounceGeneric(Bullet* b, Wall* w) {
	if (!CollisionHandler::partiallyCollided(b, w)) {
		return false;
	}

	if (b->y - w->y <= (w->h / w->w) * (b->x - w->x)) { //I think this is bottom right
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //bottom
			b->y -= (b->y + b->r - w->y) * 2; //b->y = w->y - b->r
			b->velocity.setAngle(b->velocity.getAngle() * -1);
		} else { //right
			b->x += (w->x + w->w - (b->x - b->r)) * 2; //b->x = w->x + w->w + w->r
			b->velocity.setAngle(PI - b->velocity.getAngle());
		}
	} else { //top left?
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //left
			b->x -= (b->x + b->r - w->x) * 2; //b->x = w->x - b->r
			b->velocity.setAngle(PI - b->velocity.getAngle());
		} else { //top
			b->y += (w->y + w->h - (b->y - b->r)) * 2; //b->y = w->y + w->h + b->r
			b->velocity.setAngle(b->velocity.getAngle() * -1);
		}
	}
	//TODO: ensure bullet is not actually in wall; move bullet to edge of relevant wall if still colliding

	return true; //this means the bullet bounced, not that it should be deleted
}

bool PowerFunctionHelper::bounceGenericWithCorners(Bullet* b, Wall* w) { //not the default because bullets move too quickly on average
	if (!CollisionHandler::partiallyCollided(b, w)) {
		return false;
	}

	if ((b->x < w->x) && (b->y < w->y)) { //circle in bottom left
		return bounceGenericWithCornersCornerHandler(b, w->x, w->y);
	}
	//can't do elses because more than one corner could be in the circle (TODO: really?)
	if ((b->x > (w->x + w->w)) && (b->y < w->y)) { //circle in bottom right
		return bounceGenericWithCornersCornerHandler(b, (w->x + w->w), w->y);
	}
	if ((b->x < w->x) && (b->y > (w->y + w->h))) { //circle in top left
		return bounceGenericWithCornersCornerHandler(b, w->x, (w->y + w->h));
	}
	if ((b->x > (w->x + w->w)) && (b->y > (w->y + w->h))) { //circle in top right
		return bounceGenericWithCornersCornerHandler(b, (w->x + w->w), (w->y + w->h));
	}

	return PowerFunctionHelper::bounceGeneric(b, w);
	//return false;
}

bool PowerFunctionHelper::bounceGenericWithCornersCornerHandler(Bullet* b, double x, double y) {
	//I have no idea if this is correct but it behaves exactly as I want it to, I think
	//it's not exact because intersection points aren't calculated but it's close enough

	if ((abs(x - b->x) <= b->r) && (abs(y - b->y) <= b->r)) {
		double d = sqrt(pow(x - b->x, 2) + pow(y - b->y, 2));
		if (d <= b->r) {
			double angle = atan2((y - b->y), (x - b->x));
			b->y -= sin(angle) * (b->r - d);
			b->x -= cos(angle) * (b->r - d);

			//so a rectangle has an area of influence against a circle: outer edges + radius, and corners are radius (picture a rounded rectangle)
			//when a bullet's center enters the area, it is inside the rectangle, and therefore needs to reflect
			//in effect, rounded rectangle against a point is the same as rectangle against circle
			//the bullet's angle needs to reflect off of the perpendicular to the tangent, and the tangent goes through the intersection between the bullet's path and the area of influence

			double newAngle = 2*angle - (b->velocity.getAngle() - PI);
			b->y += sin(newAngle) * (b->r - d);
			b->x += cos(newAngle) * (b->r - d);
			b->velocity.setAngle(newAngle);

			return true;
		}
	}

	return false;
}

bool PowerFunctionHelper::bounceEdgeGenericY(Bullet* b) {
	bool bounced = 0;
	if (b->y + b->r > GAME_HEIGHT) { //top edge
		b->y -= ((b->y + b->r) - GAME_HEIGHT) * 2;
		b->velocity.setAngle(b->velocity.getAngle() * -1);
		bounced = true;
	} else if (b->y - b->r < 0) { //bottom edge
		b->y += -(b->y - b->r) * 2;
		b->velocity.setAngle(b->velocity.getAngle() * -1);
		bounced = true;
	}

	return bounced;
}

bool PowerFunctionHelper::bounceEdgeGenericX(Bullet* b) {
	bool bounced = 0;
	if (b->x + b->r > GAME_WIDTH) { //right edge
		b->x -= (b->x + b->r - GAME_WIDTH) * 2;
		b->velocity.setAngle(PI - b->velocity.getAngle());
		bounced = true;
	} else if (b->x - b->r < 0) { //left edge
		b->x += -(b->x - b->r) * 2;
		b->velocity.setAngle(PI - b->velocity.getAngle());
		bounced = true;
	}

	return bounced;
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

bool PowerFunctionHelper::homingGeneric(Bullet* b, double maxAngleMove, bool moveByAngle) { //moveByAngle = target based on angle differences, not distance
	//TODO: when team mode is a thing (or single-player campaign?), this will need an update
	char targetTank;

	if (moveByAngle) {
		double* angleDiffs = new double[TankManager::getNumTanks()];
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			Tank* t = TankManager::getTank(i);
			if (t->getTeamID() == b->getTeamID()) {
				angleDiffs[i] = 2*PI * 2; //is way more than enough
			} else {
				angleDiffs[i] = abs(atan2(b->y - t->y, b->x - t->x));
			}
		}
		targetTank = findMinIndex(angleDiffs, TankManager::getNumTanks());
		if (TankManager::getTank(targetTank)->getTeamID() == b->getTeamID()) {
			targetTank = -1;
		}
		delete[] angleDiffs;
	} else { //moveByDistance
		double* distDiffs = new double[TankManager::getNumTanks()];
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			Tank* t = TankManager::getTank(i);
			if (t->getTeamID() == b->getTeamID()) {
				distDiffs[i] = GAME_WIDTH*2 + GAME_HEIGHT*2; //should be enough
			} else {
				distDiffs[i] = sqrt(pow(b->x - t->x, 2) + pow(b->y - t->y, 2)); //TODO: this an issue?
			}
		}
		targetTank = findMinIndex(distDiffs, TankManager::getNumTanks());
		if (TankManager::getTank(targetTank)->getTeamID() == b->getTeamID()) {
			targetTank = -1;
		}
		delete[] distDiffs;
	}

	if (targetTank == -1) {
		return false; //means there was nothing to target; realistically, shouldn't be happening, unless 1-tank mode is a thing
	}
	Tank* t = TankManager::getTank(targetTank);

	double targetAngle = atan2(t->y - b->y, t->x - b->x);
	double posTargetAngle = fmod(targetAngle + 2*PI, 2*PI);
	double bulletAngle = (b->getAngle()>PI ? b->getAngle() - 2*PI : b->getAngle());
	//std::cout << targetAngle << " " << bulletAngle << " " << maxAngleMove << std::endl;

	if ((abs(targetAngle - bulletAngle) <= maxAngleMove) || (abs(fmod(targetAngle + PI, 2*PI) - fmod(bulletAngle + PI, 2*PI)) <= maxAngleMove)) {
		b->velocity.setAngle(targetAngle);
	} else {
		SimpleVector2D distToTank = SimpleVector2D(t->getX() - b->x, t->getY() - b->y);
		float theta = SimpleVector2D::angleBetween(distToTank, b->velocity);
		if (abs(theta) <= maxAngleMove) {
			//small angle adjustment needed
			b->velocity.setAngle(distToTank.getAngle());
		} else {
			//large angle adjustment needed
			if (theta < 0) {
				b->velocity.changeAngle(maxAngleMove);
			} else {
				b->velocity.changeAngle(-maxAngleMove);
			}
		}
	}

	return true; //could target something
}
