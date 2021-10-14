#pragma once
class PowerSquare;

#include "gamething.h"
#include "drawablething.h"
#include "rect.h"
#include "colorvalueholder.h"
#include <string>
#include "power.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class PowerSquare : public Rect, public GameThing, public DrawableThing {
public:
	Power** heldPowers; //array of pointers (can do a vector for simplicity but I don't foresee a powersquare getting more powers)
	short numOfPowers;

public:
	static const double POWER_WIDTH;
	static const double POWER_HEIGHT;
	static const double POWER_LINE_WIDTH;
	static const double POWER_OUTLINE_MULTIPLIER;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib_main; //the main thing
	static IndexBuffer* ib_outline; //the extra bit when numOfPowers >= 2
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

private:
	PowerSquare() {} //don't use
	PowerSquare(double x, double y); //common
public:
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, std::string* names, int num);
	PowerSquare(double x, double y, std::string type, std::string name);
	PowerSquare(double x, double y, std::string type, std::string* names, int num);
	PowerSquare(double x, double y, std::string* types, std::string* names, int num);
	PowerSquare(const PowerSquare&);

	ColorValueHolder getColor() const;
	void draw() const override; //draws at center
	void draw(double xpos, double ypos) const override;
	void poseDraw() const override;
	void drawCPU();
	void givePower(Tank*);
	void givePower(Bullet*); //don't think about it now, possibly ever; it's weird
	//void givePower(Hazard*);

	~PowerSquare();
};
