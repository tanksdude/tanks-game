#pragma once
#include "power-square.h"
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

	static std::vector<std::string> protectedTypes; //types not allowed to be used (by custom powers)

public:
	static void initialize();
	static PowerSquare* getPowerup(int index);
	static PowerSquare* getPowerupByID(Game_ID);
	static void pushPowerup(PowerSquare*);
	static unsigned int getNumPowerups() { return powerups.size(); }
	static void deletePowerup(int index);
	static void deletePowerupByID(Game_ID);

	static std::vector<Rect*> getPowerupCollisionList();

	static void addPowerFactory(PowerFunction);
	static PowerFunction getPowerFactory(std::string type, std::string name);
	static std::string getPowerName(std::string type, unsigned int index);
	static unsigned int getNumPowerTypes(std::string type);

	static std::string checkCustomPowerTypesAgainstProtectedTypes(const std::vector<std::string>&) noexcept; //returns first bad type ("" if all good)

private:
	PowerupManager() = delete;
	PowerupManager(const PowerupManager&) = delete;
};
