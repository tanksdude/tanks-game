#include "old-bounce-power.h"

//const int OldBouncePower::maxBounces = 16;

std::unordered_map<std::string, float> OldBouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	return weights;
}

TankPower* OldBouncePower::makeTankPower() const {
	return new OldBounceTankPower();
}

BulletPower* OldBouncePower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

Power* OldBouncePower::factory() {
	return new OldBouncePower();
}

OldBouncePower::OldBouncePower() : BouncePower() {
	return;
}



BulletPower* OldBounceTankPower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

OldBounceTankPower::OldBounceTankPower() : BounceTankPower() {
	//nothing new
}



TankPower* OldBounceBulletPower::makeTankPower() const {
	return new OldBounceTankPower();
}

OldBounceBulletPower::OldBounceBulletPower() : BounceBulletPower() {
	//nothing new
}
