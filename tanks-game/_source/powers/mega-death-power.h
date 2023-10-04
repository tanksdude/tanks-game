#pragma once
#include "../power.h"

class MegaDeathPower : public Power {
public: //bullet stuff
	static const float destroyWallTier; //(and destroyHazardTier)
	static const double bulletSizeMultiplierPerTick;
	//difference from JS: wall degrading (doesn't exist in this version); wall degrading added acceleration again, which is negative for everything but megadeath, so it would pick up extra speed

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		//not in old
		//JS: surprisingly, did have supermix (I thought it was impossible for it to exist normally... well, whatever)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return MegaDeathPower::getClassName(); }
	static std::string getClassName() { return "megadeath"; } //"mega-death" in JS Tanks
	virtual ColorValueHolder getColor() const override { return MegaDeathPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x4F/255.0, 0x3E/255.0, 0x3E/255.0); } //brown-ish //JS unused alternate: #201A1A

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	MegaDeathPower();
	static Power* factory();
};



class MegaDeathTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MegaDeathPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new MegaDeathTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankFiringRateMultiplier() const override { return 4; }

	MegaDeathTankPower();
};



class MegaDeathBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual void tick(Bullet* b) override; //for updating wall collision bools based on size

	virtual ColorValueHolder getColor() const override {
		return MegaDeathPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new MegaDeathBulletPower(); } //should current size be passed on?
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	virtual double getBulletSpeedMultiplier() const override { return 1.0/4; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/128 or 1.0/64 might be closer to what I originally thought of
	//virtual double getBulletRadiusMultiplier() const override { return 1; }
	virtual double getBulletAcceleration() const override { return 1.0/64; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/512 or 1.0/1024 (with a smaller radius multiplier per tick) might be closer to what I orignially thought of

	virtual float getOffenseImportance() const override { return 0; }
	virtual float getOffenseTier(const Bullet*) const override;
	virtual float getDefenseImportance() const override { return 0; }
	virtual float getDefenseTier(const Bullet*) const override;

	MegaDeathBulletPower();
};
