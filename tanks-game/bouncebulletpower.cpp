#pragma once
#include "bouncebulletpower.h"
#include "bouncetankpower.h"
#include "bouncepower.h"
#include "powerfunctionhelper.h"

const short BounceBulletPower::maxBounces = 16;

PowerInteractionBoolHolder BounceBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (abs(b->velocity) * Bullet::default_radius/b->r <= .5) {
		if (PowerFunctionHelper::bounceGenericWithCorners(b, w)) {
			bouncesLeft--;
		}
	}
	else {
		if (PowerFunctionHelper::bounceGeneric(b, w)) {
			bouncesLeft--;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithWall = false;
	}

	return { this->bouncesLeft < 0 };
}
//TODO: need ability to delete just the bulletpower (needed? wanted?)

PowerInteractionBoolHolder BounceBulletPower::modifiedEdgeCollision(Bullet* b) {
	//the bullet can bounce off of edges twice in a single tick
	//therefore, it can lose 2 bounces at once
	//shouldn't ever have negative bounces, so need to check Y, then X, then Y if Y wasn't already checked, and check bouncesLeft after each edge bounce
	//I could have different checks for partiallyOutOfBounds() to only require one check each but whatever

	bool bouncedY = false;
	if (b->isPartiallyOutOfBounds()) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			bouncedY = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithEdge = false;
		return { b->isFullyOutOfBounds() };
	}

	//bool bouncedX = false;
	if (b->isPartiallyOutOfBounds()) {
		if (PowerFunctionHelper::bounceEdgeGenericX(b)) {
			bouncesLeft--;
			//bouncedX = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithEdge = false;
		return { b->isFullyOutOfBounds() };
	}

	if(!bouncedY && b->isPartiallyOutOfBounds()) {
		if (PowerFunctionHelper::bounceEdgeGenericY(b)) {
			bouncesLeft--;
			//bouncedY = true;
		}
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithEdge = false;
	}

	return { b->isFullyOutOfBounds() };
}

void BounceBulletPower::initialize(Bullet* b) {
	//b->velocity /= 2; //no longer needs this
}

void BounceBulletPower::removeEffects(Bullet * b) {
	//b->velocity *= 2;
}

TankPower* BounceBulletPower::makeTankPower() {
	return new BounceTankPower();
}

BounceBulletPower::BounceBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = BounceBulletPower::maxBounces;
	modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
	modifiesCollisionWithEdge = true;
}