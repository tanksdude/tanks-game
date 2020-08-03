#pragma once
class Bullet;

#include "gamething.h"
#include "circle.h"
#include "colorvalueholder.h"
#include "bulletpower.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

enum class BulletParentType {
	team, //tanks
	individual, //hazards
	name, //certain hazards?
	none //anything?
};

class Bullet : public Circle, public GameThing {
	friend class ResetThings;
	friend class PowerFunctionHelper;
public: //hopefully temporary
	double angle;
	double velocity;
	double initial_velocity;
	double acceleration;
	long parentID; //may not be used, depends on parentType
	long getParentID() const { return parentID; }
	BulletParentType parentType;
	BulletParentType getParentIDType() const { return parentType; }
	bool canCollideWith(GameThing*) const;
	bool canCollideWith(Bullet*) const;

	ColorValueHolder defaultColor = ColorValueHolder(.5f, .5f, .5f);
	//ColorValueHolder* explosionColor; //needed?
	double getAngle() const;
	double getInitialVelocity() const { return initial_velocity; }
	double alpha; //[0,100] to avoid minor float imprecision
	bool isDead() const;

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
	double getBulletSpeedMultiplier();
	double getBulletRadiusMultiplier();
	double getBulletAcceleration();

public:
	//helper functions:
	ColorValueHolder getColor() const;

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
	void drawBody(double, double);
	void drawOutline(double, double);
	
private:
	Bullet(double x, double y, double a, char teamID, BulletParentType parentType, long parentID); //every bullet uses this
	Bullet(double x, double y, double a, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp); //most bullets use this
public:
	Bullet(double x, double y, double r, double a, double vel, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp, bool lessOverriding); //basically just for banana
	Bullet(double x, double y, double r, double a, double vel, double acc, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp, bool manualAcceleration); //avoid using
public:
	Bullet(double x, double y, double r, double a, double vel, char teamID, BulletParentType parentType, long parentID);
	Bullet(double x, double y, double r, double a, double vel, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp);
	void move();
	void draw();
	void draw(double, double);
	void drawCPU();
	void drawCPU(double, double);
	//short determineDamage(); //maybe for another day

	void powerCalculate();
	void removePower(int index);

	bool isFullyOutOfBounds();
	bool isPartiallyOutOfBounds();
	//void edgeConstrain(); //should never be needed

	~Bullet();
};
