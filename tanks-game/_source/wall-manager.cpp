#include "wall-manager.h"

std::vector<Wall*> WallManager::walls;

void WallManager::initialize() {
	return;
}

Wall* WallManager::getWall(unsigned int index) {
	return walls[index];
}

Wall* WallManager::getWallByID(Game_ID gameID) {
	for (int i = 0; i < walls.size(); i++) {
		if (walls[i]->getGameID() == gameID) {
			return walls[i];
		}
	}
	return nullptr;
}

void WallManager::pushWall(Wall* w) {
	walls.push_back(w);
}

void WallManager::deleteWall(unsigned int index) {
	delete walls[index];
	walls.erase(walls.begin() + index);
}

void WallManager::deleteWallByID(Game_ID gameID) {
	for (int i = 0; i < walls.size(); i++) {
		if (walls[i]->getGameID() == gameID) {
			deleteWall(i);
			break;
		}
	}
}

void WallManager::clearWalls() {
	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}
	walls.clear();
}

std::vector<Rect*> WallManager::getWallCollisionList() {
	return std::vector<Rect*>(walls.begin(), walls.end());
}
