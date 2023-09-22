#pragma once
#include <string>
#include <vector>

#include "circle.h"
#include "rect.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DeveloperManager { //interpret as "developer testing manager" or "developer menu manager"
private:
	static bool leftMouse;
	static bool rightMouse;
	static bool middleMouse;
	static int insertIndex;
	static std::vector<std::string> insertListIdentifiers;
	static void devInsert(int x, int y);

	static int mousePosX;
	static int mousePosY;

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

private:
	DeveloperManager() = delete;
	DeveloperManager(const DeveloperManager&) = delete;
};
