#pragma once
class PowerSquare;

#include "constants.h"
#include "colorvalueholder.h"
#include <string>
#include "power.h"
#include "rect.h"

class PowerSquare : public Rect {
public:
	Power** heldPower; //array of pointers
	short numOfPowers;
	//double w = POWER_WIDTH;
	//double h = POWER_HEIGHT;

public:

	ColorValueHolder getColor();
	void draw(); //draws at center
	void givePower(Tank*);
	void givePower(Bullet*); //don't think about it now, possibly ever; it's weird
	//void givePower(Hazard*);
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, std::string* names, int num);
	~PowerSquare();
};