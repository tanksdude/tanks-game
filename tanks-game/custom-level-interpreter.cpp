#include "custom-level-interpreter.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map> //could use a set, not a big deal
#include "level-manager.h"
#include "basic-ini-parser.h" //for processing custom level files
#include <algorithm> //std::find

#include "reset-things.h"
#include "tank-manager.h"
#include "wall-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "rng.h"
#include "level-helper.h"

const std::string CustomLevelInterpreter::ModOrderPath = "mods/order.txt";
const std::string CustomLevelInterpreter::IgnoreListPath = "mods/ignore.txt";
const std::string CustomLevelInterpreter::ActionStartPhrase = "[LEVEL_START]";
std::unordered_map<std::string, std::unordered_map<std::string, CustomLevel*>> CustomLevelInterpreter::customLevelLookup;

CustomLevel::CustomLevelAction::CustomLevelAction(CustomLevel::CustomLevelCommands c) {
	this->command = c;
	this->data = GenericFactoryConstructionData();
}

CustomLevel::CustomLevel(std::string name,
float colorR, float colorG, float colorB,
int startPosCount, double startPosXValue, double startPosYRange,
const std::vector<std::string>& types,
const std::unordered_map<std::string, float>& weights,
std::vector<CustomLevelAction*> actions) {

	this->name = name;
	this->defaultColor = ColorValueHolder(colorR, colorG, colorB);
	this->levelTypes = std::vector<std::string>(types);
	this->levelWeights = std::unordered_map<std::string, float>(weights);

	this->startPosCount  = (startPosCount  < 1 ? 5                      : startPosCount);
	this->startPosXValue = (startPosXValue < 0 ? 20                     : startPosXValue);
	this->startPosYRange = (startPosYRange < 0 ? (8.0/10 * GAME_HEIGHT) : startPosYRange);
	this->initializationActions = std::vector<CustomLevelAction*>(actions);
}

CustomLevel::~CustomLevel() {
	//TODO: need to delete all the stuff in the GenericFactoryConstructionData

	for (int i = 0; i < initializationActions.size(); i++) {
		delete initializationActions[i];
	}
	//initializationActions.clear();
}

CustomLevel::CustomLevelCommands CustomLevel::strToCommand(const std::string& str) noexcept {
	//could make a hashmap and lookup into that (assuming hashmaps are smart enough to treat enums as a number)

	CustomLevel::CustomLevelCommands levelCommand;

	if (str == "WALL") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL;
	} else if (str == "WALL_LR") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL_LR;
	} else if (str == "WALL_UD") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL_UD;
	} else if (str == "WALL_Corners") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL_Corners;
	} else if (str == "WALL_DiagForwardSlash") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL_DiagForwardSlash;
	} else if (str == "WALL_DiagBackwardSlash") {
		levelCommand = CustomLevel::CustomLevelCommands::WALL_DiagBackwardSlash;
	}
	else if (str == "POWER") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER;
	} else if (str == "POWER_LR") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER_LR;
	} else if (str == "POWER_UD") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER_UD;
	} else if (str == "POWER_Corners") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER_Corners;
	} else if (str == "POWER_DiagForwardSlash") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER_DiagForwardSlash;
	} else if (str == "POWER_DiagBackwardSlash") {
		levelCommand = CustomLevel::CustomLevelCommands::POWER_DiagBackwardSlash;
	}
	else if (str == "CHAZARD") {
		levelCommand = CustomLevel::CustomLevelCommands::CHAZARD;
	} else if (str == "RHAZARD") {
		levelCommand = CustomLevel::CustomLevelCommands::RHAZARD;
	}
	else {
		levelCommand = CustomLevel::CustomLevelCommands::UNKNOWN;
	}

	return levelCommand;
}

void CustomLevel::initialize() {
	int randNum = RNG::randFunc() * startPosCount;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), startPosXValue, randNum * (startPosYRange/startPosCount) + (startPosYRange/(2*startPosCount)));
	//TODO: change this to no longer require TankManager or RNG

	ColorValueHolder color = getDefaultColor();

	for (int i = 0; i < initializationActions.size(); i++) {
		GenericFactoryConstructionData& data = initializationActions[i]->data;

		switch (initializationActions[i]->command) {
			case CustomLevelCommands::WALL:
				initialization_WALL(data, color);
				break;
			case CustomLevelCommands::WALL_LR:
				initialization_WALL_LR(data, color);
				break;
			case CustomLevelCommands::WALL_UD:
				initialization_WALL_UD(data, color);
				break;
			case CustomLevelCommands::WALL_Corners:
				initialization_WALL_Corners(data, color);
				break;
			case CustomLevelCommands::WALL_DiagForwardSlash:
				initialization_WALL_DiagForwardSlash(data, color);
				break;
			case CustomLevelCommands::WALL_DiagBackwardSlash:
				initialization_WALL_DiagBackwardSlash(data, color);
				break;

			case CustomLevelCommands::POWER:
				initialization_POWER(data);
				break;
			case CustomLevelCommands::POWER_LR:
				initialization_POWER_LR(data);
				break;
			case CustomLevelCommands::POWER_UD:
				initialization_POWER_UD(data);
				break;
			case CustomLevelCommands::POWER_Corners:
				initialization_POWER_Corners(data);
				break;
			case CustomLevelCommands::POWER_DiagForwardSlash:
				initialization_POWER_DiagForwardSlash(data);
				break;
			case CustomLevelCommands::POWER_DiagBackwardSlash:
				initialization_POWER_DiagBackwardSlash(data);
				break;

			case CustomLevelCommands::CHAZARD:
				try {
					initialization_CHAZARD(data);
				}
				//catch (const std::exception& e) {
				catch (const std::runtime_error& e) {
					std::cerr << "Error initializing circle hazard: " << e.what() << std::endl;
				}
				break;
			case CustomLevelCommands::RHAZARD:
				try {
					initialization_RHAZARD(data);
				}
				//catch (const std::exception& e) {
				catch (const std::runtime_error& e) {
					std::cerr << "Error initializing rect hazard: " << e.what() << std::endl;
				}
				break;

			default:
				std::cerr << "Unknown CustomLevelCommand! (this shouldn't happen because only valid commands should be in this CustomLevel's list) " + this->getName() << std::endl;
				break;
		}
	}
}

