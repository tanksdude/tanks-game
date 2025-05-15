#include "color-mixer.h"

#include <cmath> //abs, fmod
#include <iostream>

ColorValueHolder ColorMixer::mix(const ColorValueHolder& a, const ColorValueHolder& b) {
	return ColorValueHolder((a.getRf() + b.getRf())/2, (a.getGf() + b.getGf())/2, (a.getBf() + b.getBf())/2);
}

ColorValueHolder ColorMixer::mix(const ColorValueHolder& a, const ColorValueHolder& b, float amt) {
	return ColorValueHolder((a.getRf() + (b.getRf() - a.getRf())*amt), (a.getGf() + (b.getGf() - a.getGf())*amt), (a.getBf() + (b.getBf() - a.getBf())*amt));
}

ColorValueHolder ColorMixer::mix(const ColorValueHolder* c, int num) {
	float colors[3] = { 0, 0, 0 };
	for (int i = 0; i < num; i++) {
		colors[0] += c[i].getRf();
		colors[1] += c[i].getGf();
		colors[2] += c[i].getBf();
	}

	return ColorValueHolder(colors[0]/num, colors[1]/num, colors[2]/num);
}

ColorValueHolder ColorMixer::HSVtoRGB(float hue, float saturation, float value) {
	//got this from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
	float c = saturation * value;
	float hPrime = hue / 60;
	float x = c * (1 - std::abs(std::fmod(hPrime,2) - 1));
	float R, G, B;
	if (hPrime < 1) {
		R=c, G=x, B=0;
	} else if (hPrime < 2) {
		R=x, G=c, B=0;
	} else if (hPrime < 3) {
		R=0, G=c, B=x;
	} else if (hPrime < 4) {
		R=0, G=x, B=c;
	} else if (hPrime < 5) {
		R=x, G=0, B=c;
	} else {
		R=c, G=0, B=x;
	}
	float m = value - c;
	return ColorValueHolder(R+m, G+m, B+m);
}

ColorValueHolder ColorMixer::HSVtoRGB_int(int hue, float saturation, float value) {
	float c = saturation * value;
	int hPrime = hue / 60;
	float x = c * (1 - std::abs(static_cast<float>(hue % 120) / 60 - 1));
	float R, G, B;
	switch (hPrime) {
		case 0:
			R=c, G=x, B=0; break;
		case 1:
			R=x, G=c, B=0; break;
		case 2:
			R=0, G=c, B=x; break;
		case 3:
			R=0, G=x, B=c; break;
		case 4:
			R=x, G=0, B=c; break;
		case 5:
			R=c, G=0, B=x; break;
	}
	float m = value - c;
	return ColorValueHolder(R+m, G+m, B+m);
}

ColorValueHolder ColorMixer::HSLtoRGB(float hue, float saturation, float light) {
	//got this from https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
	float c = (1 - std::abs(2*light - 1)) * saturation;
	float hPrime = hue / 60;
	float x = c * (1 - std::abs(std::fmod(hPrime,2) - 1));
	float R, G, B;
	if (hPrime < 1) {
		R=c, G=x, B=0;
	} else if (hPrime < 2) {
		R=x, G=c, B=0;
	} else if (hPrime < 3) {
		R=0, G=c, B=x;
	} else if (hPrime < 4) {
		R=0, G=x, B=c;
	} else if (hPrime < 5) {
		R=x, G=0, B=c;
	} else {
		R=c, G=0, B=x;
	}
	float m = light - c/2;
	return ColorValueHolder(R+m, G+m, B+m);
}

ColorValueHolder ColorMixer::CMYKtoRGB(float cyan, float magenta, float yellow, float black) {
	//got this from https://www.101computing.net/cmyk-to-rgb-conversion-algorithm/
	float R = (1 - cyan) * (1 - black);
	float G = (1 - magenta) * (1 - black);
	float B = (1 - yellow) * (1 - black);
	return ColorValueHolder(R, G, B);
}
