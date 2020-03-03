#pragma once
#include "powersquare.h"
#include <string>
#include <vector>

typedef Power* (*PowerFunction)(void);

class PowerupManager { //technically "PowerSquareManager"
	friend class ResetThings;
private:
	static std::vector<PowerSquare*> powerups; //active powersquares

	static std::unordered_map<std::string, PowerFunction> powerLookup;
	static std::vector<PowerFunction> powerList;
	static std::vector<std::string> powerNameList;
public:
	static void initialize();
	static PowerSquare* const getPowerup(int index);
	//see BulletManager explanation
	static void pushPowerup(PowerSquare*);
	static int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(int index);

	static void addPowerFactory(PowerFunction);
	static PowerFunction getPowerFactory(std::string name);
	static std::string getPowerName(int index);
	static int getNumPowerTypes();
};