#pragma once
#include "colorvalueholder.h"

class ColorMixer {
public:
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder);
	static ColorValueHolder mix(ColorValueHolder, ColorValueHolder, float amt);
	static ColorValueHolder mix(ColorValueHolder*, int num);
	static ColorValueHolder HSVtoRGB(float hue, float saturation, float value); //[0,360], [0,1], [0,1]
};
