#pragma once
#include "color-value-holder.h"
#include <string>
#include "wall.h"
#include "position-holder.h"

class LevelHelper { //has stuff for randomizing stuff in levels
public:
	static Wall* makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, const ColorValueHolder& c, double minW, double minH, double maxW, double maxH);
	static inline Wall* makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, const ColorValueHolder& c) {
		return LevelHelper::makeNewRandomWall(x_beginning, y_beginning, width_ofArea, height_ofArea, c, 12, 8, 64, 96);
	}
	static void pushClassicWalls(const ColorValueHolder& c);
	[[nodiscard]] static PositionHolder* getClassicWallPositions();

	static std::string* getRandomPowers(int count, std::string type); //handles stacking and weights for a whole group (use this)
	static std::string* getRandomPowers(int count, std::string type, std::string* names, int nameCount); //no option for multiple types (it's effort)
	static std::string* getRandomPowersOld(int count, bool replacement, std::string* names, int nameCount); //equal weight, replacement is an option
	static std::string* getRandomPowers(int count, bool* powersCanStack, std::string* names, int nameCount); //equal weight, replaces the non-stacking names
	static std::string* getRandomPowers(int count, bool* powersCanStack, std::string* names, float* weights, int powerCount); //nonequal weight, replaces the non-stacking names
	static std::string powerAlternate(int position, int rand, std::string p1, std::string p2); //position: {0,1,2,3}, rand: {0,1}
	static std::string simplePowerAlternate(int position, int rand, std::string p1, std::string p2); //position: {0,1}, rand: {0,1}

	static PositionHolder getSymmetricPowerupPositions_LR(int position, double x_center, double y_center, double x_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_UD(int position, double x_center, double y_center, double y_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1,2,3}
	static PositionHolder getSymmetricPowerupPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1}
	static PositionHolder getSymmetricPowerupPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset); //position: {0,1}
	//about _DiagForwardSlash and _DiagBackwardSlash: it's half of _Corners; uses the diagonal two that go in the direction of a forward slash (/) or backslash (\)

	static PositionHolder getSymmetricWallPositions_LR(int position, double x_center, double y_center, double x_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_UD(int position, double x_center, double y_center, double y_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1,2,3}
	static PositionHolder getSymmetricWallPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1}
	static PositionHolder getSymmetricWallPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight); //position: {0,1}

private:
	LevelHelper() = delete;
	LevelHelper(const LevelHelper&) = delete;
};
