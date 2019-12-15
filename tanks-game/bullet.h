#pragma once
class Bullet;

#include "circle.h"
#include "colorvalueholder.h"
#include "bulletpower.h"
#include "powerfunctionhelper.h"

class Bullet : public Circle {
	friend class ResetThings;
	friend class BulletPriorityHandler;
	friend class PowerFunctionHelper;
public: //hopefully temporary
	double angle;
	double velocity;
	char id;
	ColorValueHolder defaultColor = ColorValueHolder(0x88, 0x88, 0x88);
	//ColorValueHolder* explosionColor; //needed?
	double getAngle();

public:
	std::vector<BulletPower*> bulletPowers; //change eventually?

public:
	//helper functions:
	ColorValueHolder getColor();
	
public:
	Bullet(double x_, double y_, double r_, double a, double vel, char id_);
	Bullet(double x_, double y_, double r_, double a, double vel, char id_, std::vector<BulletPower*>);
	void move();
	void draw();
	void draw(double, double);
	char getID() { return id; }
	short determineDamage();

	void powerCalculate();
	void removePower(int index);

	bool isFullyOutOfBounds();
	bool isPartiallyOutOfBounds();
	void edgeConstrain(); //probably never going to be used

	~Bullet();
};
