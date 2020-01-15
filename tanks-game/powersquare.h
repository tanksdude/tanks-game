#pragma once
class PowerSquare;

#include "constants.h"
#include "colorvalueholder.h"
#include <string>
#include "power.h"
#include "rect.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class PowerSquare : public Rect {
public:
	Power** heldPower; //array of pointers (can do a vector for simplicity but I don't foresee a powersquare getting more powers)
	short numOfPowers;
	//double w = POWER_WIDTH;
	//double h = POWER_HEIGHT;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib_main; //the main thing
	static IndexBuffer* ib_outline; //the extra bit when numOfPowers >= 2
public:
	static void initializeGPU();

public:

	ColorValueHolder getColor();
	void draw(); //draws at center
	void drawCPU();
	void givePower(Tank*);
	void givePower(Bullet*); //don't think about it now, possibly ever; it's weird
	//void givePower(Hazard*);
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, std::string* names, int num);
	~PowerSquare();
};