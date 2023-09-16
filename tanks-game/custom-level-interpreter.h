#pragma once
#include "level.h"
#include <string>
#include <vector>
#include <memory>
#include "generic-factory-construction-data.h"

//TODO: this should become a "ModInterpreter" (name permanently temporary), which gets a list of mods and sends the power/level/whatever to the relevant interpreter

class CustomLevel : public Level {
	friend class CustomLevelInterpreter;

public:
	enum class CustomLevelCommands {
		WALL,
		WALL_LR,
		WALL_UD,
		WALL_Corners,
		WALL_DiagForwardSlash,
		WALL_DiagBackwardSlash,

		RANDOM_WALLS,  //area to be placed in, count, width range, height range
		CLASSIC_WALLS, //no parameters

		POWER,
		POWER_LR,
		POWER_UD,
		POWER_Corners,
		//POWER_Corners_PowerAlternate, //TODO: other power alternates
		POWER_DiagForwardSlash,
		POWER_DiagBackwardSlash,

		CHAZARD,
		RHAZARD,

		UNKNOWN //error, don't use
	};
	static CustomLevelCommands strToCommand(const std::string&) noexcept; //UNKNOWN when can't parse

	struct CustomLevelAction {
		CustomLevelCommands command;
		GenericFactoryConstructionData data;
		CustomLevelAction(CustomLevelCommands command);
	};

protected:
	//for CustomLevelInterpreter
	CustomLevel(std::string name,
		float colorR, float colorG, float colorB,
		int startPosCount, double startPosXValue, double startPosYRange,
		const std::vector<std::string>& types,
		const std::unordered_map<std::string, float>& weights,
		std::vector<CustomLevelAction*>* actions);
	//for CustomLevel::factory()
	CustomLevel(std::string name,
		float colorR, float colorG, float colorB,
		int startPosCount, double startPosXValue, double startPosYRange,
		const std::vector<std::string>& types,
		const std::unordered_map<std::string, float>& weights,
		std::shared_ptr<std::vector<CustomLevelAction*>> actions);

protected:
	std::string name;
	ColorValueHolder defaultColor;
	std::vector<std::string> levelTypes;
	std::unordered_map<std::string, float> levelWeights;

	int startPosCount;
	double startPosXValue;
	double startPosYRange;
	std::shared_ptr<std::vector<CustomLevelAction*>> initializationActions; //remember, the vector's elements need to be deleted

protected:
	static inline void initialization_WALL(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_WALL_LR(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_WALL_UD(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_WALL_Corners(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_WALL_DiagForwardSlash(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_WALL_DiagBackwardSlash(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;

	static inline void initialization_RANDOM_WALLS(const GenericFactoryConstructionData&, ColorValueHolder wallColor) noexcept;
	static inline void initialization_CLASSIC_WALLS(ColorValueHolder wallColor) noexcept;

	static inline void initialization_POWER(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_LR(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_UD(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_Corners(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagForwardSlash(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagBackwardSlash(const GenericFactoryConstructionData&) noexcept;

	static inline void initialization_CHAZARD(const GenericFactoryConstructionData&);
	static inline void initialization_RHAZARD(const GenericFactoryConstructionData&);

public:
	//shouldn't be virtual
	std::string getName() const override { return this->name; }
	ColorValueHolder getDefaultColor() const override { return this->defaultColor; }
	std::vector<std::string> getLevelTypes() const override { return this->levelTypes; }
	std::unordered_map<std::string, float> getWeights() const override { return this->levelWeights; }

	virtual void initialize() override;
	//virtual void tick() override; //no, too complicated

	virtual ~CustomLevel();
	CustomLevel* factory() const; //makes a copy of itself; used by CustomLevelInterpreter to give LevelManager an actual level
};



class CustomLevelInterpreter {
public:
	static void ProcessCustomLevels() noexcept;
	static Level* factory(std::string type, std::string name); //TODO: basically need to rewrite how level factories are handled

protected:
	static const std::string ModOrderPath; // = "mods/order.txt";
	static const std::string IgnoreListPath; // = "mods/ignore.txt";
	static const std::string ActionStartPhrase; // = "[LEVEL_START]"; // (maybe "[BODY]" as a HTML reference? implies there should be a [HEAD] but that's implicit)

	static std::vector<std::string> getListOfKnownMods(); //from order.txt
	static std::vector<std::string> getListOfUnknownMods(); //from <filesystem>
	static std::vector<std::string> getListOfIgnoredMods(); //from ignore.txt
	static std::vector<std::string> getListOfLevels(std::string modPath);

protected:
	//actually read through the file and create the custom level
	//throws on syntax error, returns nullptr if bad for some reason (currently just path=="")
	//needs to print warnings on extra parameters for a command
	static CustomLevel* processCustomLevel(std::string path);
	static inline CustomLevel::CustomLevelAction* stringToAction(const std::pair<std::string, int>& command); //throws when action is ill-formatted
	static void pushCustomLevel(CustomLevel* level); //add it to LevelManager (with some checks)

public:
	static std::unordered_map<std::string, std::unordered_map<std::string, CustomLevel*>> customLevelLookup; //TODO: should be moved to LevelManager

protected:
	static inline void stringToAction_WALL(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_WALL_LR(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_WALL_UD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_WALL_Corners(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_WALL_DiagForwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_WALL_DiagBackwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);

	static inline void stringToAction_RANDOM_WALLS(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_CLASSIC_WALLS();

	static inline void stringToAction_POWER(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_LR(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_UD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_Corners(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_DiagForwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_DiagBackwardSlash(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);

	static inline void stringToAction_CHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_RHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);

private:
	CustomLevelInterpreter() = delete;
	CustomLevelInterpreter(const CustomLevelInterpreter&) = delete;
};
