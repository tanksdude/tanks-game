#pragma once
#include "tank.h"
#include <vector>

class TankManager { //pretty much no point in having a manager for the tanks (unless singleplayer is a thing, or a main menu)
	friend class ResetThings;
private:
	static std::vector<Tank*> tanks;
public:
	static void initialize();
	static Tank* const getTank(int index);
	//see BulletManager explanation
	static void pushTank(Tank*);
	static int getNumTanks() { return tanks.size(); }
	//static void deleteTank(int index);
};
