#include "circle-hazard.h"

#include "constants.h"

#include "collision-handler.h"

/*
std::vector<std::string> CircleHazard::getHazardTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> CircleHazard::getWeights() const {
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

float CircleHazard::getHighestOffenseImportance() const {
	float highest = LOW_IMPORTANCE;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getOffenseImportance() > highest) {
			highest = hazardPowers[i]->getOffenseImportance();
		}
	}
	*/
	return highest;
}

float CircleHazard::getHighestOffenseTier(float importance) const {
	float highest = LOW_TIER;
	/*
	if (hazardPowers.size() == 0) {
		return 0;
	}
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getOffenseImportance() == importance) {
			if (hazardPowers[i]->getOffenseTier(this) > highest) {
				highest = hazardPowers[i]->getOffenseTier(this);
			}
		}
	}
	*/
	if (importance <= 0) {
		return (highest > getDefaultOffense() ? highest : getDefaultOffense());
	}
	return highest;
}

float CircleHazard::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

float CircleHazard::getHighestDefenseImportance() const {
	float highest = LOW_IMPORTANCE;
	/*
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getDefenseImportance() > highest) {
			highest = hazardPowers[i]->getDefenseImportance();
		}
	}
	*/
	return highest;
}

float CircleHazard::getHighestDefenseTier(float importance) const {
	float highest = LOW_TIER;
	/*
	if (hazardPowers.size() == 0) {
		return 0;
	}
	for (int i = 0; i < hazardPowers.size(); i++) {
		if (hazardPowers[i]->getDefenseImportance() == importance) {
			if (hazardPowers[i]->getDefenseTier(this) > highest) {
				highest = hazardPowers[i]->getDefenseTier(this);
			}
		}
	}
	*/
	if (importance <= 0) {
		return (highest > getDefaultDefense() ? highest : getDefaultDefense());
	}
	return highest;
}

float CircleHazard::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

//hazard notes:
/*
hazard ideas:
teleport zone
pipes?
magnetic hazard of some kind (when bullet or something is near it, shoots out some electricity (not lightning; there's a difference) and pulls it in)


JS hazard numbers (with names updated):
0 = targeting turret, 1 = horizontal lightning, 2 = lava (rectangular), 3 = stationary turret, 4 = vertical lightning, 5 = rectangular lightning, 6 = no bullet zone, 7 = patrolling turret (unimplemented)
quite the lack of circular hazards
C++ order of creating the JS hazards (probably):
stationary turret, horizontal lightning, lava, rectangular lightning, vertical lightning, no bullet zone, targeting turret, patrolling turret


random fun facts:
in JS, one day I got the idea for a turret, so I set about to implement it, but then got stuck because there was nowhere to add it; many hours later, the targeting turret finally made its first appearance
when I first showed off horizontal lightning to others (in JS), they thought it was made from drawing an image because of its grayer background
surprisingly, I don't think I got the idea for JS to have circular lightning, lava, or no bullet zones

*/
