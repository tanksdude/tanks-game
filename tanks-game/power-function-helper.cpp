#include "power-function-helper.h"

#include "constants.h"
#include <cmath>
#include <iostream>
#include "mylib.h" //findMinIndex

#include "collision-handler.h"
#include "tank-manager.h"

std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> PowerFunctionHelper::superbounceGeneric(const Bullet* b, const Wall* w, double strength) {
	if (!CollisionHandler::partiallyCollided(b, w)) {
		return { false, { false, false, nullptr, nullptr } };
	}

	double b_xDelta, b_yDelta, b_angleDelta;
	double w_xDelta, w_yDelta;

	if (b->y - w->y <= (w->h / w->w) * (b->x - w->x)) { //I think this is bottom right
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //bottom
			b_yDelta = -1 * ((b->y + b->r - w->y) * 2); //b->y = w->y - b->r
			b_angleDelta = -2 * b->velocity.getAngle(); //b->velocity.setAngle(b->velocity.getAngle() * -1);
			w_yDelta = strength;
			b_xDelta = 0, w_xDelta = 0;
		} else { //right
			b_xDelta = (w->x + w->w - (b->x - b->r)) * 2; //b->x = w->x + w->w + b->r
			b_angleDelta = PI - 2*b->velocity.getAngle(); //b->velocity.setAngle(PI - b->velocity.getAngle());
			w_xDelta = -1 * strength;
			b_yDelta = 0, w_yDelta = 0;
		}
	} else { //top left?
		if (b->y - (w->y + w->h) <= (-w->h / w->w) * (b->x - w->x)) { //left
			b_xDelta = -1 * ((b->x + b->r - w->x) * 2); //b->x = w->x - b->r
			b_angleDelta = PI - 2*b->velocity.getAngle(); //b->velocity.setAngle(PI - b->velocity.getAngle());
			w_xDelta = strength;
			b_yDelta = 0, w_yDelta = 0;
		} else { //top
			b_yDelta = (w->y + w->h - (b->y - b->r)) * 2; //b->y = w->y + w->h + b->r
			b_angleDelta = -2 * b->velocity.getAngle(); //b->velocity.setAngle(b->velocity.getAngle() * -1);
			w_yDelta = -1 * strength;
			b_xDelta = 0, w_xDelta = 0;
		}
	}
	//TODO: ensure bullet is not actually in wall; move bullet to edge of relevant wall if still colliding (TODO: this still relevant?)

	return { true, { false, false, new BulletUpdateStruct(b_xDelta, b_yDelta, 0,0, b_angleDelta, 0), new WallUpdateStruct(w_xDelta, w_yDelta, 0,0) } };
	//TODO: doesn't mean it should be deleted; should separate the update struct
}

std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> PowerFunctionHelper::superbounceGenericWithCorners(const Bullet* b, const Wall* w, double strength) { //not the default because bullets move too quickly on average
	if (!CollisionHandler::partiallyCollided(b, w)) {
		return { false, { false, false, nullptr, nullptr } };
	}

	if ((b->x < w->x) && (b->y < w->y)) { //circle in bottom left
		return PowerFunctionHelper::superbounceGenericCornerHandler(b, w, w->x, w->y, strength);
	}
	//can't do elses because more than one corner could be in the circle (TODO: really?)
	if ((b->x > (w->x + w->w)) && (b->y < w->y)) { //circle in bottom right
		return PowerFunctionHelper::superbounceGenericCornerHandler(b, w, (w->x + w->w), w->y, strength);
	}
	if ((b->x < w->x) && (b->y > (w->y + w->h))) { //circle in top left
		return PowerFunctionHelper::superbounceGenericCornerHandler(b, w, w->x, (w->y + w->h), strength);
	}
	if ((b->x > (w->x + w->w)) && (b->y > (w->y + w->h))) { //circle in top right
		return PowerFunctionHelper::superbounceGenericCornerHandler(b, w, (w->x + w->w), (w->y + w->h), strength);
	}

	return PowerFunctionHelper::superbounceGeneric(b, w, strength);
	//return false;
}

