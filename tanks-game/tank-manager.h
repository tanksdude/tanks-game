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
	//static void deleteTank(unsigned int index);
	//static void deleteTankByID(Game_ID);

	static std::vector<Circle*> getTankCollisionList();

private:
	TankManager() = delete;
	TankManager(const TankManager&) = delete;
};
