#pragma once
class Bullet;

#include "circle.h"
#include "colorvalueholder.h"
#include "bulletpower.h"
#include "powerfunctionhelper.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Bullet : public Circle {
	friend class ResetThings;
	friend class BulletPriorityHandler;
	friend class PowerFunctionHelper;
public: //hopefully temporary
	double angle;
	double velocity;
	double acceleration;
	char id;
	ColorValueHolder defaultColor = ColorValueHolder(.5f, .5f, .5f);
	//ColorValueHolder* explosionColor; //needed?
	double getAngle();
	double alpha; //[0,100] to avoid minor float imprecision
	bool isDead();

public:
	std::vector<BulletPower*> bulletPowers; //change eventually?
	double getOffenseTier();
	double getDefenseTier();
private:
	double getHighestOffenseImportance();
	double getHighestOffenseTier(double importance);
	double getHighestDefenseImportance();
	double getHighestDefenseTier(double importance);

public:
	//helper functions:
	ColorValueHolder getColor();

	static const double default_radius;

private:
	//IMPORTANT: GLEW must be initialized before these are set
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;
public:
	static bool initializeGPU(); //returns whether it successful
	static bool uninitializeGPU();

	void drawBody(double, double);
	void drawOutline(double, double);
	
public:
	Bullet(double x_, double y_, double r_, double a, double vel, double acc, char id_);
	Bullet(double x_, double y_, double r_, double a, double vel, double acc, char id_, std::vector<BulletPower*>);
	void move();
	void draw();
	void draw(double, double);
	void drawCPU();
	void drawCPU(double, double);
	char getID() { return id; }
	short determineDamage(); //TODO: wait, what is this supposed to do again? (isn't everything supposed to have 1 health? so there's no point for this)

	void powerCalculate();
	void removePower(int index);

	bool isFullyOutOfBounds();
	bool isPartiallyOutOfBounds();
	//void edgeConstrain(); //probably never going to be needed

	~Bullet();
};