inline void CustomLevel::initialization_WALL(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double width   = dataArr[2];
	double height  = dataArr[3];

	WallManager::pushWall(new Wall(centerX-width/2, centerY-height/2, width, height, wallColor));
}
inline void CustomLevel::initialization_WALL_LR(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double offsetX = dataArr[2];
	double width   = dataArr[3];
	double height  = dataArr[4];
	PositionHolder pos;

	pos = LevelHelper::getSymmetricWallPositions_LR(0, centerX, centerY, offsetX, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
	pos = LevelHelper::getSymmetricWallPositions_LR(1, centerX, centerY, offsetX, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
}
inline void CustomLevel::initialization_WALL_UD(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double offsetY = dataArr[2];
	double width   = dataArr[3];
	double height  = dataArr[4];
	PositionHolder pos;

	pos = LevelHelper::getSymmetricWallPositions_UD(0, centerX, centerY, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
	pos = LevelHelper::getSymmetricWallPositions_UD(1, centerX, centerY, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
}
inline void CustomLevel::initialization_WALL_Corners(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double offsetX = dataArr[2];
	double offsetY = dataArr[3];
	double width   = dataArr[4];
	double height  = dataArr[5];
	PositionHolder pos;

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, centerX, centerY, offsetX, offsetY, width, height);
		WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
	}
}
inline void CustomLevel::initialization_WALL_DiagForwardSlash(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double offsetX = dataArr[2];
	double offsetY = dataArr[3];
	double width   = dataArr[4];
	double height  = dataArr[5];
	PositionHolder pos;

	pos = LevelHelper::getSymmetricWallPositions_DiagForwardSlash(0, centerX, centerY, offsetX, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
	pos = LevelHelper::getSymmetricWallPositions_DiagForwardSlash(1, centerX, centerY, offsetX, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
}
inline void CustomLevel::initialization_WALL_DiagBackwardSlash(const GenericFactoryConstructionData& data, ColorValueHolder wallColor) noexcept {
	double* dataArr = (double*)data.getDataPortion(0);
	double centerX = dataArr[0];
	double centerY = dataArr[1];
	double offsetX = dataArr[2];
	double offsetY = dataArr[3];
	double width   = dataArr[4];
	double height  = dataArr[5];
	PositionHolder pos;

	pos = LevelHelper::getSymmetricWallPositions_DiagBackwardSlash(0, centerX, centerY, offsetX, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
	pos = LevelHelper::getSymmetricWallPositions_DiagBackwardSlash(1, centerX, centerY, offsetX, offsetY, width, height);
	WallManager::pushWall(new Wall(pos.x, pos.y, width, height, wallColor));
}

inline void CustomLevel::initialization_POWER(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double xpos = posArr[0];
	double ypos = posArr[1];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);

	PowerupManager::pushPowerup(new PowerSquare(xpos, ypos, typesArr, namesArr, data.getDataPortionLength(1)));
}
inline void CustomLevel::initialization_POWER_LR(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double centerX = posArr[0];
	double centerY = posArr[1];
	double offsetX = posArr[2];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);
	PositionHolder pos;

	pos = LevelHelper::getSymmetricPowerupPositions_LR(0, centerX, centerY, offsetX);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
	pos = LevelHelper::getSymmetricPowerupPositions_LR(1, centerX, centerY, offsetX);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
}
inline void CustomLevel::initialization_POWER_UD(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double centerX = posArr[0];
	double centerY = posArr[1];
	double offsetY = posArr[2];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);
	PositionHolder pos;

	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, centerX, centerY, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, centerX, centerY, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
}
inline void CustomLevel::initialization_POWER_Corners(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double centerX = posArr[0];
	double centerY = posArr[1];
	double offsetX = posArr[2];
	double offsetY = posArr[3];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);
	PositionHolder pos;

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, centerX, centerY, offsetX, offsetY);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
	}
}
inline void CustomLevel::initialization_POWER_DiagForwardSlash(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double centerX = posArr[0];
	double centerY = posArr[1];
	double offsetX = posArr[2];
	double offsetY = posArr[3];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);
	PositionHolder pos;

	pos = LevelHelper::getSymmetricPowerupPositions_DiagForwardSlash(0, centerX, centerY, offsetX, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
	pos = LevelHelper::getSymmetricPowerupPositions_DiagForwardSlash(1, centerX, centerY, offsetX, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
}
inline void CustomLevel::initialization_POWER_DiagBackwardSlash(const GenericFactoryConstructionData& data) noexcept {
	double* posArr = (double*)data.getDataPortion(0);
	double centerX = posArr[0];
	double centerY = posArr[1];
	double offsetX = posArr[2];
	double offsetY = posArr[3];

	std::string* typesArr = (std::string*)data.getDataPortion(1);
	std::string* namesArr = (std::string*)data.getDataPortion(2);
	PositionHolder pos;

	pos = LevelHelper::getSymmetricPowerupPositions_DiagBackwardSlash(0, centerX, centerY, offsetX, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
	pos = LevelHelper::getSymmetricPowerupPositions_DiagBackwardSlash(1, centerX, centerY, offsetX, offsetY);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, typesArr, namesArr, data.getDataPortionLength(1)));
}

