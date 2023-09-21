#pragma once
#include "mother-turret-hazard.h"

class GinormousTurretHazard : public MotherTurretHazard {
protected:
	class MinionTurret : public TargetingTurretHazard {
		//follows orders from its parent GinormousTurretHazard
		//name should be MinionTurretHazard, but oh well

	protected:
		Game_ID parentID;

	public:
		//not sure whether these should be public or not, but there should be some kind of protection
		virtual void setTarget(Game_ID parentVerification, Game_ID target);
		virtual void unsetTarget(Game_ID parentVerification);

	protected:
		virtual bool canSeeTank(const Tank*) const override; //override: always true if it's the targeted tank

	public:
		virtual std::vector<std::string> getHazardTypes() const override {
			std::vector<std::string> types = std::vector<std::string>{ "vanilla", "dev" };
			return types;
		}
		virtual std::unordered_map<std::string, float> getWeights() const override;

	public:
		virtual std::string getName() const override { return getClassName(); }
		static std::string getClassName() { return "minion_turret"; }

		virtual void tick() override;

	protected:
		virtual inline void drawReticule(float alpha = 1.0f) const;

	public:
		MinionTurret(double xpos, double ypos, double angle, Game_ID parentID);
		virtual ~MinionTurret();
		static CircleHazard* factory(const GenericFactoryConstructionData&);
		//static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	};

protected:
	//SimpleVector2D velocity; //for stationary and targeting turrets, the magnitude will obviously be 0
	//double tickCount;
	//double tickCycle;
	//unsigned int currentState;
	//unsigned int maxState;
	//double* stateMultiplier;
	//ColorValueHolder* stateColors;

	//double turningIncrement = 128;
	//bool targeting;
	//double targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	//ColorValueHolder reticuleColors[2];
	//double targetingCount;
	//Game_ID trackingID; //if ==this->getGameID(), then it's not tracking

protected:
	virtual void turnTowardsTank(const Tank*) override; //turns twice as fast when not making children (this sounds weird)

	virtual CircleHazard* makeTurret(int turretNum) const override;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return DESTRUCTION_TIER + 1.0; }

public:
	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "ginormous_turret"; }

	virtual void tick() override;

protected:
	virtual inline void tick_notifyChildren(Game_ID id);
	virtual inline void tick_stopChildren();

protected:
	//same as StationaryTurretHazard but with a thicker outline
	virtual inline void drawOutline(float alpha = 1.0f) const;
	virtual inline void drawBarrel(float alpha = 1.0f) const;

public:
	GinormousTurretHazard(double xpos, double ypos, double angle);
	GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren);
	GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier);
	virtual ~GinormousTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	//virtual int getFactoryArgumentCount() const override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; } //probably doesn't need an angle...
	//virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
