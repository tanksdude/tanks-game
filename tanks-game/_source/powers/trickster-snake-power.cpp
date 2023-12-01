#include "trickster-snake-power.h"

const double TricksterSnakePower::timePeriod = 50;
const double TricksterSnakePower::movementAmplitude = 16.0/timePeriod;

std::unordered_map<std::string, float> TricksterSnakePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* TricksterSnakePower::makeTankPower() const {
	return new TricksterSnakeTankPower();
}

BulletPower* TricksterSnakePower::makeBulletPower() const {
	return new TricksterSnakeBulletPower();
}

/*
HazardPower* TricksterSnakePower::makeHazardPower() const {
	return new TricksterSnakeHazardPower();
}
*/

Power* TricksterSnakePower::factory() {
	return new TricksterSnakePower();
}

TricksterSnakePower::TricksterSnakePower() {
	return;
}



BulletPower* TricksterSnakeTankPower::makeBulletPower() const {
	return new TricksterSnakeBulletPower();
}

TricksterSnakeTankPower::TricksterSnakeTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "../constants.h"
#include <cmath>

InteractionBoolHolder TricksterSnakeBulletPower::modifiedMovement(Bullet* b) {
	//ypos = TricksterSnakePower::movementAmplitude * sin((2*PI) * (tickCount / TricksterSnakePower::timePeriod))
	//ydiff = A * (2*PI) * cos((2*PI) * tick)

	const float moveDiff = (TricksterSnakePower::movementAmplitude * (2*PI)) * cos((2*PI) * (tickCount / TricksterSnakePower::timePeriod));
	const SimpleVector2D perpendicularMoveDiff = SimpleVector2D(b->velocity.getAngle() + PI/2, moveDiff, true);

	b->x += perpendicularMoveDiff.getXComp();
	b->y += perpendicularMoveDiff.getYComp();

	tickCount++; //not sure whether this should go at the start or end
	return { false };
}

TankPower* TricksterSnakeBulletPower::makeTankPower() const {
	return new TricksterSnakeTankPower();
}

TricksterSnakeBulletPower::TricksterSnakeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;

	tickCount = 0;
}
