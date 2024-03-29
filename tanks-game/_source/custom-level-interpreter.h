#pragma once
#include <string>
#include <vector>
#include <memory>

#include "level.h"
#include "generic-factory-construction-data.h"

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
		POWER_DiagForwardSlash,
		POWER_DiagBackwardSlash,

		POWER_LR_Alternate, //separate the powers with |
		POWER_UD_Alternate,
		POWER_Corners_Alternate,
		POWER_DiagForwardSlash_Alternate,
		POWER_DiagBackwardSlash_Alternate,

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
	static inline void initialization_WALL(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_WALL_LR(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_WALL_UD(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_WALL_Corners(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_WALL_DiagForwardSlash(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_WALL_DiagBackwardSlash(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;

	static inline void initialization_RANDOM_WALLS(const GenericFactoryConstructionData&, const ColorValueHolder& wallColor) noexcept;
	static inline void initialization_CLASSIC_WALLS(const ColorValueHolder& wallColor) noexcept;

	static inline void initialization_POWER(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_LR(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_UD(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_Corners(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagForwardSlash(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagBackwardSlash(const GenericFactoryConstructionData&) noexcept;

	static inline void initialization_POWER_LR_Alternate(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_UD_Alternate(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_Corners_Alternate(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagForwardSlash_Alternate(const GenericFactoryConstructionData&) noexcept;
	static inline void initialization_POWER_DiagBackwardSlash_Alternate(const GenericFactoryConstructionData&) noexcept;

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
	[[nodiscard]] CustomLevel* factory() const; //makes a copy of itself; used by CustomLevelInterpreter to give LevelDataGovernor an actual level
};



class CustomLevelInterpreter {
public:
	static void ProcessCustomLevels() noexcept;

protected:
	static const std::string ActionStartPhrase; // = "[LEVEL_START]"; // (maybe "[BODY]" as a HTML reference? implies there should be a [HEAD] but that's implicit)

protected:
	//actually read through the file and create the custom level
	//throws on syntax error, returns nullptr if bad for some reason (currently just path=="")
	//needs to print warnings on extra parameters for a command
	static CustomLevel* processCustomLevel(std::string path);
	static inline CustomLevel::CustomLevelAction* stringToAction(const std::pair<std::string, int>& command); //throws when action is ill-formatted
	static void addCustomLevel(CustomLevel* level); //add it to LevelDataGovernor (with some checks)

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

	static inline void stringToAction_POWER_LR_Alternate(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_UD_Alternate(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_Corners_Alternate(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_DiagForwardSlash_Alternate(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_POWER_DiagBackwardSlash_Alternate(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);

	static inline void stringToAction_CHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);
	static inline void stringToAction_RHAZARD(const std::vector<std::string>& words, GenericFactoryConstructionData& constructionData);

private:
	CustomLevelInterpreter() = delete;
	CustomLevelInterpreter(const CustomLevelInterpreter&) = delete;
};
