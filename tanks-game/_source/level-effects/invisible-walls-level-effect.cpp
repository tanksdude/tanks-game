#include "invisible-walls-level-effect.h"

#include "../wall-manager.h"
#include "../background-rect.h"

std::unordered_map<std::string, float> InvisibleWallsLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.25f });
	weights.insert({ "old", 0.25f });
	return weights;
}

void InvisibleWallsLevelEffect::apply() {
	/*
	int length = WallManager::getNumWalls();
	for (int i = 0; i < length; i++) {
		Wall* w = WallManager::getWall(0);
		WallManager::pushWall(new Wall(w->getX(), w->getY(), w->getW(), w->getH(), BackgroundRect::getBackColor()));
		WallManager::deleteWall(0);
		//this changes the gameID which is not wanted
	}
	*/
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->color = BackgroundRect::getBackColor();
	}
	//TODO: should this fake invisibility method be the intended way?
}

void InvisibleWallsLevelEffect::tick(const Level* parent) {
	//TODO: if the background can change color, update this (or doEffects?)
	//nothing
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
