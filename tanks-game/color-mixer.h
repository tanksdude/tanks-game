#pragma once
#include "color-value-holder.h"

class ColorMixer {
public:
	static ColorValueHolder mix(const ColorValueHolder&, const ColorValueHolder&);
	static ColorValueHolder mix(const ColorValueHolder&, const ColorValueHolder&, float amt);
	static ColorValueHolder mix(const ColorValueHolder*, int num);
	static ColorValueHolder HSVtoRGB(float hue, float saturation, float value); //[0,360], [0,1], [0,1]
	static ColorValueHolder HSLtoRGB(float hue, float saturation, float light); //[0,360], [0,1], [0,1]
	static ColorValueHolder CMYKtoRGB(float cyan, float magenta, float yellow, float black); //[0,1]

private:
	ColorMixer() {}
	ColorMixer(const ColorMixer&) {}
};
