#pragma once
class Tank;

#include "gamething.h"
#include "drawablething.h"
#include "circle.h"
#include <string>
#include <vector>
#include "colorvalueholder.h"
#include "cannonpoint.h"
#include "tankpower.h"
#include "simplevector2d.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "shader.h"

struct TankInputChar {
	bool isSpecial;
	int character;
	bool getKeyState();
	TankInputChar(bool, int);
	TankInputChar();
};

class Tank : public Circle, public GameThing, public DrawableThing {
	friend class ResetThings;
	friend class PowerFunctionHelper;
	friend class EndGameHandler; //calls this->kill()
public:
	std::string name;

	double maxSpeed; // = 1;
	double acceleration; // = 1.0/16; //intentional acceleration, not total
	SimpleVector2D velocity;
	double turningIncrement; // = 64;
	double getAngle() const;
	double getCannonAngle(int index) const;
	double getRealCannonAngle(int index) const;
	std::vector<CannonPoint>* shootingPoints;

	std::vector<TankPower*> tankPowers;
	double getOffenseTier();
	double getDefenseTier();
protected:
	double getHighestOffenseImportance();
	double getHighestOffenseTier(double importance);
	double getHighestDefenseImportance();
	double getHighestDefenseTier(double importance);

public:
	//double shootingSpeedMultiplier = 1;
	double getShootingSpeedMultiplier() const;
	//double powerMultiplier; //would be used for an ini

	void updateAllValues(); //this is supposed to update all values that can get affected by powers, such as maxSpeed and acceleration
	void updateMaxSpeed();
	void updateAcceleration();
	void updateRadius();
	void updateTurningIncrement();

public:
	TankInputChar forward;
	TankInputChar turnL;
	TankInputChar turnR;
	TankInputChar shooting;
	//TankInputChar backwards; //not the point of the game
	TankInputChar specialKey;

	void makeBulletCommon(double x, double y, double angle, double radius, double speed); //move to private eventually
	void makeBullet(double x, double y, double angle, double radius, double speed, double acc); //move to private eventually (does not use makeBulletCommon) (avoid using)
	void defaultMakeBullet(double angle); //simple shoot: bullet points away from tank center at a given angle
	void regularMakeBullet(double x_offset, double y_offset, double angle); //make bullet x and y dist from tank, moving with angle
	void determineShootingAngles();

protected:
	ColorValueHolder defaultColor = ColorValueHolder(.5f, .5f, .5f);
	ColorValueHolder defaultNameFill = ColorValueHolder(1.0f, 1.0f, 1.0f);
	ColorValueHolder defaultNameStroke = ColorValueHolder(0, 0, 0);

	bool dead = false; //only kill() should modify this
	bool kill(); //allows for custom death (aka something saving the tank from death)

	void resetThings(double x, double y, double a, char teamID);
	void terminalVelocity();

	double shootCount;
	double maxShootCount;

public:
	//helper stuff:
	ColorValueHolder getBodyColor() const;

	static const double default_maxSpeed;
	static const double default_acceleration;
	static const double default_turningIncrement;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static VertexArray* cannon_va;
	static VertexBuffer* cannon_vb;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	Tank(double x, double y, double a, char id, std::string name, TankInputChar forward, TankInputChar left, TankInputChar right, TankInputChar shoot, TankInputChar special);

	void move();
	//void edgeConstrain();
	//bool isPartiallyOutOfBounds();
	//bool isFullyOutOfBounds();
	void shoot();
	void powerCalculate();
	void removePower(int index);
	void powerReset();
	void draw() const override;
	void draw(double xpos, double ypos) const override;
	void poseDraw() const override;
	void drawCPU() const;
	void drawCPU(double, double) const;
	std::string getName() { return name; }

	~Tank();
};
