#pragma once
#include "circlehazard.h"
#include "recthazard.h"
#include <vector>
#include <unordered_map>

typedef CircleHazard* (*CircleHazardFunction)(int, std::string*);
typedef CircleHazard* (*CircleHazardRandomizationFunction)(double, double, double, double, int, std::string*);
typedef RectHazard* (*RectHazardFunction)(int, std::string*);
typedef RectHazard* (*RectHazardRandomizationFunction)(double, double, double, double, int, std::string*);

struct CircleHazardFactoryGroup {
protected:
	CircleHazardFunction circleFactory;
	int argCount;
	CircleHazardConstructionTypes constructionType;
	CircleFactoryInformation factoryInformation;
	CircleHazardRandomizationFunction randomizationFunction;
public:
	CircleHazardFunction getFactory() {
		return circleFactory;
	}
	int getArgCount() {
		return argCount;
	}
	CircleHazardConstructionTypes getConstructionType() {
		return constructionType;
	}
	CircleFactoryInformation getFactoryInformation() {
		return factoryInformation;
	}
	CircleHazardRandomizationFunction getRandomizationFunction() {
		return randomizationFunction;
	}
	CircleHazardFactoryGroup(CircleHazardFunction cf, int ac, CircleHazardConstructionTypes ct, CircleFactoryInformation fi, CircleHazardRandomizationFunction random) {
		circleFactory = cf;
		argCount = ac;
		constructionType = ct;
		factoryInformation = fi;
		randomizationFunction = random;
	}
	CircleHazardFactoryGroup() { circleFactory = nullptr, argCount = 0, constructionType = CircleHazardConstructionTypes::constructionIsTooComplex, factoryInformation = {}, randomizationFunction = nullptr; }
};

struct RectHazardFactoryGroup {
protected:
	RectHazardFunction rectFactory;
	int argCount;
	RectHazardConstructionTypes constructionType;
	RectFactoryInformation factoryInformation;
	RectHazardRandomizationFunction randomizationFunction;
public:
	RectHazardFunction getFactory() {
		return rectFactory;
	}
	int getArgCount() {
		return argCount;
	}
	RectHazardConstructionTypes getConstructionType() {
		return constructionType;
	}
	RectFactoryInformation getFactoryInformation() {
		return factoryInformation;
	}
	RectHazardRandomizationFunction getRandomizationFunction() {
		return randomizationFunction;
	}
	RectHazardFactoryGroup(RectHazardFunction rf, int ac, RectHazardConstructionTypes ct, RectFactoryInformation fi, RectHazardRandomizationFunction random) {
		rectFactory = rf;
		argCount = ac;
		constructionType = ct;
		factoryInformation = fi;
		randomizationFunction = random;
	}
	RectHazardFactoryGroup() { rectFactory = nullptr, argCount = 0, constructionType = RectHazardConstructionTypes::constructionIsTooComplex, factoryInformation = {}, randomizationFunction = nullptr; }
};

class HazardManager {
	friend class ResetThings;
private:
	static std::vector<CircleHazard*> circleHazards;
	static std::vector<RectHazard*> rectHazards;
	static void clearCircleHazards();
	static void clearRectHazards(); //only ResetThings is supposed to be using these, so no real point in splitting them

	static std::unordered_map<std::string, CircleHazardFactoryGroup> circleHazardLookup;
	static std::unordered_map<std::string, RectHazardFactoryGroup> rectHazardLookup;
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

	static void addCircleHazardFactory(CircleHazardFunction, CircleHazardRandomizationFunction);
	static void addRectHazardFactory(RectHazardFunction, RectHazardRandomizationFunction);
	static CircleHazardFunction getCircleHazardFactory(std::string name);
	static RectHazardFunction getRectHazardFactory(std::string name);
	static CircleHazardFactoryGroup getCircleHazardFactoryGroup(std::string name); //might not get used
	static RectHazardFactoryGroup getRectHazardFactoryGroup(std::string name); //might not get used
	static CircleHazardRandomizationFunction getCircleHazardRandomizationFunction(std::string name);
	static RectHazardRandomizationFunction getRectHazardRandomizationFunction(std::string name);
	static std::string getCircleHazardName(int index);
	static std::string getRectHazardName(int index);
	static int getNumCircleHazardTypes();
	static int getNumRectHazardTypes();
};
