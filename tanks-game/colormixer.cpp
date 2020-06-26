#include "colormixer.h"
#include <iostream>

//TODO: account for colors to contain alpha

ColorValueHolder ColorMixer::mix(ColorValueHolder a, ColorValueHolder b) {
	return ColorValueHolder((a.getRf() + b.getRf())/2, (a.getGf() + b.getGf())/2, (a.getBf() + b.getBf())/2);
}

ColorValueHolder ColorMixer::mix(ColorValueHolder a, ColorValueHolder b, float amt) {
	return ColorValueHolder((a.getRf() + (b.getRf() - a.getRf())*amt), (a.getGf() + (b.getGf() - a.getGf())*amt), (a.getBf() + (b.getBf() - a.getBf())*amt));
}

ColorValueHolder ColorMixer::mix(ColorValueHolder* c, int num) {
	float* colors = new float[3]{ 0,0,0 };
	for (int i = 0; i < num; i++) {
		colors[0] += c[i].getRf();
		colors[1] += c[i].getGf();
		colors[2] += c[i].getBf();
	}

	ColorValueHolder mixed = ColorValueHolder(colors[0]/num, colors[1]/num, colors[2]/num);
	delete[] colors;

	return mixed;
}

ColorValueHolder ColorMixer::HSVtoRGB(float hue, float saturation, float value) {
	//got this from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
	float c = saturation * value;
	float hPrime = hue / 60;
	float x = c * (1 - abs(fmod(hPrime,2) - 1));
	float R, G, B;
	if (hPrime <= 1) {
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
