#pragma once
class TripleBulletPower;

#include "bulletpower.h"
#include "triplepower.h"

class TripleBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TriplePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TripleBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	TripleBulletPower();
};
