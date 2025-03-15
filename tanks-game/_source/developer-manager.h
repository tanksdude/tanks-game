#pragma once
#include <string>
#include <utility>
#include <vector>

#include "circle.h"
#include "rect.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DeveloperManager { //interpret as "developer testing manager" or "developer menu manager"
private:
	static bool leftMouse;
	static bool rightMouse;
	static int mousePosX;
	static int mousePosY;

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

	static void mouseButtonCallbackFunc(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallbackFunc(GLFWwindow* window, double xOffset, double yOffset);
	static void mouseCursorPosCallbackFunc(GLFWwindow* window, double xPos, double yPos);

	static void addInsertPower(std::string identifier, std::string type, std::string name);

private:
	DeveloperManager() = delete;
	DeveloperManager(const DeveloperManager&) = delete;
};
