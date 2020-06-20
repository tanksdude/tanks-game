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
