#pragma once
class Tank;

#include "game-thing.h"
#include "drawable-thing.h"
#include "circle.h"
#include <string>
#include <vector>
#include "color-value-holder.h"
#include "cannon-point.h"
#include "tank-power.h"
#include "simple-vector-2d.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

struct TankInputChar {
	bool isSpecial;
	int character;
	bool getKeyState() const;
	TankInputChar(bool, int);
	TankInputChar();
};

class Tank : public Circle, public GameThing, public DrawableThing {
	friend class ResetThings;
	friend class PowerFunctionHelper;
	friend class EndGameHandler; //calls this->kill()

public:
	SimpleVector2D velocity;
	double maxSpeed; // = 1;
	double acceleration; // = 1.0/16; //intentional acceleration, not total
	double turningIncrement; // = 64;
	std::vector<CannonPoint> shootingPoints;
	std::vector<TankPower*> tankPowers;
	double shootCount;
	double maxShootCount;
	bool dead = false; //only kill() should modify this

	double getOffenseTier() const;
	double getDefenseTier() const;

protected:
	double getHighestOffenseImportance() const;
	double getHighestOffenseTier(double importance) const;
	double getHighestDefenseImportance() const;
	double getHighestDefenseTier(double importance) const;

public:
	//double shootingSpeedMultiplier = 1;
	double getShootingSpeedMultiplier() const;
	//double powerMultiplier; //would be used for an ini

public:
	void determineShootingAngles();
	void updateAllValues(); //this is supposed to update all values that can get affected by powers, such as maxSpeed and acceleration
	void updateMaxSpeed();
	void updateAcceleration();
	void updateRadius();
	void updateTurningIncrement();

protected:
	std::string name;
	TankInputChar forward;
	TankInputChar turnL;
	TankInputChar turnR;
	TankInputChar shooting;
	//TankInputChar backwards; //not the point of the game
	TankInputChar specialKey;

protected:
	void makeBulletCommon(double x, double y, double angle, double radius, double speed);
public:
	void makeBullet(double x, double y, double angle, double radius, double speed, double acc); //move to private eventually (does not use makeBulletCommon) (avoid using)
	void defaultMakeBullet(double angle); //simple shoot: bullet points away from tank center at a given angle
	void regularMakeBullet(double x_offset, double y_offset, double angle); //make bullet x and y dist from tank, moving with angle

protected:
	ColorValueHolder defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f); //JS: #888888
	ColorValueHolder defaultNameFill = ColorValueHolder(1.0f, 1.0f, 1.0f);
	ColorValueHolder defaultNameStroke = ColorValueHolder(0.0f, 0.0f, 0.0f);

	bool kill(); //allows for custom death (a.k.a. something saving the tank from death)
	void kill_hard(); //kills without accounting for extra lives
	inline void terminalVelocity();
	inline void move_base();
	//void resetThings(double x, double y, double angle, Team_ID teamID);

public:
	//helper stuff:
	ColorValueHolder getBodyColor() const;
	std::string getName() const { return name; }
	TankInputChar* getKeys() const;

	double getAngle() const;
	double getCannonAngle(int index) const;
	double getRealCannonAngle(int index) const;

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
	bool move();
	void shoot();
	void powerCalculate();
	void removePower(int index);
	void powerReset();

	void draw() const override;
	void draw(DrawingLayers) const override;
	void poseDraw() const override;
	void poseDraw(DrawingLayers) const override;
	void ghostDraw(float alpha) const override;
	void ghostDraw(DrawingLayers, float alpha) const override;
	//void drawCPU() const;
	//void drawCPU(double, double) const;

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawDead(float alpha = 1.0f) const; //probably doesn't need alpha
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawShootingCooldown(float alpha = 1.0f) const;
	inline void drawPowerCooldown(float alpha = 1.0f) const;
	inline void drawMainBarrel(float alpha = 1.0f) const;
	inline void drawExtraBarrels(float alpha = 1.0f) const;

public:
	Tank(double x, double y, double angle, Team_ID id, std::string name, TankInputChar* inputs);
	Tank(double x, double y, double angle, Team_ID id, std::string name, TankInputChar forward, TankInputChar left, TankInputChar right, TankInputChar shoot, TankInputChar special);
	~Tank();
};
