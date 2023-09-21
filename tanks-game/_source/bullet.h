#pragma once
class Bullet;
struct BulletUpdateStruct;

#include <vector>

#include "game-thing.h"
#include "circle.h"
#include "drawable-thing.h"
#include "bullet-power.h"

#include "color-value-holder.h"
#include "simple-vector-2d.h"

enum class BulletParentType {
	team, //tanks
	individual, //hazards
	name, //certain hazards?
	none //anything?
};

class Bullet : public GameThing, public Circle, public DrawableThing {
	friend class ResetThings;
	friend class PowerFunctionHelper;
	friend class EndGameHandler;

public: //hopefully temporary
	SimpleVector2D velocity;
	double initial_velocity;
	double acceleration;
	Game_ID parentID; //may not be used, depends on parentType
	BulletParentType parentType;
	std::vector<BulletPower*> bulletPowers; //change eventually?
	double lifeValue; //[0,100], controls transparency (0=nothing, 100=opaque)

public:
	float getOffenseTier() const;
	float getDefenseTier() const;

protected:
	float getHighestOffenseImportance() const;
	float getHighestOffenseTier(float importance) const;
	float getHighestDefenseImportance() const;
	float getHighestDefenseTier(float importance) const;

public:
	double getBulletSpeedMultiplier() const;
	double getBulletRadiusMultiplier() const;
	double getBulletAcceleration() const;

protected:
	ColorValueHolder defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f);
	bool kill(); //allows for custom death (a.k.a. something saving the bullet from death)
	inline void move_base();

public:
	//helper functions:
	ColorValueHolder getColor() const;
	double getAngle() const; //TODO: remove
	double getInitialVelocity() const { return initial_velocity; }
	Game_ID getParentID() const { return parentID; }
	BulletParentType getParentIDType() const { return parentType; }
	bool isDead() const { return (lifeValue <= 0); }

	static const double default_radius;

public:
	bool move();
	void powerCalculate();
	void removePower(int index);
	//int determineDamage(); //maybe for another day

	bool canCollideWith(const GameThing*) const;
	bool canCollideWith(const Bullet*) const;
	void update(const BulletUpdateStruct*);

	void draw() const override;
	void draw(DrawingLayers) const override;
	void poseDraw() const override;
	void poseDraw(DrawingLayers) const override;
	void ghostDraw(float alpha) const override;
	void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawDeathCooldown(float alpha = 1.0f) const;

private:
	Bullet(double x, double y, double angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID); //every bullet uses this
	Bullet(double x, double y, double angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp); //most bullets use this
public:
	Bullet(double x, double y, double r, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool lessOverriding); //basically just for banana
	Bullet(double x, double y, double r, double angle, double vel, double acc, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool manualAcceleration); //avoid using
public:
	Bullet(double x, double y, double r, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID);
	Bullet(double x, double y, double r, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp);
	~Bullet();
};

struct BulletUpdateStruct {
	//deltas:
	double x;
	double y;
	double r;
	double speed;
	double angle;
	double alpha;
	//absolutes:
	//double acceleration;
	std::vector<Bullet*> newBullets;

	void add(const BulletUpdateStruct& other);

	BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha, const std::vector<Bullet*>& newBullets);
	BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha);
	BulletUpdateStruct() : BulletUpdateStruct(0, 0, 0, 0, 0, 0) {}
};
