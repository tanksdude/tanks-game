#pragma once
#include "circle.h"
#include "rect.h"
#include <string>
#include <vector>

class DeveloperManager { //interpret as "developer testing manager" or "developer menu manager"
private:
	static bool leftMouse;
	static bool rightMouse;
	static int insertIndex;
	static std::vector<std::string> insertListIdentifiers;
	static void devInsert(int x, int y);

public:
	static double getX(Circle*);
	static double getY(Circle*);
	static double getR(Circle*);

	static double getX(Rect*);
	static double getY(Rect*);
	static double getW(Rect*);
	static double getH(Rect*);

	static void mouseDragFunc(int x, int y);
	static void mouseClickFunc(int button, int state, int x, int y);
	static void mouseWheelFunc(int wheel, int dir, int x, int y);
};
