#pragma once
#include "circle.h"
#include "rect.h"

class DeveloperManager { //interpret as "developer testing manager" or "developer menu manager"
public:
	static double getX(Circle*);
	static double getY(Circle*);
	static double getR(Circle*);

	static double getX(Rect*);
	static double getY(Rect*);
	static double getW(Rect*);
	static double getH(Rect*);


};