#pragma once
#include "circle.h"
#include "colorvalueholder.h"
#include <string>
#include <vector>
#include "resetthings.h"

class Tank : public Circle {
	friend class ResetThings;
	friend class Level; //might remove
	char id;
	std::string name;

	int wins = 0;

	double maxSpeed = 1;
	double acceleration = 1.0/16; //intentional acceleration, not total
	double velocity = 0; //intentional velocity, not total
	double turningIncrement = 64;
	double angle;

public:
	bool turnL;
	bool turnR;
	bool forward;
	bool shooting;
	//bool backwards; //not the point of the game

private:
	ColorValueHolder defaultColor = ColorValueHolder(0x88, 0x88, 0x88);
	bool dead = false;
	ColorValueHolder* explosionColor;
	ColorValueHolder defaultNameFill = ColorValueHolder(0xFF, 0xFF, 0xFF);
	ColorValueHolder defaultNameStroke = ColorValueHolder(0x00, 0x00, 0x00);

	void resetThings(double x, double y, double a, char id, std::string name);

	double shootCount = 0;
	double maxShootCount = 200; //temporary?

	//CannonPoint thing; //position on where to shoot, just needs radius input

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
	Tank(double x, double y, double a, char id, std::string name);
	Tank();

	void move();
	void terminalVelocity(); //move to protected
	void edgeConstrain(); //hitEdge but doesn't use a "WallEdge"
	bool isPartiallyOutOfBounds();
	bool isFullyOutOfBounds();
	void shoot();
	void powerCalculate();
	void removePower();
	void powerReset();
	void draw();
	void draw(double xpos, double ypos);
	void drawName();
	void drawName(double xpos, double ypos);
	char getID() { return id; }
	std::string getName() { return name; }
	
	~Tank();
};