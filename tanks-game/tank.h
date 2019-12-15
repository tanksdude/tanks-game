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
	std::vector<CannonPoint>* shootingPoints;

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
	ColorValueHolder defaultColor = ColorValueHolder(0x88, 0x88, 0x88);
	bool dead = false;
	ColorValueHolder* explosionColor;
	ColorValueHolder defaultNameFill = ColorValueHolder(0xFF, 0xFF, 0xFF);
	ColorValueHolder defaultNameStroke = ColorValueHolder(0x00, 0x00, 0x00);

	void resetThings(double x, double y, double a, char id, std::string name);

	double shootCount = 0;
	double maxShootCount = 200; //temporary? //should be 100

	//std::vector<double> getCannonPoints();

	//std::vector<Power> powers;
	/*
	this.shootCount = 0,
	this.shootMultiplier = 1,
	this.powerMultiplier = 10,

	this.powerCount = [],
	this.maxPowerCount = [],
	this.power = new PowerStuff(),
	this.trapped = false,
	this.specialShooting = 0,
	this.colorDefault = normalColor,

	this.shootingPower = new ShotPowerStuff(),
	this.shotSizeMultiplier = 1,
	this.shotVelocityMultiplier = 1,

	*/

public:
	//helper stuff:
	ColorValueHolder getBodyColor();

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
	void drawName();
	void drawName(double xpos, double ypos);
	char getID() { return id; }
	std::string getName() { return name; }
	
	~Tank();
};