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
	std::vector<ExtraCannonPoint> extraShootingPoints;
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

protected:
	void makeBulletCommon(double x, double y, double angle, double radius, double speed);
public:
	void makeBullet(double x, double y, double angle, double radius, double speed, double acc); //does not use makeBulletCommon (avoid using)
	void defaultMakeBullet(double angle); //simple shoot: bullet points away from tank center at a given angle
	void defaultMakeBullet(double angle, double edgeAngleOffset); //same but control over the firing angle
	void preciseMakeBullet(double x_offset, double y_offset, double angle); //make bullet x and y dist from tank, moving with angle; basically just for mines

protected:
	ColorValueHolder defaultColor; // = ColorValueHolder(0.5f, 0.5f, 0.5f); //JS: #888888
	//ColorValueHolder defaultNameFill = ColorValueHolder(1.0f, 1.0f, 1.0f);
	//ColorValueHolder defaultNameStroke = ColorValueHolder(0.0f, 0.0f, 0.0f);

	bool kill(); //allows for custom death (a.k.a. something saving the tank from death)
	void kill_hard(); //kills without accounting for extra lives
	inline void terminalVelocity(bool forward);
	inline void move_base(bool forward, bool turnL, bool turnR);
	inline void determineShootingAngles_helper(std::vector<double>* newCannonPoints);
	inline double getEvaluatedCannonAngle(unsigned int index) const;
	inline double getEvaluatedCannonAngle(unsigned int indexRegular, unsigned int indexExtra) const;
	inline double getEvaluatedCannonAngleWithEdge(unsigned int indexRegular, unsigned int indexExtra) const;

public:
	//helper stuff:
	ColorValueHolder getBodyColor() const;
	std::string getName() const { return name; }

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

protected:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawBodyDead(float alpha = 1.0f) const; //probably doesn't need alpha
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawShootingCooldown(float alpha = 1.0f) const;
	inline void drawPowerCooldown(float alpha = 1.0f) const;
	inline void drawMainBarrel(float alpha = 1.0f) const;
	inline void drawExtraBarrels(float alpha = 1.0f) const;
	inline void drawExtraExtraBarrels(float alpha = 1.0f) const;

private:
	static SimpleVector2D body_vertices[Circle::NumOfSides+1];
	static unsigned int body_indices[Circle::NumOfSides*3];
	static unsigned int outline_indices[Circle::NumOfSides*2*3];
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	Tank(double x, double y, double angle, Team_ID id, std::string name, double shootCooldown);
	~Tank();
};
