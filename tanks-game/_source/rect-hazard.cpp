#include "rect-hazard.h"

#include "collision-handler.h"

/*
std::vector<std::string> RectHazard::getHazardTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> RectHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}
*/

void RectHazard::update(const RectHazardUpdateStruct* up) {
	this->x += up->x;
	this->y += up->y;
	this->w += up->w;
	this->h += up->h;
}

InteractionUpdateHolder<TankUpdateStruct, RectHazardUpdateStruct> RectHazard::modifiedTankCollision(const Tank* t) const {
	std::pair<double, double> vec = CollisionHandler::pushMovableAwayFromImmovable_vecOnly(t, this);
	return { false, false, new TankUpdateStruct(vec.first, vec.second, 0,0,0), nullptr };
}

InteractionUpdateHolder<BulletUpdateStruct, RectHazardUpdateStruct> RectHazard::modifiedBulletCollision(const Bullet* b) const {
	std::pair<double, double> vec = CollisionHandler::pushMovableAwayFromImmovable_vecOnly(b, this);
	return { false, false, new BulletUpdateStruct(vec.first, vec.second, 0,0,0,0), nullptr };
}

RectHazardUpdateStruct::RectHazardUpdateStruct(double x, double y, double w, double h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void RectHazardUpdateStruct::add(const RectHazardUpdateStruct& other) {
	this->x += other.x;
	this->y += other.y;
	this->w += other.w;
	this->h += other.h;
}

//see circle-hazard.cpp for hazard notes
