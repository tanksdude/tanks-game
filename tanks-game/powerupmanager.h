#pragma once
#include "powersquare.h"
#include <string>
#include <vector>
#include <unordered_map>

typedef Power* (*PowerFunction)(void);

class PowerupManager { //technically "PowerSquareManager"
	friend class ResetThings;
private:
	static std::vector<PowerSquare*> powerups; //active powersquares
	static void clearPowerups(); //for ResetThings

	static std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> powerLookup;
	static std::unordered_map<std::string, std::vector<PowerFunction>> powerList;
	static std::unordered_map<std::string, std::vector<std::string>> powerNameList;
public:
	static void initialize();
	static PowerSquare* getPowerup(int index);
	static void pushPowerup(PowerSquare*);
	static int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(int index);

	static void addPowerFactory(PowerFunction);
	static PowerFunction getPowerFactory(std::string name);
	static std::string getPowerName(int index);
	static int getNumPowerTypes();

	static void addSpecialPowerFactory(std::string type, PowerFunction);
	static PowerFunction getSpecialPowerFactory(std::string type, std::string name);
	static std::string getSpecialPowerName(std::string type, int index);
	static int getNumSpecialPowerTypes(std::string type);
};
