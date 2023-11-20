#include "invisible-walls-level-effect.h"

#include "../wall-manager.h"
#include "../background-rect.h"

std::unordered_map<std::string, float> InvisibleWallsLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.25f });
	weights.insert({ "old", 0.125f });
	return weights;
}

void InvisibleWallsLevelEffect::apply() {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->color = BackgroundRect::getBackColor();
	}
	//TODO: should this fake invisibility method be the intended way?
	//TODO: if the background can change color, use tick() or doEffects()
}

InvisibleWallsLevelEffect::InvisibleWallsLevelEffect() {
	//nothing
}

InvisibleWallsLevelEffect::~InvisibleWallsLevelEffect() {
	//nothing
}

LevelEffect* InvisibleWallsLevelEffect::factory(const GenericFactoryConstructionData& args) {
	return new InvisibleWallsLevelEffect();
}
