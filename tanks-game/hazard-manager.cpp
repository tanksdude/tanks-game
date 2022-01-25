#include "hazard-manager.h"

std::vector<CircleHazard*> HazardManager::circleHazards;
std::vector<RectHazard*> HazardManager::rectHazards;

std::unordered_map<std::string, std::unordered_map<std::string, CircleHazardFactoryGroup>> HazardManager::circleHazardLookup;
std::unordered_map<std::string, std::unordered_map<std::string, RectHazardFactoryGroup>> HazardManager::rectHazardLookup;
std::unordered_map<std::string, std::vector<std::string>> HazardManager::circleHazardNameList;
std::unordered_map<std::string, std::vector<std::string>> HazardManager::rectHazardNameList;

void HazardManager::initialize() {
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

CircleHazard* HazardManager::getCircleHazard(int index) {
	return circleHazards[index];
}
RectHazard* HazardManager::getRectHazard(int index) {
	return rectHazards[index];
}

CircleHazard* HazardManager::getCircleHazardByID(Game_ID gameID) {
	for (int i = 0; i < circleHazards.size(); i++) {
		if (circleHazards[i]->getGameID() == gameID) {
			return circleHazards[i];
		}
	}
	return nullptr;
}
RectHazard* HazardManager::getRectHazardByID(Game_ID gameID) {
	for (int i = 0; i < rectHazards.size(); i++) {
		if (rectHazards[i]->getGameID() == gameID) {
			return rectHazards[i];
		}
	}
	return nullptr;
}

void HazardManager::pushCircleHazard(CircleHazard* ch) {
	circleHazards.push_back(ch);
}
void HazardManager::pushRectHazard(RectHazard* rh) {
	rectHazards.push_back(rh);
}

void HazardManager::deleteCircleHazard(int index) {
	delete circleHazards[index];
	circleHazards.erase(circleHazards.begin() + index);
}
void HazardManager::deleteRectHazard(int index) {
	delete rectHazards[index];
	rectHazards.erase(rectHazards.begin() + index);
}

void HazardManager::deleteCircleHazardByID(Game_ID gameID) {
	for (int i = 0; i < circleHazards.size(); i++) {
		if (circleHazards[i]->getGameID() == gameID) {
			deleteCircleHazard(i);
			break;
		}
	}
}
void HazardManager::deleteRectHazardByID(Game_ID gameID) {
	for (int i = 0; i < rectHazards.size(); i++) {
		if (rectHazards[i]->getGameID() == gameID) {
			deleteRectHazard(i);
			break;
		}
	}
}

void HazardManager::clearCircleHazards() {
	for (int i = 0; i < circleHazards.size(); i++) {
		delete circleHazards[i];
	}
	circleHazards.clear();
}
void HazardManager::clearRectHazards() {
	for (int i = 0; i < rectHazards.size(); i++) {
		delete rectHazards[i];
	}
	rectHazards.clear();
}


void HazardManager::addCircleHazardFactory(CircleHazardFunction factory, CircleHazardRandomizationFunction randFactory) {
	GenericFactoryConstructionData constructionData;
	CircleHazard* ch = factory(constructionData);
	std::vector<std::string> types = ch->getHazardTypes();
	for (int i = 0; i < types.size(); i++) {
		circleHazardLookup[types[i]].insert({ ch->getName(), { factory, ch->getFactoryArgumentCount(), ch->getConstructionType(), ch->getFactoryInformation(), randFactory } });
		circleHazardNameList[types[i]].push_back(ch->getName());
	}
	delete ch;
}
void HazardManager::addRectHazardFactory(RectHazardFunction factory, RectHazardRandomizationFunction randFactory) {
	GenericFactoryConstructionData constructionData;
	RectHazard* rh = factory(constructionData);
	std::vector<std::string> types = rh->getHazardTypes();
	for (int i = 0; i < types.size(); i++) {
		rectHazardLookup[types[i]].insert({ rh->getName(), { factory, rh->getFactoryArgumentCount(), rh->getConstructionType(), rh->getFactoryInformation(), randFactory } });
		rectHazardNameList[types[i]].push_back(rh->getName());
	}
	delete rh;
}

CircleHazardFunction HazardManager::getCircleHazardFactory(std::string type, std::string name) {
	return circleHazardLookup[type][name].getFactory();
}
RectHazardFunction HazardManager::getRectHazardFactory(std::string type, std::string name) {
	return rectHazardLookup[type][name].getFactory();
}

CircleHazardRandomizationFunction HazardManager::getCircleHazardRandomizationFunction(std::string type, std::string name) {
	return circleHazardLookup[type][name].getRandomizationFunction();
}
RectHazardRandomizationFunction HazardManager::getRectHazardRandomizationFunction(std::string type, std::string name) {
	return rectHazardLookup[type][name].getRandomizationFunction();
}

CircleHazardFactoryGroup HazardManager::getCircleHazardFactoryGroup(std::string type, std::string name) {
	return circleHazardLookup[type][name];
}
RectHazardFactoryGroup HazardManager::getRectHazardFactoryGroup(std::string type, std::string name) {
	return rectHazardLookup[type][name];
}

std::string HazardManager::getCircleHazardName(std::string type, int index) {
	return circleHazardNameList[type][index];
}
std::string HazardManager::getRectHazardName(std::string type, int index) {
	return rectHazardNameList[type][index];
}

int HazardManager::getNumCircleHazardTypes(std::string type) {
	return circleHazardNameList[type].size();
}
int HazardManager::getNumRectHazardTypes(std::string type) {
	return rectHazardNameList[type].size();
}

std::vector<Circle*> HazardManager::getCircleHazardCollisionList() {
	return std::vector<Circle*>(circleHazards.begin(), circleHazards.end());
}
std::vector<Rect*> HazardManager::getRectHazardCollisionList() {
	return std::vector<Rect*>(rectHazards.begin(), rectHazards.end());
}
