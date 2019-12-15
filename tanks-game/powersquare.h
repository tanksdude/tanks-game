#pragma once
class PowerSquare;

#include "constants.h"
#include <string>
#include "power.h"
#include "rect.h"

class PowerSquare : public Rect {
private:
	Power* heldPower;
	short numOfPowers;
	double w = POWER_WIDTH;
	double h = POWER_HEIGHT;

public:

	void draw(); //draws at center
	void givePower(Tank*);
	//void givePower(Hazard*);
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, std::string* names, int num);
	~PowerSquare();
};