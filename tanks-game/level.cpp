#include "level.h"

//this shouldn't really need to be changed, but I'll leave it here
std::vector<std::string> Level::getLevelTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> Level::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

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

Level::~Level() {
	for (int i = 0; i < getNumEffects(); i++) {
		delete effects[i];
	}
	effects.clear(); //does ~Level happen before ~child_Level?
}

/*
TODO: separate levels from level effects? (... is this not the case?)

list of (eventual) levels or level effects, I dunno:
|random (just need to flesh it out once powers and hazards exist)
|empty
invisibility? (no one appreciates it though)
|wind
ice (no one really likes it, as expected)
mines


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
