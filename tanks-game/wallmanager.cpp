#pragma once
#include "wallmanager.h"

std::vector<Wall*> WallManager::walls;
void WallManager::initialize() {
	return;
}

Wall* const WallManager::getWall(int index) {
	return walls[index];
}

void WallManager::pushWall(Wall* w) {
	walls.push_back(w);
}

void WallManager::deleteWall(int index) {
	delete walls[index];
	walls.erase(walls.begin() + index);
}