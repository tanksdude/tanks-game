#include "circle-hazard.h"
#include "collision-handler.h"
#include "constants.h"

/*
std::vector<std::string> CircleHazard::getHazardTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> CircleHazard::getWeights() const {
	//not really sure what the weights mean yet
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}
*/

void CircleHazard::modifiedTankCollision(Tank* t) {
	CollisionHandler::pushMovableAwayFromImmovable(t, this);
}

void CircleHazard::modifiedBulletCollision(Bullet* b) {
	CollisionHandler::pushMovableAwayFromImmovable(b, this);
}

double CircleHazard::getHighestOffenseImportance() const {
	double highest = LOW_IMPORTANCE;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getOffenseImportance() > highest) {
			highest = hazardPowers[i]->getOffenseImportance();
		}
	}
	*/
	return highest;
}

double CircleHazard::getHighestOffenseTier(double importance) const {
	double highest = LOW_TIER;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getOffenseImportance() == importance) {
			if (hazardPowers[i]->getOffenseTier(this) > highest) {
				highest = hazardPowers[i]->getOffenseTier(this);
			}
		}
	}
	if (hazardPowers.size() == 0) {
		return 0;
	}
	*/
	if (importance <= 0) {
		//I'm really glad this works in C++ without massive amounts of copy and pasting code
		return (highest > getDefaultOffense() ? highest : getDefaultOffense());
	}
	return highest;
}

double CircleHazard::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double CircleHazard::getHighestDefenseImportance() const {
	double highest = LOW_IMPORTANCE;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getDefenseImportance() > highest) {
			highest = hazardPowers[i]->getDefenseImportance();
		}
	}
	*/
	return highest;
}

double CircleHazard::getHighestDefenseTier(double importance) const {
	double highest = LOW_TIER;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getDefenseImportance() == importance) {
			if (hazardPowers[i]->getDefenseTier(this) > highest) {
				highest = hazardPowers[i]->getDefenseTier(this);
			}
		}
	}
	if (hazardPowers.size() == 0) {
		return 0;
	}
	*/
	if (importance <= 0) {
		return (highest > getDefaultDefense() ? highest : getDefaultDefense());
	}
	return highest;
}

double CircleHazard::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

//hazard notes:
/*
some hazards aren't bad

list of (eventual) hazards:
|stationary turret
|targeting turret
|patrolling turret
|rectangular/circular lightning
|horizontal lightning
|vertical lightning
|lava
|no bullet zone
teleport zone
pipes
trap?


other:
cloud (what would this do? spit out some powerups every now and then?) (it's *not* another lightning, that's for sure)
	(maybe it eats a bullet and spits out a powerup)
flag for capture the flag (base where the tank spawns; bringing it back gives godmode or something)
|wall hazards (it's a wall, except it's a hazard) (dev purposes only)
	maybe it can be pushed or something
magnetic hazard of some kind (when bullet or something is near it, shoots out some electricity (not lightning; there's a difference) and pulls it in)
tank swapper (swaps tanks every so often) (could also teleport them to some random place)


list of possible hazards, joke or not:
the reflecktor (bounces/reflects tanks when they hit it)


JS hazard numbers (with names updated):
0 = targeting turret, 1 = horizontal lightning, 2 = lava (rectangular), 3 = stationary turret, 4 = vertical lightning, 5 = rectangular lightning, 6 = no bullet zone, 7 = patrolling turret (unimplemented)
quite the lack of circular hazards
C++ order of creating the JS hazards (probably):
stationary turret, horizontal lightning, lava, rectangular lightning, vertical lightning, no bullet zone, targeting turret, patrolling turret

*/
