#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class SpeedPower : public Power{

public:
	color = ColorValueHolder(0x44, 0x44, 0xFF);
	canBeInARandomLevel = true;
	canBeMixed = true;
	TankPower tp;
	BulletPower bp;
};