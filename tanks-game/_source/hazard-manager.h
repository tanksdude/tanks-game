#pragma once
#include <vector>
#include "circle-hazard.h"
#include "rect-hazard.h"

class HazardManager {
	friend class ResetThings;

private:
	static std::vector<CircleHazard*> circleHazards;
	static std::vector<RectHazard*> rectHazards;
	static void clearCircleHazards();
	static void clearRectHazards(); //only ResetThings is supposed to be using these, so no real point in splitting them

public:
	static void initialize();
	static CircleHazard* getCircleHazard(unsigned int index);
	static RectHazard* getRectHazard(unsigned int index);
	static CircleHazard* getCircleHazardByID(Game_ID);
	static RectHazard* getRectHazardByID(Game_ID);
	static void pushCircleHazard(std::string type, std::string name, const GenericFactoryConstructionData& data);
	static void pushRectHazard(std::string type, std::string name, const GenericFactoryConstructionData& data);
	static unsigned int getNumCircleHazards() { return circleHazards.size(); }
	static unsigned int getNumRectHazards() { return rectHazards.size(); }
	static void deleteCircleHazard(unsigned int index);
	static void deleteRectHazard(unsigned int index);
	static void deleteCircleHazardByID(Game_ID);
	static void deleteRectHazardByID(Game_ID);

	[[nodiscard]] static CircleHazard* makeCircleHazard(std::string type, std::string name, const GenericFactoryConstructionData& data);
	[[nodiscard]] static RectHazard* makeRectHazard(std::string type, std::string name, const GenericFactoryConstructionData& data);
	static void pushCircleHazard(CircleHazard*);
	static void pushRectHazard(RectHazard*);

private:
	HazardManager() = delete;
	HazardManager(const HazardManager&) = delete;
};
