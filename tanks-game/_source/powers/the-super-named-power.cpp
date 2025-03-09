#include "the-super-named-power.h"

#include "../constants.h"
#include <cmath> //sin, fmod

#include "../color-mixer.h"
#include "../game-manager.h" //getTickCount()

inline float TheSuperNamedPower::getTimeValue() {
	return static_cast<float>(GameManager::getTickCount()) / 200;
}

std::unordered_map<std::string, float> TheSuperNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	return weights;
}

std::string TheSuperNamedPower::getClassIdentifier() {
	return TheSuperNamedPower::getClassName();
}

ColorValueHolder TheSuperNamedPower::getClassColor() {
	return ColorMixer::mix(ColorValueHolder(0, 0, 0), ColorValueHolder(1, 1, 1), .5f + .5f*sin(static_cast<float>(2*PI) * TheSuperNamedPower::getTimeValue()));
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
