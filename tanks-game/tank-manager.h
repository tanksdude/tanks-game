#pragma once
#include "tank.h"
#include <vector>

class TankManager {
	friend class ResetThings;

private:
	static std::vector<Tank*> tanks;
	static void clearTanks(); //for ResetThings

public:
	static void initialize();
	static Tank* getTank(int index);
	static Tank* getTankByID(Game_ID);
	static void pushTank(Tank*);
	static int getNumTanks() { return tanks.size(); }
	//static void deleteTank(int index);
	//static void deleteTankByID(Game_ID);

private:
	TankManager() {}
	TankManager(const TankManager&) {}
};
