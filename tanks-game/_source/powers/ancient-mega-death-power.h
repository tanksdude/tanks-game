#pragma once
#include "../power.h"

class AncientMegaDeathPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		//not in old because it never saw the light of day
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return AncientMegaDeathPower::getClassName(); }
	static std::string getClassName() { return "ancient_megadeath"; }
	virtual std::string getIdentifier() const override { return AncientMegaDeathPower::getClassIdentifier(); }
	static std::string getClassIdentifier() { return AncientMegaDeathPower::getClassName(); }
	virtual ColorValueHolder getColor() const override { return AncientMegaDeathPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x20/255.0, 0x1A/255.0, 0x1A/255.0); } //JS unused alternate for megadeath

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	AncientMegaDeathPower();
	static Power* factory();
};



class AncientMegaDeathTankPower : public TankPower {
public:
	virtual std::string getIdentifier() const override { return AncientMegaDeathPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return AncientMegaDeathPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new AncientMegaDeathTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankFiringRateMultiplier() const override { return 2; }

	AncientMegaDeathTankPower();
};



#include "../constants.h"

class AncientMegaDeathBulletPower : public BulletPower {
public:
	virtual std::string getIdentifier() const override { return AncientMegaDeathPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return AncientMegaDeathPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new AncientMegaDeathBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	virtual double getBulletSpeedMultiplier() const override { return 1.0/64; }
	virtual double getBulletAcceleration() const override { return 1.0/64; } //I think the original had multiplicative or increasing acceleration, because it was a lot more aggressive than this

	virtual float getOffenseTier(const Bullet*) const override { return DESTRUCTION_TIER; } //maybe damage could increase with distance like the Staff from Smash Bros Ultimate
	virtual float getDefenseTier(const Bullet*) const override { return DESTRUCTION_TIER; } //no really, that's its name, I looked it up: https://www.ssbwiki.com/Staff

	AncientMegaDeathBulletPower();
};
