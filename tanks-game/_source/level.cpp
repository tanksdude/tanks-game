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
	effects.clear(); //in case ~Level happens after ~ChildLevel
}

/*
level ideas:
maze
level that randomly gives powers (that last one second)?


JS level numbers (with names updated):
special: 0 = default random, 1 = empty, 2 = portal level, 3 = winning path, 4 = tricky maneuvering, 5 = mine heaven; not special: 0 = hiding places, 1 = evened corridors, 2 = few obstacles, 3 = lone turret, 4 = concealed powerups, 5 = many hazards, 6 = lightning corners (but not really since it was cut)
the special vs. not special distinction was made so the boring not special levels could appear less often
C++ order of creating the JS levels (probably):
empty, default random, evened corridors, few obstacles, concealed powerups, many hazards, hiding places, lightning corners, lone turret, tricky maneuvering, mine heaven, winning path, portal level (kinda)


random fun facts:
in JS, most levels were made solely around a new power or other feature existing, and they were typically just a test level with a few walls that got slightly modified into a real level
the earlier levels (mainly in JS) are far simpler because that was all a power needed to show off
more levels was one of the most important things I wanted to address in this version, but I never quite delivered on that...
I think someone once said mines was their favorite powerup, so I made mine heaven level as a joke

*/
