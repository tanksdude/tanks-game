#pragma once
#include "../power.h"

class GodmodePower : public Power {
public:
	static inline int getHueValue();

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "old", "ultimate", "ultimate-vanilla" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes; //not supposed to mix with other powers
		return attributes;
	}

	virtual std::string getName() const override { return GodmodePower::getClassName(); }
	static std::string getClassName() { return "godmode"; } //"godMode" in JS Tanks
	virtual ColorValueHolder getColor() const override { return GodmodePower::getClassColor(); }
	static ColorValueHolder getClassColor(); //rainbow!
	virtual float getColorImportance() const override { return GodmodePower::getClassColorImportance(); }
	static float getClassColorImportance() { return 1; }

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	GodmodePower();
	static Power* factory();
};



class GodmodeTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override { return GodmodePower::getClassColor(); }
	virtual float getColorImportance() const override { return GodmodePower::getClassColorImportance(); }

	virtual TankPower* makeDuplicate() const override { return new GodmodeTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	GodmodeTankPower();
};



class GodmodeBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return GodmodePower::getClassColor(); }
	virtual float getColorImportance() const override { return GodmodePower::getClassColorImportance(); }

	virtual BulletPower* makeDuplicate() const override { return new GodmodeBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	GodmodeBulletPower();
};
