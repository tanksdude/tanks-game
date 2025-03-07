#include "the-super-named-power.h"

#include "../constants.h"
#include <cmath> //sin

#include "../color-mixer.h"
#include "../game-manager.h" //getTickCount()

std::unordered_map<std::string, float> TheSuperNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	return weights;
}

ColorValueHolder TheSuperNamedPower::getClassColor() {
	return ColorMixer::mix(ColorValueHolder(0, 0, 0), ColorValueHolder(1, 1, 1), .5 + .5*sin((2*PI) * (GameManager::getTickCount() / 200)));
}

TankPower* TheSuperNamedPower::makeTankPower() const {
	return new TheSuperNamedTankPower();
}

BulletPower* TheSuperNamedPower::makeBulletPower() const {
	return new TheSuperNamedBulletPower();
}

Power* TheSuperNamedPower::factory() {
	return new TheSuperNamedPower();
}

TheSuperNamedPower::TheSuperNamedPower() {
	return;
}



BulletPower* TheSuperNamedTankPower::makeBulletPower() const {
	return new TheSuperNamedBulletPower();
}

TheSuperNamedTankPower::TheSuperNamedTankPower() {
	maxTime = 1000;
	timeLeft = 1000;
}



InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> TheSuperNamedBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	//copied from BigNamedBulletPower
	return { false, true, nullptr, nullptr };
}

TankPower* TheSuperNamedBulletPower::makeTankPower() const {
	return new TheSuperNamedTankPower();
}

TheSuperNamedBulletPower::TheSuperNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
