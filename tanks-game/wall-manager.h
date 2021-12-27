#pragma once
#include "wall.h"
#include <vector>

class WallManager {
	friend class ResetThings;

private:
	static std::vector<Wall*> walls;
	static void clearWalls(); //for ResetThings

public:
	static void initialize();
	static Wall* getWall(int index);
	static Wall* getWallByID(Game_ID);
	static void pushWall(Wall*);
	static int getNumWalls() { return walls.size(); }
	static void deleteWall(int index);
	static void deleteWallByID(Game_ID);

	static std::vector<Rect*> getWallCollisionList() {
		std::vector<Rect*> wallList; wallList.reserve(walls.size());
		for (int i = 0; i < walls.size(); i++) {
			wallList.push_back((Rect*)walls[i]);
		}
		return wallList;
	}

private:
	WallManager() {}
	WallManager(const WallManager&) {}
};
