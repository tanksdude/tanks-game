#pragma once
#include "circlehazard.h"
#include "recthazard.h"
#include <vector>
#include <unordered_map>

typedef CircleHazard* (*CircleHazardFunction)(int, std::string*);
typedef RectHazard* (*RectHazardFunction)(int, std::string*);

class HazardManager {
	friend class ResetThings;
private:
	static std::vector<CircleHazard*> circleHazards;
	static std::vector<RectHazard*> rectHazards;
	static void clearCircleHazards();
	static void clearRectHazards(); //only ResetThings is supposed to be using these, so no real point in splitting them

	static std::unordered_map<std::string, CircleHazardFunction> circleHazardLookup;
	static std::unordered_map<std::string, RectHazardFunction> rectHazardLookup;
	static std::vector<CircleHazardFunction> circleHazardList;
	static std::vector<RectHazardFunction> rectHazardList;
	static std::vector<std::string> circleHazardNameList;
	static std::vector<std::string> rectHazardNameList;
public:
	static void initialize();
	static CircleHazard* getCircleHazard(int index);
	static RectHazard* getRectHazard(int index);
	static void pushCircleHazard(CircleHazard*);
	static void pushRectHazard(RectHazard*);
	static int getNumCircleHazards() { return circleHazards.size(); }
	static int getNumRectHazards() { return rectHazards.size(); }
	static void deleteCircleHazard(int index);
	static void deleteRectHazard(int index);

	static void addCircleHazardFactory(CircleHazardFunction);
	static void addRectHazardFactory(RectHazardFunction);
	static CircleHazardFunction getCircleHazardFactory(std::string name);
	static RectHazardFunction getRectHazardFactory(std::string name);
	static std::string getCircleHazardName(int index);
	static std::string getRectHazardName(int index);
	static int getNumCircleHazardTypes();
	static int getNumRectHazardTypes();
};
