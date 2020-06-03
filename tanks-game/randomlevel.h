#pragma once
#include "colorvalueholder.h"
#include <string>
#include "wall.h"

class RandomLevel { //has stuff for randomizing stuff in levels
public:
	static Wall* makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH);
	static Wall* makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c) {
		return RandomLevel::makeNewWall(x_beginning, y_beginning, width_ofArea, height_ofArea, c, 12, 8, 64, 96);
	}
	static std::string* getRandomPowers(int count, bool replacement, int nameCount, std::string* names);
};
