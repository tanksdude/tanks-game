#pragma once
#include "powersquare.h"
#include <string>
#include <vector>
#include <unordered_map>

typedef Power* (*PowerFunction)(void);

class PowerupManager { //manager of PowerSquares and Powers
	friend class ResetThings;

private:
	static std::vector<PowerSquare*> powerups; //active powersquares
	static void clearPowerups(); //for ResetThings

	static std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> powerLookup;
	static std::unordered_map<std::string, std::vector<std::string>> powerNameList;

public:
	static void initialize();
	static PowerSquare* getPowerup(int index);
	static void pushPowerup(PowerSquare*);
	static int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(int index);

	static void addPowerFactory(PowerFunction);
	static PowerFunction getPowerFactory(std::string type, std::string name);
	static std::string getPowerName(std::string type, int index);
	static int getNumPowerTypes(std::string type);

private:
	PowerupManager() {}
	PowerupManager(const PowerupManager&) {}
};
