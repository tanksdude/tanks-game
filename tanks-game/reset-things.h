#pragma once
#include "tank.h"
#include "game-main-loop.h"

class ResetThings {
public:
	static const double default_tankToEdgeDist;

public:
	static void reset() { ResetThings::reset(0); }
	static void reset(int);
	static void firstReset(); //temporary?
	static void firstGameInitialize(std::string tank1TeamName, std::string tank2TeamName, GameMainLoop& gameInstance); //pushes the first tanks and team watchings
	static void firstLevelPush(); //chosen from INI file

	static void tankPositionReset(Tank* first, Tank* second); //use this one
	static void tankPositionReset(Tank* first, Tank* second, double x); //distance from edge
	static void tankPositionReset(Tank* first, Tank* second, double x, double y); //x and y are first's position; second is opposite first

private:
	ResetThings() {}
	ResetThings(const ResetThings&) {}
};
