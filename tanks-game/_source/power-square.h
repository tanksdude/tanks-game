#pragma once
class PowerSquare;

#include <string>

#include "game-thing.h"
#include "rect.h"
#include "drawable-thing.h"
#include "power.h"

#include "color-value-holder.h"

class PowerSquare : public GameThing, public Rect, public DrawableThing {
protected:
	Power** heldPowers; //array of pointers (can do a vector for simplicity but I don't foresee a powersquare getting more powers)
	int numOfPowers;

public:
	static const double POWER_WIDTH;
	static const double POWER_HEIGHT;
	static const double POWER_LINE_WIDTH;
	static const double POWER_OUTLINE_MULTIPLIER;

public:
	ColorValueHolder getColor() const;
	void givePower(Tank*);
	//void givePower(Bullet*); //don't think about it now, possibly ever; it's weird
	//void givePower(Hazard*);

	void draw() const override;
	void draw(DrawingLayers) const override;
	void poseDraw() const override;
	void poseDraw(DrawingLayers) const override;
	void ghostDraw(float alpha) const override;
	void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	inline void drawMain(float alpha = 1.0f) const;
	inline void drawOutlineThing(float alpha = 1.0f) const;

private:
	PowerSquare(double x, double y); //common
public:
	PowerSquare(double x, double y, std::string name);
	PowerSquare(double x, double y, const std::string* names, int num);
	PowerSquare(double x, double y, std::string type, std::string name);
	PowerSquare(double x, double y, std::string type, const std::string* names, int num);
	PowerSquare(double x, double y, const std::string* types, const std::string* names, int num);
	PowerSquare(const PowerSquare&);
	~PowerSquare();

private:
	PowerSquare() = delete;
};
