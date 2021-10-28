#pragma once
#include "tank.h"
#include <vector>

class TankManager {
	friend class ResetThings;

private:
	static std::vector<Tank*> tanks;
	//static void clearTanks(); //for ResetThings (shouldn't be used, though (at least for now))

public:
	static void initialize();
	static Tank* getTank(int index);
	static void pushTank(Tank*);
	static int getNumTanks() { return tanks.size(); }
	//static void deleteTank(int index);

private:
	TankManager() {}
	TankManager(const TankManager&) {}
};
