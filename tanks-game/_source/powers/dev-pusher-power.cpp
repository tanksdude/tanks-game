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

InteractionUpdateHolder<TankUpdateStruct, WallUpdateStruct> DevPusherTankPower::modifiedCollisionWithWall(const Tank* t, const Wall* w) {
	std::pair<std::pair<double, double>, std::pair<double, double>> vecs = CollisionHandler::pushMovableAwayFromMovable_vecOnly(t, w);
	return { false, false, new TankUpdateStruct(vecs.first.first, vecs.first.second, 0,0,0), new WallUpdateStruct(vecs.second.first, vecs.second.second, 0,0) };
}

bool DevPusherTankPower::getModifiesCollisionWithCircleHazard(const CircleHazard* ch) const {
	return (ch->getCollisionType() == CircleHazardCollisionType::solid);
}

bool DevPusherTankPower::getModifiesCollisionWithRectHazard(const RectHazard* rh) const {
	return (rh->getCollisionType() == RectHazardCollisionType::solid);
}

InteractionUpdateHolder<TankUpdateStruct, CircleHazardUpdateStruct> DevPusherTankPower::modifiedCollisionWithCircleHazard(const Tank* t, const CircleHazard* ch) {
	std::pair<std::pair<double, double>, std::pair<double, double>> vecs = CollisionHandler::pushMovableAwayFromMovable_vecOnly(t, ch);
	return { false, false, new TankUpdateStruct(vecs.first.first, vecs.first.second, 0,0,0), new CircleHazardUpdateStruct(vecs.second.first, vecs.second.second, 0) };
}

InteractionUpdateHolder<TankUpdateStruct, RectHazardUpdateStruct> DevPusherTankPower::modifiedCollisionWithRectHazard(const Tank* t, const RectHazard* rh) {
	std::pair<std::pair<double, double>, std::pair<double, double>> vecs = CollisionHandler::pushMovableAwayFromMovable_vecOnly(t, rh);
	return { false, false, new TankUpdateStruct(vecs.first.first, vecs.first.second, 0,0,0), new RectHazardUpdateStruct(vecs.second.first, vecs.second.second, 0,0) };
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
