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

struct TankInputChar {
	bool isSpecial;
	int character;
	bool getKeyState();
	TankInputChar(bool, int);
	TankInputChar();
};

class Tank : public Circle {
public:
	friend class ResetThings;
	friend class Level; //might remove
	friend class PowerFunctionHelper;
	char id;
	std::string name;

	int wins = 0;

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

	std::vector<TankPower*> tankPowers;
	double getOffenseTier();
	double getDefenseTier();
private:
	double getHighestOffenseImportance();
	double getHighestOffenseTier(double importance);
	double getHighestDefenseImportance();
	double getHighestDefenseTier(double importance);

public:
	//double shootingSpeedMultiplier = 1;
	double getShootingSpeedMultiplier();
	//double powerMultiplier; //would be used for an ini

	double getBulletSpeedMultiplier(); //BIG TODO: this isn't going to work if a bullet picks up a power; solution: worry about it later
	double getBulletRadiusMultiplier();
	double getBulletAcceleration();

public:
	TankInputChar forward;
	TankInputChar turnL;
	TankInputChar turnR;
	TankInputChar shooting;
	//TankInputChar backwards; //not the point of the game

	double& giveMaxSpeed() { return maxSpeed; }
	double& giveX() { return x; }
	double& giveY() { return y; }

	void makeBullet(double x, double y, double angle, double radius, double speed, double acc); //move to private eventually, just public for emergency testing
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

	double shootCount;
	double maxShootCount;

public:
	//helper stuff:
	ColorValueHolder getBodyColor();

private:
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	VertexArray* cannon_va;
	VertexBuffer* cannon_vb;
public:
	void initializeGPU();

public:
	Tank(double x, double y, double a, char id, std::string name, TankInputChar forward, TankInputChar left, TankInputChar right, TankInputChar shoot);

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