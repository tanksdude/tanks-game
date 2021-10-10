#include "oldbouncepower.h"

std::unordered_map<std::string, float> OldBouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	return weights;
}

TankPower* OldBouncePower::makeTankPower() const {
	return new OldBounceTankPower();
}

BulletPower* OldBouncePower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

/*
HazardPower* OldBouncePower::makeHazardPower() const {
	return new OldBounceHazardPower();
}
*/

Power* OldBouncePower::factory() {
	return new OldBouncePower();
}

OldBouncePower::OldBouncePower() {
	return;
}



void OldBounceTankPower::initialize(Tank* parent) {
	//nothing
}

void OldBounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldBounceTankPower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

OldBounceTankPower::OldBounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "powerfunctionhelper.h"
#include "collisionhandler.h"

const short OldBounceBulletPower::maxBounces = 16;

InteractionBoolHolder OldBounceBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (abs(b->velocity.getMagnitude()) * Bullet::default_radius/b->r <= .5) {
		if (PowerFunctionHelper::bounceGenericWithCorners(b, w)) {
			bouncesLeft--;
		}
	} else {
		if (PowerFunctionHelper::bounceGeneric(b, w)) {
			bouncesLeft--;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithWall = false;
		modifiesEdgeCollision = false;
	}

	return { (bouncesLeft < 0), false };
}

InteractionBoolHolder OldBounceBulletPower::modifiedEdgeCollision(Bullet* b) {
	//the bullet can bounce off of edges twice in a single tick
	//therefore, it can lose 2 bounces at once
	//shouldn't ever have negative bounces, so need to check Y, then X, then Y if Y wasn't already checked, and check bouncesLeft after each edge bounce
	//I could have different checks for partiallyOutOfBounds() to only require one check each but whatever

	bool bouncedY = false;
	if (CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			bouncedY = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesEdgeCollision = false;
		modifiesCollisionWithWall = false;
		return { CollisionHandler::fullyOutOfBounds(b) };
	}

	//bool bouncedX = false;
	if (CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericX(b)) {
			bouncesLeft--;
			//bouncedX = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesEdgeCollision = false;
		modifiesCollisionWithWall = false;
		return { CollisionHandler::fullyOutOfBounds(b) };
	}

	if (!bouncedY && CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			//bouncedY = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesEdgeCollision = false;
		modifiesCollisionWithWall = false;
	}

	return { CollisionHandler::fullyOutOfBounds(b) };
}

void OldBounceBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldBounceBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* OldBounceBulletPower::makeTankPower() const {
	return new OldBounceTankPower();
}

OldBounceBulletPower::OldBounceBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = OldBounceBulletPower::maxBounces;
	modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
	modifiesEdgeCollision = true;
}
