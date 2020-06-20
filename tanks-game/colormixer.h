#pragma once
#include "colorvalueholder.h"

class ColorMixer {
public:
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder);
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder, float amt);
	static ColorValueHolder mix(ColorValueHolder*, int num);
};
