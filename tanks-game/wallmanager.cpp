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

void WallManager::clearWalls() {
	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}
	walls.clear();
}
