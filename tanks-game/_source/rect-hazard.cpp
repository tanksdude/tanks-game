#include "rect-hazard.h"

#include "constants.h"

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

void RectHazard::modifiedTankCollision(Tank* t) {
	CollisionHandler::pushMovableAwayFromImmovable(t, this);
}

void RectHazard::modifiedBulletCollision(Bullet* b) {
	CollisionHandler::pushMovableAwayFromImmovable(b, this);
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
