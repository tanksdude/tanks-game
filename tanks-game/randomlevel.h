#pragma once
#include "colorvalueholder.h"
#include <string>
#include "wall.h"
#include "positionholder.h"

class RandomLevel { //has stuff for randomizing stuff in levels
public:
	static Wall* makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH);
	static inline Wall* makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c) {
		return RandomLevel::makeNewRandomWall(x_beginning, y_beginning, width_ofArea, height_ofArea, c, 12, 8, 64, 96);
	}
	static std::string* getRandomPowers(int count, bool replacement, int nameCount, std::string* names);
	static std::string powerAlternate(int position, int rand, std::string p1, std::string p2); //position: {0,1,2,3}, rand: {0,1}

	static PositionHolder getSymmetricPowerupPositions_LR(int position, double x_center, double y_center, double x_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_UD(int position, double x_center, double y_center, double y_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1,2,3}
	static PositionHolder getSymmetricPowerupPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1}
	//about _DiagForwardSlash and _DiagBackwardSlash: it's half of _Corners; uses the diagonal two that go in the direction of a regular (forward) slash (/) or backslash (\)

	static PositionHolder getSymmetricWallPositions_LR(int position, double x_center, double y_center, double x_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_UD(int position, double x_center, double y_center, double y_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1,2,3}
	static PositionHolder getSymmetricWallPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1}
};
