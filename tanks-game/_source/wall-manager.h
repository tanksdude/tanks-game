#pragma once
#include <vector>
#include "wall.h"

class WallManager {
	friend class ResetThings;

private:
	static std::vector<Wall*> walls;
	static void clearWalls(); //for ResetThings

public:
	static void initialize();
	static Wall* getWall(unsigned int index);
	static Wall* getWallByID(Game_ID);
	static void pushWall(Wall*);
	static unsigned int getNumWalls() { return walls.size(); }
	static void deleteWall(unsigned int index);
	static void deleteWallByID(Game_ID);

private:
	WallManager() = delete;
	WallManager(const WallManager&) = delete;
};
