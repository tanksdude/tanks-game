#include "level.h"

/*
std::vector<std::string> Level::getLevelTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> Level::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}
*/

void Level::tickLevelEffects() {
	for (int i = 0; i < getNumEffects(); i++) {
		effects[i]->tick(this);
	}
}

void Level::doLevelEffects() {
	for (int i = 0; i < getNumEffects(); i++) {
		effects[i]->doEffects(this);
	}
}

void Level::drawLevelEffects() const {
	for (int i = 0; i < getNumEffects(); i++) {
		effects[i]->draw();
	}
}

void Level::drawLevelEffects(DrawingLayers layer) const {
	for (int i = 0; i < getNumEffects(); i++) {
		effects[i]->draw(layer);
	}
}

Level::~Level() {
	for (int i = 0; i < getNumEffects(); i++) {
		delete effects[i];
	}
	effects.clear(); //does ~Level happen before ~child_Level?
}

/*
list of (eventual) levels:
|empty
|invisible walls
|wind
|ice and mines


list of potential levels:
portal? (shouldn't be that hard if I do it correctly this time, right?)
maze


boring levels:
|hiding places (has mines?)
|evened corridors
|few obstacles (homing level)
|turret level (kinda boring)
|concealed powers (needs some spice to become good)
|many hazards
|lightning corners (needs spice, but is a kinda fun level)


other:
wallless (no walls, only no bullet zones)


test levels (purpose: to showcase some functionality):
one wall (corner collision, heck yeah!)
one room with each powerup, isolated, permanent
test bullet priorities? level controls tanks for ease of viewing
one room with each hazard (and necessary other materials, if needed)
one room with each level effect ('cuz why not at this point)
turret room: two turrets; one has distance-based targeting (just selection), one has angle-based targeting (just selection)


"other":
team mode! (for two tanks...)

*/
