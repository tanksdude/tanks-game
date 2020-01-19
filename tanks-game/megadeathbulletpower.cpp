#pragma once
#include "megadeathbulletpower.h"
#include "megadeathtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"

bool MegaDeathBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w, int index) {
	if (getOffenseTier() >= 2) {
		if (CollisionHandler::partiallyCollided(b, w)) {
			delete walls[index];
			walls.erase(walls.begin() + index);
		}
		return false;
	} else {
		return CollisionHandler::partiallyCollided(b, w);
	}
}

void MegaDeathBulletPower::modifiedMovement(Bullet* b) {
	//b->velocity *= 1 + 1.0/64;
	b->velocity += 1.0/64;
	b->r *= 65.0/64;
}

double MegaDeathBulletPower::getDefenseTier() {
	double value = 2; //TODO: bulletpower knows its bullet? (the memory management big refactoring would be nullified) //no because tankpowers can make bulletpowers
	if (value >= 2) {
		modifiesCollisionWithWall = true;
		modifiedCollisionWithWallCanWorkWithOthers = false;
	}
	return value;
}

double MegaDeathBulletPower::getOffenseTier() {
	double value = 2;
	if (value >= 2) {
		modifiesCollisionWithWall = true;
		modifiedCollisionWithWallCanWorkWithOthers = false;
	}
	return value;
}

void MegaDeathBulletPower::initialize(Bullet* b) {
	//nothing
}

void MegaDeathBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* MegaDeathBulletPower::makeTankPower() {
	return new MegaDeathTankPower();
}

MegaDeathBulletPower::MegaDeathBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	//modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
}