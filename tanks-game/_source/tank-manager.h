#pragma once
#include <vector>
#include "tank.h"

class TankManager {
	friend class ResetThings;

private:
	static std::vector<Tank*> tanks;
	static void clearTanks(); //for ResetThings

public:
	static void initialize();
	static Tank* getTank(unsigned int index);
	static Tank* getTankByID(Game_ID);
	static void pushTank(Tank*);
	static unsigned int getNumTanks() { return tanks.size(); }

private:
	TankManager() = delete;
	TankManager(const TankManager&) = delete;
};
