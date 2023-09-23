#pragma once
#include <string>
#include <vector>
#include <memory>

#include "power.h"
#include "generic-factory-construction-data.h"

class CustomPower : public Power {
	friend class CustomPowerInterpreter;

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
	//for CustomPowerInterpreter
	CustomPower(std::string name,
		float colorR, float colorG, float colorB,
		int startPosCount, double startPosXValue, double startPosYRange,
		const std::vector<std::string>& types,
		const std::unordered_map<std::string, float>& weights,
		std::vector<CustomLevelAction*>* actions);
	//for CustomPower::factory()
	CustomPower(std::string name,
		float colorR, float colorG, float colorB,
		int startPosCount, double startPosXValue, double startPosYRange,
		const std::vector<std::string>& types,
		const std::unordered_map<std::string, float>& weights,
		std::shared_ptr<std::vector<CustomLevelAction*>> actions);

protected:
	std::vector<std::string> powerTypes;
	std::unordered_map<std::string, float> powerWeights;
	std::vector<std::string> powerAttributes;
	std::string name;
	ColorValueHolder color;
	float colorImportance;

	//TODO: the various values needed
	int startPosCount;
	double startPosXValue;
	double startPosYRange;
	std::shared_ptr<std::vector<CustomLevelAction*>> initializationActions; //remember, the vector's elements need to be deleted

public:
	//shouldn't be virtual
	std::vector<std::string> getPowerTypes() const override { return this->powerTypes; }
	std::unordered_map<std::string, float> getWeights() const override { return this->powerWeights; }
	std::vector<std::string> getPowerAttributes() const override { return this->powerAttributes; }

	std::string getName() const override { return this->name; }
	ColorValueHolder getColor() const override { return this->color; }
	float getColorImportance() const override { return this->colorImportance; }

	TankPower* makeTankPower() const override;
	BulletPower* makeBulletPower() const override;
	//HazardPower* makeHazardPower() const override;

	virtual ~CustomPower();
	[[nodiscard]] CustomPower* factory() const; //makes a copy of itself; used by CustomPowerInterpreter to give PowerDataGovernor an actual power
};



class CustomPowerInterpreter {
public:
	static void ProcessCustomPowers() noexcept;

protected:
	//static const std::string ActionStartPhrase; // = "[LEVEL_START]"; // (maybe "[BODY]" as a HTML reference? implies there should be a [HEAD] but that's implicit)

protected:
	//actually read through the file and create the custom level
	//throws on syntax error, returns nullptr if bad for some reason (currently just path=="")
	//needs to print warnings on extra parameters for a command
	static CustomPower* processCustomPower(std::string path);
	//static inline CustomPower::CustomLevelAction* stringToAction(const std::pair<std::string, int>& command); //throws when action is ill-formatted
	static void addCustomPower(CustomPower* level); //add it to PowerDataGovernor (with some checks)

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
	CustomPowerInterpreter() = delete;
	CustomPowerInterpreter(const CustomPowerInterpreter&) = delete;
};
