#pragma once
class Tank;

#include <string>
#include <vector>

#include "game-thing.h"
#include "circle.h"
#include "drawable-thing.h"
#include "tank-power.h"

#include "color-value-holder.h"
#include "simple-vector-2d.h"
#include "cannon-point.h"

class Tank : public GameThing, public Circle, public DrawableThing {
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
	std::string name;

public:
	float getOffenseTier() const;
	float getDefenseTier() const;

protected:
	float getHighestOffenseImportance() const;
	float getHighestOffenseTier(float importance) const;
	float getHighestDefenseImportance() const;
	float getHighestDefenseTier(float importance) const;

public:
	void determineShootingAngles();
	void updateAllValues(); //this is supposed to update all values that can get affected by powers, such as maxSpeed and acceleration
	void updateMaxSpeed();
	void updateAcceleration();
	void updateRadius();
	void updateTurningIncrement();

	double getShootingSpeedMultiplier() const;
	//double powerMultiplier; //would be used for an ini

protected:
	void makeBulletCommon(double x, double y, double angle, double radius, double speed);
public:
	void makeBullet(double x, double y, double angle, double radius, double speed, double acc); //does not use makeBulletCommon (avoid using)
	void defaultMakeBullet(double angle); //simple shoot: bullet points away from tank center at a given angle
	void regularMakeBullet(double x_offset, double y_offset, double angle); //make bullet x and y dist from tank, moving with angle

protected:
	ColorValueHolder defaultColor; // = ColorValueHolder(0.5f, 0.5f, 0.5f); //JS: #888888
	//ColorValueHolder defaultNameFill = ColorValueHolder(1.0f, 1.0f, 1.0f);
	//ColorValueHolder defaultNameStroke = ColorValueHolder(0.0f, 0.0f, 0.0f);

	bool kill(); //allows for custom death (a.k.a. something saving the tank from death)
	void kill_hard(); //kills without accounting for extra lives
	inline void terminalVelocity(bool forward);
	inline void move_base(bool forward, bool turnL, bool turnR);
	inline void determineShootingAngles_helper(std::vector<double>* newCannonPoints);

public:
	//helper stuff:
	ColorValueHolder getBodyColor() const;
	std::string getName() const { return name; }

	//TODO: remove
	double getAngle() const;
	double getCannonAngle(int index) const;
	double getRealCannonAngle(int index) const;

	static const double default_maxSpeed;
	static const double default_acceleration;
	static const double default_turningIncrement;

public:
	bool move(bool forward, bool turnL, bool turnR, bool specialKey);
	void shoot(bool shooting);
	void powerCalculate();
	void removePower(int index);
	void powerReset();

	void draw() const override;
	void draw(DrawingLayers) const override;
	void poseDraw() const override;
	void poseDraw(DrawingLayers) const override;
	void ghostDraw(float alpha) const override;
	void ghostDraw(DrawingLayers, float alpha) const override;

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawDead(float alpha = 1.0f) const; //probably doesn't need alpha
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawShootingCooldown(float alpha = 1.0f) const;
	inline void drawPowerCooldown(float alpha = 1.0f) const;
	inline void drawMainBarrel(float alpha = 1.0f) const;
	inline void drawExtraBarrels(float alpha = 1.0f) const;

public:
	Tank(double x, double y, double angle, Team_ID id, std::string name, double shootCooldown);
	~Tank();
};
