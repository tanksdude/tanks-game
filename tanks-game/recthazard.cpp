#include "recthazard.h"
#include "collisionhandler.h"

/*
std::vector<std::string> RectHazard::getHazardTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> RectHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}
*/

void RectHazard::modifiedTankCollision(Tank* t) {
	CollisionHandler::pushMovableAwayFromImmovable(t, this);
}

void RectHazard::modifiedBulletCollision(Bullet* b) {
	CollisionHandler::pushMovableAwayFromImmovable(b, this);
}

double RectHazard::getHighestOffenseImportance() const {
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

double RectHazard::getHighestOffenseTier(double importance) const {
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
		return (highest > getDefaultOffense() ? highest : getDefaultOffense());
	}
	return highest;
}

double RectHazard::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double RectHazard::getHighestDefenseImportance() const {
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

double RectHazard::getHighestDefenseTier(double importance) const {
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

double RectHazard::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

/*
void RectHazard::initializeGPU() {
	float positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//va = new VertexArray();
	vb = new VertexBuffer(positions, 4*2 * sizeof(float));

	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(indices, 6);
}
*/
