#include "dev-pusher-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> DevPusherPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 0.25f }); //it's supposed to be a test power, not actually used
	return weights;
}

TankPower* DevPusherPower::makeTankPower() const {
	return new DevPusherTankPower();
}

BulletPower* DevPusherPower::makeBulletPower() const {
	return new DevPusherBulletPower();
}

Power* DevPusherPower::factory() {
	return new DevPusherPower();
}

DevPusherPower::DevPusherPower() {
	return;
}



#include "../collision-handler.h"

InteractionBoolHolder DevPusherTankPower::modifiedCollisionWithWall(Tank* t, Wall* w) {
	//TODO: supposed to check if they're still collided first; don't care, it's a dev power
	CollisionHandler::pushMovableAwayFromMovable(t, w);
	return { false, false };
}

bool DevPusherTankPower::getModifiesCollisionWithCircleHazard(const CircleHazard* ch) const {
	return (ch->getCollisionType() == CircleHazardCollisionType::solid);
}

bool DevPusherTankPower::getModifiesCollisionWithRectHazard(const RectHazard* rh) const {
	return (rh->getCollisionType() == RectHazardCollisionType::solid);
}

InteractionBoolHolder DevPusherTankPower::modifiedCollisionWithCircleHazard(Tank* t, CircleHazard* ch) {
	CollisionHandler::pushMovableAwayFromMovable(t, ch);
	return { false, false };
}

InteractionBoolHolder DevPusherTankPower::modifiedCollisionWithRectHazard(Tank* t, RectHazard* rh) {
	CollisionHandler::pushMovableAwayFromMovable(t, rh);
	return { false, false };
}

BulletPower* DevPusherTankPower::makeBulletPower() const {
	return new DevPusherBulletPower();
}

DevPusherTankPower::DevPusherTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;

	modifiesCollisionWithWall = true;
}



TankPower* DevPusherBulletPower::makeTankPower() const {
	return new DevPusherTankPower();
}

DevPusherBulletPower::DevPusherBulletPower() {
	//nothing
}
