#pragma once
#include "powersquare.h"
#include <vector>

class PowerupManager { //technically "PowerSquareManager"
	friend class ResetThings;
private:
	static std::vector<PowerSquare*> powerups;
public:
	static void initialize();
	static PowerSquare* const getPowerup(int index);
	//see BulletManager explanation
	static void pushPowerup(PowerSquare*);
	static int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(int index);
};