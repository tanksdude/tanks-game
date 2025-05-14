#pragma once
#include "game-main-loop.h"

class ResetThings {
public:
	static const int default_tankStartingYCount;
	static const double default_tankToEdgeDist;
	static const double default_tankStartingYRange;

public:
	static void reset();
	static void firstReset(); //temporary?
	static void firstGameInitialize(std::string tank1TeamName, std::string tank2TeamName, GameMainLoop& gameInstance); //pushes the first tanks and team watchings
	static void firstLevelPush(); //chosen from INI file

	static void tankPositionReset(); //use this one
	static void tankPositionReset(double x); //distance from edge
	static void tankPositionReset(double x, double y); //exact position of left tank; right one is mirrored
	static void tankPositionReset(double x, double yRange, int yCount);

private:
	ResetThings() = delete;
	ResetThings(const ResetThings&) = delete;
};
