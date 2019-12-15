#pragma once
#include "circle.h"
#include "colorvalueholder.h"

class Bullet : public Circle {
	friend class ResetThings;
	friend class BulletPriorityHandler;
	double angle;
	double velocity;
	char id;
	ColorValueHolder defaultColor = ColorValueHolder(0x88, 0x88, 0x88);
	//ColorValueHolder* explosionColor; //needed?
	
public:
	Bullet(double x_, double y_, double r_, double a, double vel, char id_);
	void move();
	void draw();
	void draw(double, double);
	char getID() { return id; }
	short determineDamage();

	bool isFullyOutOfBounds();
	bool isPartiallyOutOfBounds();
	void edgeConstrain(); //probably never going to be used

	~Bullet();
};