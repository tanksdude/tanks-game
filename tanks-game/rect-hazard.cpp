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

void RectHazard::modifiedTankCollision(Tank* t) {
	CollisionHandler::pushMovableAwayFromImmovable(t, this);
}

void RectHazard::modifiedBulletCollision(Bullet* b) {
	CollisionHandler::pushMovableAwayFromImmovable(b, this);
}

float RectHazard::getHighestOffenseImportance() const {
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

float RectHazard::getHighestOffenseTier(float importance) const {
	float highest = LOW_TIER;
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
		return (highest > getDefaultOffense() ? highest : getDefaultOffense());
	}
	return highest;
}

float RectHazard::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

float RectHazard::getHighestDefenseImportance() const {
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

float RectHazard::getHighestDefenseTier(float importance) const {
	float highest = LOW_TIER;
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

float RectHazard::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

//see circle-hazard.cpp for hazard notes
