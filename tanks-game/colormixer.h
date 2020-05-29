#pragma once
#include "colorvalueholder.h"
#include "power.h"
#include <vector>
#include "tankpower.h"
#include "bulletpower.h"

class ColorMixer {
public:
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder);
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder, float amt);
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder, double amt);
	static ColorValueHolder mix(ColorValueHolder*, int num);
	static ColorValueHolder mix(Power**, int num);
	static ColorValueHolder mix(std::vector<TankPower*>*);
	static ColorValueHolder mix(std::vector<BulletPower*>*);
	//static ColorValueHolder mix(std::vector<HazardPower*>*);
};
