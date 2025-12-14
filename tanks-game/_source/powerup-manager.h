#pragma once
#include <vector>
#include "power-square.h"

class PowerupManager { //manages PowerSquares
	friend class ResetThings;

private:
	static std::vector<PowerSquare*> powerups; //active powersquares
	static void clearPowerups(); //for ResetThings

public:
	static void initialize();
	static PowerSquare* getPowerup(unsigned int index);
	static PowerSquare* getPowerupByID(Game_ID);
	static void pushPowerup(PowerSquare*);
	static unsigned int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(unsigned int index);
	static void deletePowerupByID(Game_ID);

private:
	PowerupManager() = delete;
	PowerupManager(const PowerupManager&) = delete;
};
