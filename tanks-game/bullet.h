#pragma once
class Bullet;

#include "gamething.h"
#include "drawablething.h"
#include "circle.h"
#include "colorvalueholder.h"
#include "bulletpower.h"
#include "simplevector2d.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

enum class BulletParentType {
	team, //tanks
	individual, //hazards
	name, //certain hazards?
	none //anything?
};

class Bullet : public Circle, public GameThing, public DrawableThing {
	friend class ResetThings;
	friend class PowerFunctionHelper;

public: //hopefully temporary
	SimpleVector2D velocity;
	double initial_velocity;
	double acceleration;
	Game_ID parentID; //may not be used, depends on parentType
	BulletParentType parentType;
	std::vector<BulletPower*> bulletPowers; //change eventually?
	double alpha; //[0,100] to avoid minor float imprecision

public:
	double getOffenseTier() const;
	double getDefenseTier() const;

protected:
	double getHighestOffenseImportance() const;
	double getHighestOffenseTier(double importance) const;
	double getHighestDefenseImportance() const;
	double getHighestDefenseTier(double importance) const;

public:
	double getBulletSpeedMultiplier() const;
	double getBulletRadiusMultiplier() const;
	double getBulletAcceleration() const;

protected:
	ColorValueHolder defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f);
public:
	bool kill(); //allows for custom death (a.k.a. something saving the bullet from death)

public:
	//helper functions:
	ColorValueHolder getColor() const;
	double getAngle() const;
	double getInitialVelocity() const { return initial_velocity; }
	Game_ID getParentID() const { return parentID; }
	BulletParentType getParentIDType() const { return parentType; }
	bool isDead() const { return (alpha <= 0); }

	static const double default_radius;

private:
	//IMPORTANT: GLEW must be initialized before these are set
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;

	static bool initializeGPU(); //returns whether it successfully initialized (false if it was already initialized)
	static bool uninitializeGPU();

public:
	void move();
	void powerCalculate();
	void removePower(int index);
	//short determineDamage(); //maybe for another day
	bool canCollideWith(const GameThing*) const;
	bool canCollideWith(const Bullet*) const;

	void draw() const override;
	void draw(double xpos, double ypos) const override;
	void poseDraw() const override;
	void drawBody(double, double) const;
	void drawOutline(double, double) const;
	void drawCPU() const;
	void drawCPU(double, double) const;

private:
	Bullet(double x, double y, double a, Team_ID teamID, BulletParentType parentType, Game_ID parentID); //every bullet uses this
	Bullet(double x, double y, double a, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp); //most bullets use this
public:
	Bullet(double x, double y, double r, double a, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool lessOverriding); //basically just for banana
	Bullet(double x, double y, double r, double a, double vel, double acc, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool manualAcceleration); //avoid using
public:
	Bullet(double x, double y, double r, double a, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID);
	Bullet(double x, double y, double r, double a, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp);
	~Bullet();
};
