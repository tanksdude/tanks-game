#pragma once
#include <string>
#include <utility>
#include <vector>

#include "circle.h"
#include "rect.h"

class DeveloperManager { //interpret as "developer testing manager" or "developer menu manager"
private:
	static bool leftMouse;
	static bool rightMouse;
	static int insertIndex;
	static std::vector<std::string> insertListIdentifiers;
	static int insertListIdentifiers_normalSize; //before addInsertPower()
	static std::vector<std::pair<std::string, std::string>> insertList_extra; //before addInsertPower()
	static void devInsert(int x, int y);

public:
	static void setX(Circle*, double);
	static void setY(Circle*, double);
	static void setR(Circle*, double);

	static void setX(Rect*, double);
	static void setY(Rect*, double);
	static void setW(Rect*, double);
	static void setH(Rect*, double);

	//static void mouseDragFunc(int x, int y);
	//static void mouseClickFunc(int button, int state, int x, int y);
	//static void mouseWheelFunc(int wheel, int dir, int x, int y);
	static void doMouseStuff();

	static void addInsertPower(std::string identifier, std::string type, std::string name);

private:
	DeveloperManager() = delete;
	DeveloperManager(const DeveloperManager&) = delete;
};