std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> PowerFunctionHelper::superbounceGenericCornerHandler(const Bullet* b, const Wall* w, double x, double y, double strength) {
	//I have no idea if this is correct but it behaves exactly as I want it to, I think
	//it's not exact because intersection points aren't calculated but it's close enough

	if ((abs(x - b->x) <= b->r) && (abs(y - b->y) <= b->r)) {
		double d = sqrt(pow(x - b->x, 2) + pow(y - b->y, 2));
		if (d <= b->r) {
			double b_xDelta = 0, b_yDelta = 0, b_angleDelta;
			double w_xDelta, w_yDelta;

			double angle = atan2((y - b->y), (x - b->x));
			b_yDelta -= sin(angle) * (b->r - d);
			b_xDelta -= cos(angle) * (b->r - d);

			//so a rectangle has an area of influence against a circle: outer edges + radius, and corners are radius (picture a rounded rectangle)
			//when a bullet's center enters the area, it is inside the rectangle, and therefore needs to reflect
			//in effect, rounded rectangle against a point is the same as rectangle against circle
			//the bullet's angle needs to reflect off of the perpendicular to the tangent, and the tangent goes through the intersection between the bullet's path and the area of influence

			double newAngle = 2*angle - (b->velocity.getAngle() - PI);
			b_yDelta += sin(newAngle) * (b->r - d);
			b_xDelta += cos(newAngle) * (b->r - d);
			b_angleDelta = (newAngle - b->velocity.getAngle()); //b->velocity.setAngle(newAngle);

			w_yDelta = sin(angle) * strength;
			w_xDelta = cos(angle) * strength;

			return { true, { false, false, new BulletUpdateStruct(b_xDelta, b_yDelta, 0,0, b_angleDelta, 0), new WallUpdateStruct(w_xDelta, w_yDelta, 0,0) } };
		}
	}

	return { false, { false, false, {}, {} } };
}

std::pair<bool, BulletUpdateStruct> PowerFunctionHelper::superbounceEdgeGenericX(const Bullet* b, double) {
	bool bounced = 0; //wait, why is this =0? leftover from edge bouncing X and Y being together?
	double b_xDelta, b_yDelta=0, b_angleDelta;

	if (b->x + b->r > GAME_WIDTH) { //right edge
		b_xDelta = -1 * (((b->x + b->r) - GAME_WIDTH) * 2);
		b_angleDelta = PI - 2*b->velocity.getAngle(); //b->velocity.setAngle(PI - b->velocity.getAngle());
		bounced = true;
	} else if (b->x - b->r < 0) { //left edge
		b_xDelta = -(b->x - b->r) * 2;
		b_angleDelta = PI - 2*b->velocity.getAngle(); //b->velocity.setAngle(PI - b->velocity.getAngle());
		bounced = true;
	} else {
		b_xDelta = 0;
		b_angleDelta = 0;
	}

	return { bounced, BulletUpdateStruct(b_xDelta, b_yDelta, 0,0, b_angleDelta, 0) };
}

std::pair<bool, BulletUpdateStruct> PowerFunctionHelper::superbounceEdgeGenericY(const Bullet* b, double) {
	bool bounced = 0;
	double b_xDelta=0, b_yDelta, b_angleDelta;

	if (b->y + b->r > GAME_HEIGHT) { //top edge
		b_yDelta = -1 * (((b->y + b->r) - GAME_HEIGHT) * 2);
		b_angleDelta = -2 * b->velocity.getAngle(); //b->velocity.setAngle(b->velocity.getAngle() * -1);
		bounced = true;
	} else if (b->y - b->r < 0) { //bottom edge
		b_yDelta = -(b->y - b->r) * 2;
		b_angleDelta = -2 * b->velocity.getAngle(); //b->velocity.setAngle(b->velocity.getAngle() * -1);
		bounced = true;
	} else {
		b_yDelta = 0;
		b_angleDelta = 0;
	}

	return { bounced, BulletUpdateStruct(b_xDelta, b_yDelta, 0,0, b_angleDelta, 0) };
}

