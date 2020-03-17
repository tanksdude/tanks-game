#pragma once
class PowerSquare;

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

public:
	static const double POWER_WIDTH;
	static const double POWER_HEIGHT;
	static const double POWER_LINE_WIDTH;
	static const double POWER_OUTLINE_MULTIPLIER;

private:
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib_main; //the main thing
	IndexBuffer* ib_outline; //the extra bit when numOfPowers >= 2
public:
	void initializeGPU();

private:
	PowerSquare(double x, double y);
public:
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, std::string* names, int num);

	ColorValueHolder getColor();
	void draw(); //draws at center
	void drawCPU();
	void givePower(Tank*);
	void givePower(Bullet*); //don't think about it now, possibly ever; it's weird
	//void givePower(Hazard*);

	~PowerSquare();
};