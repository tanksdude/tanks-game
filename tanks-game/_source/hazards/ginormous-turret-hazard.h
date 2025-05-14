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
		//not sure whether these should be public or not, but there needs be some kind of protection
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
		virtual void drawReticule(float alpha = 1.0f) const override;

	public:
		MinionTurret(double xpos, double ypos, double angle, Game_ID parentID);
		virtual ~MinionTurret();
		static CircleHazard* factory(const GenericFactoryConstructionData&);
	};

protected:
	virtual void turnTowardsTank(const Tank*) override; //turns twice as fast when not making children (this sounds weird)

	virtual CircleHazard* makeTurret(int turretNum) const override;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual float getOffenseTier() const override { return 0; }
	virtual float getDefenseTier() const override { return DESTRUCTION_TIER + 1.0f; }

public:
	//virtual void uninitialize() override; //tells children to stop targeting

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "ginormous_turret"; }

	virtual void tick() override;

protected:
	virtual void tick_notifyChildren(Game_ID id);
	virtual void tick_stopChildren();

protected:
	//same as StationaryTurretHazard but with a thicker outline
	virtual void drawOutline(float alpha = 1.0f) const override;
	virtual void drawBarrel(float alpha = 1.0f) const override;

public:
	GinormousTurretHazard(double xpos, double ypos, double angle);
	GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren);
	GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier);
	virtual ~GinormousTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
};
