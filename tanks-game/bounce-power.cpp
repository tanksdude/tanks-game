#include "bounce-power.h"

const int BouncePower::maxBounces = 16;

std::unordered_map<std::string, float> BouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	//weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* BouncePower::makeTankPower() const {
	return new BounceTankPower();
}

BulletPower* BouncePower::makeBulletPower() const {
	return new BounceBulletPower();
}

/*
HazardPower* BouncePower::makeHazardPower() const {
	return new BounceHazardPower();
}
*/

Power* BouncePower::factory() {
	return new BouncePower();
}

BouncePower::BouncePower() {
	return;
}



//not reducing size on the bullet or tank in this version because I originally only did that to make it different from others
//(I think that was literally the only reason)

void BounceTankPower::initialize(Tank* parent) {
	//nothing
}

void BounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BounceTankPower::makeBulletPower() const {
	return new BounceBulletPower();
}

BounceTankPower::BounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "power-function-helper.h"
#include "collision-handler.h"

InteractionBoolHolder BounceBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (abs(b->velocity.getMagnitude()) * Bullet::default_radius/b->r <= .5) {
		//should abs() be used? it's not needed...
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
//TODO: need ability to delete just the bulletpower (needed? wanted? no because bounces should be reset for banana (should it?))

InteractionBoolHolder BounceBulletPower::modifiedEdgeCollision(Bullet* b) {
	//the bullet can bounce off of edges twice in a single tick
	//therefore, it can lose 2 bounces at once
	//shouldn't ever have negative bounces, so need to check Y, then X, then Y if Y wasn't already checked, and check bouncesLeft after each edge bounce
	//I could have different checks for partiallyOutOfBounds() to only require one check each but whatever

	bool bouncedY = false;
	//bool bouncedX = false;

	if (CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			bouncedY = true;
		}
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	if (CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericX(b)) {
			bouncesLeft--;
			//bouncedX = true;
		}
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	if (!bouncedY && CollisionHandler::partiallyOutOfBounds(b)) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			//bouncedY = true;
		}
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			//return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	return { CollisionHandler::fullyOutOfBounds(b) };
}

void BounceBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BounceBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* BounceBulletPower::makeDuplicate() const {
	return new BounceBulletPower(this->bouncesLeft);
}

TankPower* BounceBulletPower::makeTankPower() const {
	return new BounceTankPower();
}

BounceBulletPower::BounceBulletPower() : BounceBulletPower(BouncePower::maxBounces) {}

BounceBulletPower::BounceBulletPower(int bounces) {
	timeLeft = 0;
	maxTime = -1;

	this->bouncesLeft = bounces;
	if (bounces > 0) {
		modifiesCollisionWithWall = true;
		//modifiedCollisionWithWallCanWorkWithOthers = false;
		modifiesEdgeCollision = true;
	}
}