std::vector<double>* PowerFunctionHelper::equallySpacedCannonPoints(int count) {
	std::vector<double>* newCannonPoints = new std::vector<double>;
	newCannonPoints->reserve(count-1);
	for (int i = 1; i < count; i++) {
		newCannonPoints->push_back(i / double(count));
	}
	return newCannonPoints;
}

Game_ID PowerFunctionHelper::homingGenericTarget(const Bullet* b, bool targetUsingAngleDiff) {
	int targetTankIndex; //only targets tanks for now

	if (targetUsingAngleDiff) {
		double* angleDiffs = new double[TankManager::getNumTanks()];
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			const Tank* t = TankManager::getTank(i);
			if (t->getTeamID() == b->getTeamID()) {
				angleDiffs[i] = (2*PI) * 2; //is way more than enough
			} else {
				angleDiffs[i] = abs(atan2(b->y - t->y, b->x - t->x));
			}
		}
		targetTankIndex = findMinIndex(angleDiffs, TankManager::getNumTanks());
		if (TankManager::getTank(targetTankIndex)->getTeamID() == b->getTeamID()) {
			targetTankIndex = -1;
		}
		delete[] angleDiffs;
	} else { //targetUsingDistance
		double* distDiffs = new double[TankManager::getNumTanks()];
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			const Tank* t = TankManager::getTank(i);
			if (t->getTeamID() == b->getTeamID()) {
				distDiffs[i] = GAME_WIDTH*2 + GAME_HEIGHT*2; //should be enough
			} else {
				distDiffs[i] = sqrt(pow(b->x - t->x, 2) + pow(b->y - t->y, 2)); //TODO: this an issue?
			}
		}
		targetTankIndex = findMinIndex(distDiffs, TankManager::getNumTanks());
		if (TankManager::getTank(targetTankIndex)->getTeamID() == b->getTeamID()) {
			targetTankIndex = -1;
		}
		delete[] distDiffs;
	}

	if (targetTankIndex == -1) {
		return -1; //means there was nothing to target; realistically, shouldn't be happening, unless 1-tank mode is a thing
	}
	return TankManager::getTank(targetTankIndex)->getGameID();
}

void PowerFunctionHelper::homingGenericMove(Bullet* b, Game_ID targetID, double maxAngleChange) {
	const Tank* t = TankManager::getTankByID(targetID);

	double targetAngle = atan2(t->y - b->y, t->x - b->x);
	double posTargetAngle = fmod(targetAngle + 2*PI, 2*PI);
	double bulletAngle = (b->getAngle()>PI ? b->getAngle() - 2*PI : b->getAngle());
	//std::cout << targetAngle << " " << bulletAngle << " " << maxAngleChange << std::endl;

	if ((abs(targetAngle - bulletAngle) <= maxAngleChange) || (abs(fmod(targetAngle + PI, 2*PI) - fmod(bulletAngle + PI, 2*PI)) <= maxAngleChange)) {
		b->velocity.setAngle(targetAngle);
	} else {
		SimpleVector2D distToTank = SimpleVector2D(t->getX() - b->x, t->getY() - b->y);
		float theta = SimpleVector2D::angleBetween(distToTank, b->velocity);
		if (abs(theta) <= maxAngleChange) {
			//small angle adjustment needed
			b->velocity.setAngle(distToTank.getAngle());
		} else {
			//large angle adjustment needed
			if (theta < 0) {
				b->velocity.changeAngle(maxAngleChange);
			} else {
				b->velocity.changeAngle(-maxAngleChange);
			}
		}
	}
}
