#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "circle-hazard.h"
#include "rect-hazard.h"

typedef CircleHazard* (*CircleHazardFunction)(const GenericFactoryConstructionData&);
typedef CircleHazard* (*CircleHazardRandomizationFunction)(double, double, double, double, const GenericFactoryConstructionData&);
typedef RectHazard* (*RectHazardFunction)(const GenericFactoryConstructionData&);
typedef RectHazard* (*RectHazardRandomizationFunction)(double, double, double, double, const GenericFactoryConstructionData&);

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

class HazardDataGovernor {
private:
	static std::unordered_map<std::string, std::unordered_map<std::string, CircleHazardFactoryGroup>> circleHazardLookup;
	static std::unordered_map<std::string, std::unordered_map<std::string, RectHazardFactoryGroup>> rectHazardLookup;
	static std::unordered_map<std::string, std::vector<std::string>> circleHazardNameList;
	static std::unordered_map<std::string, std::vector<std::string>> rectHazardNameList;

public:
	static void initialize();

	static void addCircleHazardFactory(CircleHazardFunction, CircleHazardRandomizationFunction);
	static void addRectHazardFactory(RectHazardFunction, RectHazardRandomizationFunction);
	static CircleHazardFunction getCircleHazardFactory(std::string type, std::string name);
	static RectHazardFunction getRectHazardFactory(std::string type, std::string name);
	static CircleHazardFactoryGroup getCircleHazardFactoryGroup(std::string type, std::string name); //might not get used
	static RectHazardFactoryGroup getRectHazardFactoryGroup(std::string type, std::string name); //might not get used
	static CircleHazardRandomizationFunction getCircleHazardRandomizationFunction(std::string type, std::string name);
	static RectHazardRandomizationFunction getRectHazardRandomizationFunction(std::string type, std::string name);
	static std::string getCircleHazardName(std::string type, unsigned int index);
	static std::string getRectHazardName(std::string type, unsigned int index);
	static unsigned int getNumCircleHazardTypes(std::string type);
	static unsigned int getNumRectHazardTypes(std::string type);

private:
	HazardDataGovernor() = delete;
	HazardDataGovernor(const HazardDataGovernor&) = delete;
};
