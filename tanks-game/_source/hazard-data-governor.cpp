#include "hazard-data-governor.h"

std::unordered_map<std::string, std::unordered_map<std::string, CircleHazardFactoryGroup>> HazardDataGovernor::circleHazardLookup;
std::unordered_map<std::string, std::unordered_map<std::string, RectHazardFactoryGroup>> HazardDataGovernor::rectHazardLookup;
std::unordered_map<std::string, std::vector<std::string>> HazardDataGovernor::circleHazardNameList;
std::unordered_map<std::string, std::vector<std::string>> HazardDataGovernor::rectHazardNameList;

void HazardDataGovernor::initialize() {
	circleHazardLookup.insert({ "vanilla", std::unordered_map<std::string, CircleHazardFactoryGroup>() });
	circleHazardLookup.insert({ "vanilla-extra", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //what would this include? no bullet zone?
	circleHazardLookup.insert({ "random-vanilla", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //can include vanilla-extra but probably won't
	circleHazardLookup.insert({ "old", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //will probably be deleted
	circleHazardLookup.insert({ "random-old", std::unordered_map<std::string, CircleHazardFactoryGroup>() });
	circleHazardLookup.insert({ "random", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //general random
	circleHazardLookup.insert({ "dev", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //anything?
	circleHazardLookup.insert({ "random-dev", std::unordered_map<std::string, CircleHazardFactoryGroup>() }); //would this be used?

	rectHazardLookup.insert({ "vanilla", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "vanilla-extra", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "random-vanilla", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "old", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "random-old", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "random", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "dev", std::unordered_map<std::string, RectHazardFactoryGroup>() });
	rectHazardLookup.insert({ "random-dev", std::unordered_map<std::string, RectHazardFactoryGroup>() });
}

void HazardDataGovernor::addCircleHazardFactory(CircleHazardFunction factory, CircleHazardRandomizationFunction randFactory) {
	GenericFactoryConstructionData constructionData;
	CircleHazard* ch = factory(constructionData);
	std::vector<std::string> types = ch->getHazardTypes();
	for (int i = 0; i < types.size(); i++) {
		circleHazardLookup[types[i]].insert({ ch->getName(), { factory, ch->getFactoryArgumentCount(), ch->getConstructionType(), ch->getFactoryInformation(), randFactory } });
		circleHazardNameList[types[i]].push_back(ch->getName());
	}
	delete ch;
}
void HazardDataGovernor::addRectHazardFactory(RectHazardFunction factory, RectHazardRandomizationFunction randFactory) {
	GenericFactoryConstructionData constructionData;
	RectHazard* rh = factory(constructionData);
	std::vector<std::string> types = rh->getHazardTypes();
	for (int i = 0; i < types.size(); i++) {
		rectHazardLookup[types[i]].insert({ rh->getName(), { factory, rh->getFactoryArgumentCount(), rh->getConstructionType(), rh->getFactoryInformation(), randFactory } });
		rectHazardNameList[types[i]].push_back(rh->getName());
	}
	delete rh;
}

CircleHazardFunction HazardDataGovernor::getCircleHazardFactory(std::string type, std::string name) {
	return getCircleHazardFactoryGroup(type, name).getFactory();
}
RectHazardFunction HazardDataGovernor::getRectHazardFactory(std::string type, std::string name) {
	return getRectHazardFactoryGroup(type, name).getFactory();
}

CircleHazardRandomizationFunction HazardDataGovernor::getCircleHazardRandomizationFunction(std::string type, std::string name) {
	return getCircleHazardFactoryGroup(type, name).getRandomizationFunction();
}
RectHazardRandomizationFunction HazardDataGovernor::getRectHazardRandomizationFunction(std::string type, std::string name) {
	return getRectHazardFactoryGroup(type, name).getRandomizationFunction();
}

CircleHazardFactoryGroup HazardDataGovernor::getCircleHazardFactoryGroup(std::string type, std::string name) {
	if (circleHazardLookup.find(type) == circleHazardLookup.end()) {
		throw std::runtime_error("circle hazard type \"" + type + "\" unknown!");
	}
	if (circleHazardLookup[type].find(name) == circleHazardLookup[type].end()) {
		throw std::runtime_error("circle hazard name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return circleHazardLookup[type][name];
}
RectHazardFactoryGroup HazardDataGovernor::getRectHazardFactoryGroup(std::string type, std::string name) {
	if (rectHazardLookup.find(type) == rectHazardLookup.end()) {
		throw std::runtime_error("rect hazard type \"" + type + "\" unknown!");
	}
	if (rectHazardLookup[type].find(name) == rectHazardLookup[type].end()) {
		throw std::runtime_error("rect hazard name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return rectHazardLookup[type][name];
}

std::string HazardDataGovernor::getCircleHazardName(std::string type, unsigned int index) {
	if (circleHazardLookup.find(type) == circleHazardLookup.end()) {
		throw std::runtime_error("circle hazard type \"" + type + "\" unknown!");
	}
	if (index >= circleHazardNameList[type].size()) {
		throw std::range_error("circle hazard index " + std::to_string(index) + " is too large!");
	}
	return circleHazardNameList[type][index];
}
std::string HazardDataGovernor::getRectHazardName(std::string type, unsigned int index) {
	if (rectHazardLookup.find(type) == rectHazardLookup.end()) {
		throw std::runtime_error("rect hazard type \"" + type + "\" unknown!");
	}
	if (index >= rectHazardNameList[type].size()) {
		throw std::range_error("rect hazard index " + std::to_string(index) + " is too large!");
	}
	return rectHazardNameList[type][index];
}

unsigned int HazardDataGovernor::getNumCircleHazardTypes(std::string type) {
	if (circleHazardLookup.find(type) == circleHazardLookup.end()) {
		throw std::runtime_error("circle hazard type \"" + type + "\" unknown!");
	}
	return circleHazardNameList[type].size();
}
unsigned int HazardDataGovernor::getNumRectHazardTypes(std::string type) {
	if (rectHazardLookup.find(type) == rectHazardLookup.end()) {
		throw std::runtime_error("rect hazard type \"" + type + "\" unknown!");
	}
	return rectHazardNameList[type].size();
}
