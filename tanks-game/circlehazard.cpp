#include "circlehazard.h"
#include "collisionhandler.h"
#include "constants.h"

/*
std::vector<std::string> CircleHazard::getHazardTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> CircleHazard::getWeights() const {
	//not really sure what the weights mean yet
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
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

/*
void CircleHazard::initializeGPU() {
	float positions[(Circle::numOfSides+1)*2];
	for (int i = 0; i < Circle::numOfSides; i++) {
		positions[i*2]   = cos(i * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin(i * 2*PI / Circle::numOfSides);
	}
	positions[Circle::numOfSides*2]   = 0;
	positions[Circle::numOfSides*2+1] = 0;

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = Circle::numOfSides;
		indices[i*3+1] = i;
		indices[i*3+2] = (i+1) % Circle::numOfSides;
	}

	//va = new VertexArray();
	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float));

	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(indices, Circle::numOfSides*3);
}
*/
