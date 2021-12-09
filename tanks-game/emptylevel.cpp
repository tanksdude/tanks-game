#include "emptylevel.h"
#include "tankmanager.h"
#include "resetthings.h"

std::unordered_map<std::string, float> EmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .25f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "old", .25f }); //not really necessary
	weights.insert({ "random", .125f }); //it's boring so it shouldn't appear very often
	return weights;
}

void EmptyLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);
	//positions are also reset in ResetThings, but may as well put this here in case something changes
}

Level* EmptyLevel::factory() {
	return new EmptyLevel();
}

EmptyLevel::EmptyLevel() { return; }
