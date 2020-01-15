#pragma once
class Tank;

#include <string>
#include <vector>
#include "circle.h"
#include "colorvalueholder.h"
#include "cannonpoint.h"
#include "resetthings.h"
#include "powerfunctionhelper.h"
#include "tankpower.h"
#include "bulletpower.h"
//include hazardpower?

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Tank : public Circle {
public:
	friend class ResetThings;
	friend class Level; //might remove
	friend class PowerFunctionHelper;
	char id;
	std::string name;

	int wins = 0;
	std::vector<TankPower*> tankPowers;

	double maxSpeed = 1;
	double acceleration = 1.0/16; //intentional acceleration, not total
	double velocity = 0; //intentional velocity, not total
	//TODO: system that can apply forces
	double turningIncrement = 64;
	double angle;
	double getAngle();
	double getCannonAngle(int index);
	double getRealCannonAngle(int index);
	std::vector<CannonPoint>* shootingPoints;

	static const double default_radius;

	//double shootingSpeedMultiplier = 1;
	double getShootingSpeedMultiplier();
	//double powerMultiplier; //would be used for an ini

	double getBulletSpeedMultiplier(); //BIG TODO: this isn't going to work if a bullet picks up a power; solution: worry about it later
	double getBulletRadiusMultiplier();

public:
	bool turnL;
	bool turnR;
	bool forward;
	bool shooting;
	//bool backwards; //not the point of the game

	double& giveMaxSpeed() { return maxSpeed; }
	double& giveX() { return x; }
	double& giveY() { return y; }

	void makeBullet(double x, double y, double angle, double radius, double speed); //move to private eventually, just public for emergency testing
	void defaultMakeBullet(double x, double y, double angle);
	//next up: make cannon points a thing; function should get list of cannon points (probably make a struct to hold x, y, angle) (for multishot), shoot function does the shooting (possibly modified uwu (for triple/shotgun))
	void determineShootingAngles();

private:
	ColorValueHolder defaultColor = ColorValueHolder(.5f, .5f, .5f);
	bool dead = false;
	ColorValueHolder* explosionColor;
	ColorValueHolder defaultNameFill = ColorValueHolder(1.0f, 1.0f, 1.0f);
	ColorValueHolder defaultNameStroke = ColorValueHolder(0, 0, 0);

	void resetThings(double x, double y, double a, char id, std::string name);

	double shootCount = 0;
	double maxShootCount = 200; //temporary? //should be 100

public:
	//helper stuff:
	ColorValueHolder getBodyColor();

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static VertexArray* cannon_va;
	static VertexBuffer* cannon_vb;
public:
	static void initializeGPU();

public:
	Tank(double x, double y, double a, char id, std::string name);
	Tank();

	void move();
	void terminalVelocity(); //move to protected
	void edgeConstrain();
	bool isPartiallyOutOfBounds();
	bool isFullyOutOfBounds();
	void shoot();
	void powerCalculate();
	void removePower(int index);
	void powerReset();
	void draw();
	void draw(double xpos, double ypos);
	void drawCPU();
	void drawCPU(double, double);
	void drawName();
	void drawName(double xpos, double ypos);
	char getID() { return id; }
	std::string getName() { return name; }
	
	~Tank();
};