inline void CustomLevel::initialization_CHAZARD(const GenericFactoryConstructionData& data) {
	//should only throw on complex constructions, if the data is badly-formatted
	std::string* nameArr = (std::string*)data.getDataPortion(0);
	GenericFactoryConstructionData realData; //TODO
	realData.pushData(data.getDataPortionLength(1), data.getDataPortion(1));
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory(nameArr[0], nameArr[1])(realData));
}
inline void CustomLevel::initialization_RHAZARD(const GenericFactoryConstructionData& data) {
	//should only throw on complex constructions, if the data is badly-formatted
	std::string* nameArr = (std::string*)data.getDataPortion(0);
	GenericFactoryConstructionData realData; //TODO
	realData.pushData(data.getDataPortionLength(1), data.getDataPortion(1));
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory(nameArr[0], nameArr[1])(realData));
}

CustomLevel* CustomLevel::factory() const {
	return new CustomLevel(name,
		defaultColor.getRf(), defaultColor.getGf(), defaultColor.getBf(),
		startPosCount, startPosXValue, startPosYRange,
		levelTypes, levelWeights, initializationActions);
}



void CustomLevelInterpreter::ProcessCustomLevels() noexcept {
	//get list of mods, interpret them, then add factories to LevelManager
	//should syntax version be a thing? likely features will only be added, but who knows

	std::vector<std::string> modOrder_list;
	std::unordered_map<std::string, bool> modOrder_set;

	try {
		std::vector<std::string> knownList = getListOfKnownMods();
		for (int i = 0; i < knownList.size(); i++) {
			if (modOrder_set.find(knownList[i]) == modOrder_set.end()) {
				modOrder_set.insert({ knownList[i], true });
				modOrder_list.push_back(knownList[i]);
			} else { [[unlikely]]
				std::cerr << "Item duplicated in " + ModOrderPath + ":" + std::to_string(i) + ", ignoring (" + knownList[i] + ")" << std::endl;
			}
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "CustomLevelInterpreter::getListOfUnknownMods() error: " << e.what() << std::endl;
		//return; //keep going
	}
	/*
	catch (const std::exception& e) {
		std::cerr << "CustomLevelInterpreter::getListOfKnownMods() Unexpected error: " << e.what() << std::endl;
		return;
	}
	*/

	try {
		std::vector<std::string> unknownList = getListOfUnknownMods();
		for (int i = 0; i < unknownList.size(); i++) {
			if (modOrder_set.find(unknownList[i]) == modOrder_set.end()) {
				modOrder_set.insert({ unknownList[i], true });
				modOrder_list.push_back(unknownList[i]);
			} else {
				//std::cerr << "Item already in " + ModOrderPath + ":" + std::to_string(i) + ", ignoring (" + unknownList[i] + ")" << std::endl;
			}
		}
		//originally, this was going to push the "new" mods to the order.txt file, but that doesn't feel right
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "CustomLevelInterpreter::getListOfUnknownMods() filesystem_error: " << e.what() << std::endl;
		return; //likely some major problem, so quit
	}
	/*
	catch (const std::exception& e) {
		std::cerr << "CustomLevelInterpreter::getListOfUnknownMods() Unexpected error: " << e.what() << std::endl;
		return;
	}
	*/

	try {
		std::vector<std::string> ignoreList = getListOfIgnoredMods();
		for (int i = 0; i < ignoreList.size(); i++) {
			if (modOrder_set.find(ignoreList[i]) != modOrder_set.end()) {
				modOrder_set.erase(ignoreList[i]);
				modOrder_list.erase(std::find(modOrder_list.begin(), modOrder_list.end(), ignoreList[i]));
			} else {
				std::cerr << "Item not found in " + IgnoreListPath + ":" + std::to_string(i) + ", skipping (" + ignoreList[i] + ")" << std::endl;
			}
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "CustomLevelInterpreter::getListOfIgnoredMods() error: " << e.what() << std::endl;
		//return; //keep going
	}
	/*
	catch (const std::exception& e) {
		std::cerr << "CustomLevelInterpreter::getListOfIgnoredMods() Unexpected error: " << e.what() << std::endl;
		return;
	}
	*/

	for (int i = 0; i < modOrder_list.size(); i++) {
		std::vector<std::string> levelOrder_list;
		try {
			levelOrder_list = getListOfLevels("mods/" + modOrder_list[i] + "/levels");
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "CustomLevelInterpreter::getListOfLevels(" + ("mods/" + modOrder_list[i] + "/levels") + ") filesystem_error: " << e.what() << std::endl;
			continue; //hope things work out on the next mod
		}

		for (int j = 0; j < levelOrder_list.size(); j++) {
			//std::cout << "modOrder_list[" << i << "]: " << "levelOrder_list[" << j << "]: mods/" + modOrder_list[i] + "/levels/" << levelOrder_list[j] << std::endl;
			std::string levelPath = "mods/" + modOrder_list[i] + "/levels/" + levelOrder_list[j];
			CustomLevel* level;
			try {
				level = processCustomLevel(levelPath);
			}
			catch (const std::runtime_error& e) {
				std::cerr << "ERROR: Unable to process " + levelPath + ", reason: " << e.what() << std::endl;
				continue;
			}
			/*
			catch (const std::exception& e) {
				std::cerr << "ERROR: Catastrophic error processing " + levelPath + ", message: " << e.what() << std::endl;
				continue;
			}
			*/

			if (level == nullptr) { [[unlikely]]
				//std::cerr << "WARNING: Bad path \"" + levelPath + "\"" << std::endl;
				continue;
			} else {
				try {
					pushCustomLevel(level);
				}
				catch (const std::runtime_error& e) {
					std::cerr << "ERROR: Unable to materialize " + levelPath + ", reason: " << e.what() << std::endl;
					delete level;
					continue;
				}
				/*
				catch (const std::exception& e) {
					std::cerr << "ERROR: Catastrophic error materializing " + levelPath + ", message: " << e.what() << std::endl;
					delete level;
					continue;
				}
				*/
			}

			std::cout << "Processed custom level: " + levelPath << std::endl;
		}
	}

	std::cout << std::endl;
}

std::vector<std::string> CustomLevelInterpreter::getListOfKnownMods() {
	std::ifstream modOrder_file;
	modOrder_file.open(ModOrderPath);

	std::vector<std::string> modOrder_list;

	if (modOrder_file.is_open()) {
		std::string line;
		while (std::getline(modOrder_file, line)) {
			modOrder_list.push_back(line);
		}
		modOrder_file.close();
	} else {
		throw std::runtime_error("Could not read file \"" + ModOrderPath + "\"");
	}

	return modOrder_list;
}

std::vector<std::string> CustomLevelInterpreter::getListOfUnknownMods() {
	std::vector<std::string> directory_list;

	for (const auto& dir_entry : std::filesystem::directory_iterator("mods")) {
		if (std::filesystem::is_directory(dir_entry)) {
			directory_list.push_back(dir_entry.path().string());
			//this still has "mod\" in the filepath
		}
		//can throw std::filesystem::filesystem_error
	}

	//scrub parent folders from directory
	for (std::string& s : directory_list) {
		size_t slashPos = s.find_last_of("/\\");
		if (slashPos == std::string::npos) {
			//is this possible?
		} else {
			s = s.substr(slashPos+1);
		}
	}

	return directory_list;
}

std::vector<std::string> CustomLevelInterpreter::getListOfIgnoredMods() {
	//copied from getListOfKnownMods()
	std::ifstream ignoreList_file;
	ignoreList_file.open(IgnoreListPath);

	std::vector<std::string> ignoreList_list;

	if (ignoreList_file.is_open()) {
		std::string line;
		while (std::getline(ignoreList_file, line)) {
			ignoreList_list.push_back(line);
		}
		ignoreList_file.close();
	} else {
		throw std::runtime_error("Could not read file \"" + IgnoreListPath + "\"");
	}

	return ignoreList_list;
}

std::vector<std::string> CustomLevelInterpreter::getListOfLevels(std::string modPath) {
	//basically the same as getListOfUnknownMods()
	std::vector<std::string> file_list;

	for (const auto& dir_entry : std::filesystem::directory_iterator(modPath)) {
		if (std::filesystem::is_regular_file(dir_entry)) {
			file_list.push_back(dir_entry.path().string());
		}
	}

	//scrub parent folders from directory
	for (std::string& s : file_list) {
		size_t slashPos = s.find_last_of("/\\");
		if (slashPos == std::string::npos) {
			//is this possible?
		} else {
			s = s.substr(slashPos+1);
		}
	}

	return file_list;
}

CustomLevel* CustomLevelInterpreter::processCustomLevel(std::string path) {
	if (path == "") { [[unlikely]]
		return nullptr;
	}

	std::string name;
	float colorR, colorG, colorB;
	int startPosCount = -1;
	double startPosXValue = -1, startPosYRange = -1;
	std::vector<std::string> types;
	std::unordered_map<std::string, float> weights;
	std::vector<CustomLevel::CustomLevelAction*> actions;

	bool name_set = false;
	bool color_set = false;
	//bool startPosCount_set = false;
	//bool startPosXValue_set = false, startPosYRange_set = false;
	bool types_set = false;
	bool weights_set = false;

	std::vector<std::string> temp_weights; //set when reading the file
	int temp_weights_lineNum;

	bool levelAction_started = false;
	std::vector<std::pair<std::string, int>> levelAction_list;

	std::ifstream level_file;
	level_file.open(path);

	if (level_file.is_open()) {
		std::string line;
		int lineNum = 0;
		std::string error_string = ""; //when an exception is generated, this is set and the loop below is broken out of (so the file can get closed)

		while (std::getline(level_file, line)) {
			lineNum++;
			BasicINIParser::removeLeftWhitespace(line);
			BasicINIParser::removeComments(line);
			BasicINIParser::removeRightWhitespace(line);
			if (line.size() == 0) {
				continue;
			}

			if (line == ActionStartPhrase) { [[unlikely]]
				levelAction_started = true;
				continue;
			}

			if (!levelAction_started) { //assignment
				//read the same way as an INI file: split on the '=', left is name, right is values
				//values get split on spaces (just like INI files)
				//code copied from BasicINIParser

				int separator_index = BasicINIParser::findSeparatorIndex(line);
				if (separator_index == -1) { [[unlikely]]
					error_string = "Syntax error on line " + std::to_string(lineNum) + ": no '='";
					break;
				} else {
					std::string assignmentName = line.substr(0, separator_index);
					std::string assignmentValues = line.substr(separator_index+1);
					BasicINIParser::removeRightWhitespace(assignmentName);
					BasicINIParser::processEscapeSequences_most(assignmentValues);

					std::vector<std::string> assignmentValues_list;
					while (assignmentValues.size() != 0) {
						BasicINIParser::removeLeftWhitespace(assignmentValues);
						int substr_end = BasicINIParser::findSubstringEndIndex(assignmentValues);
						if (assignmentValues[0] == '\"') {
							assignmentValues_list.push_back(assignmentValues.substr(1, substr_end-1));
							assignmentValues.erase(0, substr_end+1);
						} else {
							assignmentValues_list.push_back(assignmentValues.substr(0, substr_end));
							assignmentValues.erase(0, substr_end);
						}
						BasicINIParser::processEscapeSequences_all(assignmentValues_list[assignmentValues_list.size()-1]);
					}

					if (assignmentValues_list.size() == 0) { [[unlikely]]
						error_string = "Syntax error on line " + std::to_string(lineNum) + ": no data given for \"" + assignmentName + "\"";
						break;
					}

					if (assignmentName == "Name") {
						name = assignmentValues_list[0];
						name_set = true;
					}
					else if (assignmentName == "Color") {
						if (assignmentValues_list.size() != 3) { [[unlikely]]
							error_string = "Syntax error on line " + std::to_string(lineNum) + ": expected 3 data items for \"Color\" but got " + std::to_string(assignmentValues_list.size());
							break;
						}
						try {
							colorR = std::stof(assignmentValues_list[0]);
							colorG = std::stof(assignmentValues_list[1]);
							colorB = std::stof(assignmentValues_list[2]);
						}
						catch (const std::exception&) {
							//std::invalid_argument (and std::out_of_range)
							error_string = "Syntax error on line " + std::to_string(lineNum) + ": unable to parse \"" + assignmentName + "\" values";
							break;
						}
						color_set = true;
					}
					else if (assignmentName == "LevelTypes") {
						types = assignmentValues_list;
						types_set = true;
					}
					else if (assignmentName == "LevelWeights") {
						temp_weights = assignmentValues_list;
						temp_weights_lineNum = lineNum;
						weights_set = true;
					}
					else if (assignmentName == "RandomStartPositionCount") {
						try {
							startPosCount = std::stoi(assignmentValues_list[0]);
						}
						catch (const std::exception&) {
							error_string = "Syntax error on line " + std::to_string(lineNum) + ": unable to parse \"" + assignmentName + "\" values";
							break;
						}
						//startPosCount_set = true;
					}
					else if (assignmentName == "RandomStartPositionXValue") {
						try {
							startPosXValue = std::stod(assignmentValues_list[0]);
						}
						catch (const std::exception&) {
							error_string = "Syntax error on line " + std::to_string(lineNum) + ": unable to parse \"" + assignmentName + "\" values";
							break;
						}
						//startPosXValue_set = true;
					}
					else if (assignmentName == "RandomStartPositionYRange") {
						try {
							startPosYRange = std::stod(assignmentValues_list[0]);
						}
						catch (const std::exception&) {
							error_string = "Syntax error on line " + std::to_string(lineNum) + ": unable to parse \"" + assignmentName + "\" values";
							break;
						}
						//startPosYRange_set = true;
					}
					else {
						//error_string = "Syntax error on line " + std::to_string(lineNum) + ": unknown assignment \"" + assignmentName + "\"";
						//break;
						std::cerr << "Warning parsing level " + path + ": line " + std::to_string(lineNum) + ": unknown assignment \"" + assignmentName + "\"" << std::endl;
					}
				}

			} else { //command
				//just push the line to a list to be processed later
				levelAction_list.push_back({ line, lineNum });
			}
		}

		level_file.close();
		if (error_string != "") {
			throw std::runtime_error(error_string);
		}
	} else {
		throw std::runtime_error("Could not read file \"" + path + "\"");
	}

	if (!name_set) {
		throw std::runtime_error("Error parsing level: level name not set");
	}
	if (!color_set) {
		throw std::runtime_error("Error parsing level: level color not set");
	}
	if (!types_set) {
		throw std::runtime_error("Error parsing level: level types not set");
	}
	if (!weights_set) {
		throw std::runtime_error("Error parsing level: level weights not set");
	}

	if (types.size() == 0) {
		throw std::runtime_error("Error parsing level: no level types stated");
	}
	if (types.size() < temp_weights.size()) {
		throw std::runtime_error("Error parsing level: more level types than level weights");
	}
	if (types.size() != temp_weights.size()) {
		//std::cerr << "Warning parsing level " + path + ": more level weights than level types" << std::endl;
		throw std::runtime_error("Error parsing level: more level weights than level types");
		//TODO: throw or warn?
	}
	for (int i = 0; i < types.size(); i++) {
		float weight;
		try {
			weight = std::stof(temp_weights[i]);
		}
		catch (const std::exception&) {
			throw std::runtime_error("Syntax error on line " + std::to_string(temp_weights_lineNum) + ": unable to parse \"LevelWeights\" value[" + std::to_string(i) + "]: \"" + temp_weights[i] + "\"");
		}
		weights.insert({ types[i], weight });
	}

	for (int i = 0; i < levelAction_list.size(); i++) {
		try {
			actions.push_back(stringToAction(levelAction_list[i]));
			//TODO: need to clear memory
		}
		catch (const std::runtime_error& e) {
			//throw std::runtime_error("Error parsing level " + path + ": " + e.what()); //no error because left-to-right generates a string
			//throw std::runtime_error("Error parsing level: " + e.what()); //yes error because that's two const char*, can't add like a string
			throw std::runtime_error("Error parsing level: " + std::string(e.what()));
		}
	}

	return new CustomLevel(name,
		colorR, colorG, colorB,
		startPosCount, startPosXValue, startPosYRange,
		types, weights, actions);
}

inline CustomLevel::CustomLevelAction* CustomLevelInterpreter::stringToAction(const std::pair<std::string, int>& command) {
	std::string line = command.first;
	std::vector<std::string> words;
	while (line.size() != 0) {
		BasicINIParser::removeLeftWhitespace(line);
		int substr_end = BasicINIParser::findSubstringEndIndex(line);
		if (line[0] == '\"') {
			words.push_back(line.substr(1, substr_end-1));
			line.erase(0, substr_end+1);
		} else {
			words.push_back(line.substr(0, substr_end));
			line.erase(0, substr_end);
		}
		//BasicINIParser::processEscapeSequences_all(words[words.size()-1]);
	}

	if (words.size() <= 1) { [[unlikely]]
		throw std::runtime_error("line " + std::to_string(command.second) + " insufficient parameters");
	}

	CustomLevel::CustomLevelCommands levelCommand = CustomLevel::strToCommand(words[0]);
	std::string levelCommand_strBackup = words[0]; //for printing error messages
	words.erase(words.begin()); //remove the command type from the parameter list

	CustomLevel::CustomLevelAction* levelAction = new CustomLevel::CustomLevelAction(levelCommand);
	GenericFactoryConstructionData& constructionData = levelAction->data;

	try {
		switch (levelCommand) {
			case CustomLevel::CustomLevelCommands::WALL:
				stringToAction_WALL(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::WALL_LR:
				stringToAction_WALL_LR(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::WALL_UD:
				stringToAction_WALL_UD(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::WALL_Corners:
				stringToAction_WALL_Corners(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::WALL_DiagForwardSlash:
				stringToAction_WALL_DiagForwardSlash(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::WALL_DiagBackwardSlash:
				stringToAction_WALL_DiagBackwardSlash(words, constructionData);
				break;

			case CustomLevel::CustomLevelCommands::POWER:
				stringToAction_POWER(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::POWER_LR:
				stringToAction_POWER_LR(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::POWER_UD:
				stringToAction_POWER_UD(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::POWER_Corners:
				stringToAction_POWER_Corners(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::POWER_DiagForwardSlash:
				stringToAction_POWER_DiagForwardSlash(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::POWER_DiagBackwardSlash:
				stringToAction_POWER_DiagBackwardSlash(words, constructionData);
				break;

			case CustomLevel::CustomLevelCommands::CHAZARD:
				stringToAction_CHAZARD(words, constructionData);
				break;
			case CustomLevel::CustomLevelCommands::RHAZARD:
				stringToAction_RHAZARD(words, constructionData);
				break;

			default:
				throw std::runtime_error("Unknown level command!");
				//break;
		}
	}
	catch (const std::runtime_error& e) {
		throw std::runtime_error("line " + std::to_string(command.second) + " command \"" + levelCommand_strBackup + "\": " + e.what());
	}

	return levelAction;
}

void CustomLevelInterpreter::pushCustomLevel(CustomLevel* level) {
	std::vector<std::string> types = level->getLevelTypes();

	std::string result = LevelManager::checkCustomLevelTypesAgainstProtectedTypes(types);
	if (result != "") {
		throw std::runtime_error("Error pushing level " + level->getName() + ": level includes \"" + result + "\" type, which is not allowed");
	}

	for (int i = 0; i < types.size(); i++) {
		customLevelLookup[types[i]].insert({ level->getName(), level });
	}
	LevelManager::addCustomLevel(level->getName(), types);
}

Level* CustomLevelInterpreter::factory(std::string type, std::string name) {
	return customLevelLookup[type][name]->factory();
}

inline void CustomLevelInterpreter::stringToAction_WALL(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 4) {
		throw std::runtime_error("expected 4 data items but got " + std::to_string(words.size()));
	}

	double centerX, centerY;
	double width, height;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
		width   = std::stod(words[2]);
		height  = std::stod(words[3]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	constructionData.pushData(4, new double[4]{ centerX, centerY, width, height });
}
inline void CustomLevelInterpreter::stringToAction_WALL_LR(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 5) {
		throw std::runtime_error("expected 5 data items but got " + std::to_string(words.size()));
	}

	double centerX, centerY;
	double width, height;
	double offsetX;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
		offsetX = std::stod(words[2]);
		width   = std::stod(words[3]);
		height  = std::stod(words[4]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	constructionData.pushData(5, new double[5]{ centerX, centerY, offsetX, width, height });
}
inline void CustomLevelInterpreter::stringToAction_WALL_UD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_WALL_LR(words, constructionData);
}
inline void CustomLevelInterpreter::stringToAction_WALL_Corners(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 6) {
		throw std::runtime_error("expected 6 data items but got " + std::to_string(words.size()));
	}

	double centerX, centerY;
	double width, height;
	double offsetX, offsetY;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
		offsetX = std::stod(words[2]);
		offsetY = std::stod(words[3]);
		width   = std::stod(words[4]);
		height  = std::stod(words[5]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	constructionData.pushData(6, new double[6]{ centerX, centerY, offsetX, offsetY, width, height });
}
inline void CustomLevelInterpreter::stringToAction_WALL_DiagForwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_WALL_Corners(words, constructionData);
}
inline void CustomLevelInterpreter::stringToAction_WALL_DiagBackwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_WALL_Corners(words, constructionData);
}

inline void CustomLevelInterpreter::stringToAction_POWER(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 4) {
		throw std::runtime_error("expected 4 data items but got " + std::to_string(words.size()));
	}
	if (words.size() % 2 == 1) {
		throw std::runtime_error("expected an even number of data items");
	}

	double centerX, centerY;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	const unsigned int powerSize = (words.size()-2) / 2;
	std::string* typesArr = new std::string[powerSize];
	std::string* namesArr = new std::string[powerSize];

	for (unsigned int i = 0; i < powerSize; i++) {
		typesArr[i] = words[i*2   + 2];
		namesArr[i] = words[i*2+1 + 2];
	}

	unsigned int i;
	try {
		for (i = 0; i < powerSize; i++) {
			PowerupManager::getPowerFactory(typesArr[i], namesArr[i]);
		}
	}
	catch (const std::runtime_error&) {
		throw std::runtime_error("power \"" + typesArr[i] + "\" \"" + namesArr[i] + "\" does not exist");
	}

	constructionData.pushData(2, new double[2]{ centerX, centerY });
	constructionData.pushData(powerSize, typesArr);
	constructionData.pushData(powerSize, namesArr);
}
inline void CustomLevelInterpreter::stringToAction_POWER_LR(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 5) {
		throw std::runtime_error("expected 5 data items but got " + std::to_string(words.size()));
	}
	if (words.size() % 2 == 0) {
		throw std::runtime_error("expected an odd number of data items");
	}

	double centerX, centerY;
	double offsetX;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
		offsetX = std::stod(words[2]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	const unsigned int powerSize = (words.size()-3) / 2;
	std::string* typesArr = new std::string[powerSize];
	std::string* namesArr = new std::string[powerSize];

	for (unsigned int i = 0; i < powerSize; i++) {
		typesArr[i] = words[i*2   + 3];
		namesArr[i] = words[i*2+1 + 3];
	}

	unsigned int i;
	try {
		for (i = 0; i < powerSize; i++) {
			PowerupManager::getPowerFactory(typesArr[i], namesArr[i]);
		}
	}
	catch (const std::runtime_error&) {
		throw std::runtime_error("power \"" + typesArr[i] + "\" \"" + namesArr[i] + "\" does not exist");
	}

	constructionData.pushData(3, new double[3]{ centerX, centerY, offsetX });
	constructionData.pushData(powerSize, typesArr);
	constructionData.pushData(powerSize, namesArr);
}
inline void CustomLevelInterpreter::stringToAction_POWER_UD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_POWER_LR(words, constructionData);
}
inline void CustomLevelInterpreter::stringToAction_POWER_Corners(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 6) {
		throw std::runtime_error("expected 6 data items but got " + std::to_string(words.size()));
	}
	if (words.size() % 2 == 1) {
		throw std::runtime_error("expected an even number of data items");
	}

	double centerX, centerY;
	double offsetX, offsetY;
	try {
		centerX = std::stod(words[0]);
		centerY = std::stod(words[1]);
		offsetX = std::stod(words[2]);
		offsetY = std::stod(words[3]);
	}
	catch (const std::exception&) {
		throw std::runtime_error("unable to parse values");
	}

	const unsigned int powerSize = (words.size()-4) / 2;
	std::string* typesArr = new std::string[powerSize];
	std::string* namesArr = new std::string[powerSize];

	for (unsigned int i = 0; i < powerSize; i++) {
		typesArr[i] = words[i*2   + 4];
		namesArr[i] = words[i*2+1 + 4];
	}

	unsigned int i;
	try {
		for (i = 0; i < powerSize; i++) {
			PowerupManager::getPowerFactory(typesArr[i], namesArr[i]);
		}
	}
	catch (const std::runtime_error&) {
		throw std::runtime_error("power \"" + typesArr[i] + "\" \"" + namesArr[i] + "\" does not exist");
	}

	constructionData.pushData(4, new double[4]{ centerX, centerY, offsetX, offsetY });
	constructionData.pushData(powerSize, typesArr);
	constructionData.pushData(powerSize, namesArr);
}
inline void CustomLevelInterpreter::stringToAction_POWER_DiagForwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_POWER_Corners(words, constructionData);
}
inline void CustomLevelInterpreter::stringToAction_POWER_DiagBackwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	stringToAction_POWER_Corners(words, constructionData);
}

inline void CustomLevelInterpreter::stringToAction_CHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 2) {
		throw std::runtime_error("expected at least 2 data items but got " + std::to_string(words.size()));
	}

	try {
		HazardManager::getCircleHazardFactory(words[0], words[1]);
	}
	catch (const std::runtime_error&) {
		throw std::runtime_error("circle hazard \"" + words[0] + "\" \"" + words[1] + "\" does not exist");
	}

	CircleHazardFactoryGroup chazard_factoryGroup = HazardManager::getCircleHazardFactoryGroup(words[0], words[1]);
	if ((words.size()-2) < chazard_factoryGroup.getArgCount()) {
		throw std::runtime_error("expected " + std::to_string(chazard_factoryGroup.getArgCount()) + " parameters but got " + std::to_string(words.size()-2));
	}

	double centerX, centerY;
	double radius, angle; //angle in degrees
	double* chazard_parameters;
	switch (chazard_factoryGroup.getConstructionType()) {
		case CircleHazardConstructionTypes::constructionIsTooComplex:
			//TODO
			throw std::runtime_error("circle hazard construction type \"constructionIsTooComplex\" not being processed right now");
			break;

		case CircleHazardConstructionTypes::simpleConstruction:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			chazard_parameters = new double[2]{ centerX, centerY };
			break;

		case CircleHazardConstructionTypes::angleRequired:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
				angle   = std::stod(words[4]) * (PI/180);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			chazard_parameters = new double[3]{ centerX, centerY, angle };
			break;

		case CircleHazardConstructionTypes::radiusRequired:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
				radius  = std::stod(words[4]);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			chazard_parameters = new double[3]{ centerX, centerY, radius };
			break;

		case CircleHazardConstructionTypes::radiusAndAngleRequired:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
				radius  = std::stod(words[4]);
				angle   = std::stod(words[5]) * (PI/180);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			chazard_parameters = new double[4]{ centerX, centerY, radius, angle };
			break;

		default:
			throw std::runtime_error("unknown circle hazard construction type");
			//break;
	}

	constructionData.pushData(2, new std::string[2]{ words[0], words[1] });
	constructionData.pushData(chazard_factoryGroup.getArgCount(), chazard_parameters);
	//TODO: optional parameters
	//TODO: maybe the constructionIsTooComplex types can have their information separated by "", though the hazards still need a custom factory to handle it
}
inline void CustomLevelInterpreter::stringToAction_RHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData) {
	if (words.size() < 2) {
		throw std::runtime_error("expected at least 2 data items but got " + std::to_string(words.size()));
	}

	try {
		HazardManager::getRectHazardFactory(words[0], words[1]);
	}
	catch (const std::runtime_error&) {
		throw std::runtime_error("rect hazard \"" + words[0] + "\" \"" + words[1] + "\" does not exist");
	}

	RectHazardFactoryGroup rhazard_factoryGroup = HazardManager::getRectHazardFactoryGroup(words[0], words[1]);
	if ((words.size()-2) < rhazard_factoryGroup.getArgCount()) {
		throw std::runtime_error("expected " + std::to_string(rhazard_factoryGroup.getArgCount()) + " parameters but got " + std::to_string(words.size()-2));
	}

	double centerX, centerY;
	double width, height;
	double* rhazard_parameters;
	switch (rhazard_factoryGroup.getConstructionType()) {
		case RectHazardConstructionTypes::constructionIsTooComplex:
			//TODO
			throw std::runtime_error("rect hazard construction type \"constructionIsTooComplex\" not being processed right now");
			break;

		case RectHazardConstructionTypes::simpleConstruction:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			rhazard_parameters = new double[2]{ centerX, centerY };
			break;

		case RectHazardConstructionTypes::standardConstruction:
			try {
				centerX = std::stod(words[2]);
				centerY = std::stod(words[3]);
				width   = std::stod(words[4]);
				height  = std::stod(words[5]);
			}
			catch (const std::exception&) {
				throw std::runtime_error("unable to parse values");
			}
			rhazard_parameters = new double[4]{ centerX-width/2, centerY-height/2, width, height };
			break;

		default:
			throw std::runtime_error("unknown rect hazard construction type");
			//break;
	}

	constructionData.pushData(2, new std::string[2]{ words[0], words[1] });
	constructionData.pushData(rhazard_factoryGroup.getArgCount(), rhazard_parameters);
	//TODO: optional parameters
}
