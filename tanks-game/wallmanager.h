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
	static Wall* const getWall(int index);
	//see BulletManager explanation
	static void pushWall(Wall*);
	static int getNumWalls() { return walls.size(); }
	static void deleteWall(int index);
};
