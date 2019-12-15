#pragma once
#include <iostream>
#include "colormixer.h"
#include "colorvalueholder.h"
#include "power.h"

ColorValueHolder ColorMixer::mix(ColorValueHolder a, ColorValueHolder b) {
	return ColorValueHolder((a.getR() + b.getR())/2, (a.getG() + b.getG())/2, (a.getB() + b.getB())/2);
}

ColorValueHolder ColorMixer::mix(ColorValueHolder* c, int num) {
	double* colors = new double[3]{ 0,0,0 };
	for (int i = 0; i < num; i++) {
		colors[0] += c[i].getR();
		colors[1] += c[i].getG();
		colors[2] += c[i].getB();
	}

	ColorValueHolder mixed = ColorValueHolder(colors[0]/num, colors[1]/num, colors[2]/num);
	delete[] colors;

	return mixed;
}

ColorValueHolder ColorMixer::mix(Power** p, int num) {
	double* colors = new double[3]{ 0,0,0 };
	for (int i = 0; i < num; i++) {
		ColorValueHolder a = p[i]->getColor();
		colors[0] += a.getR();
		colors[1] += a.getG();
		colors[2] += a.getB();
	}

	ColorValueHolder mixed = ColorValueHolder(colors[0]/num, colors[1]/num, colors[2]/num);
	delete[] colors;

	return mixed;
}

ColorValueHolder ColorMixer::mix(std::vector<TankPower*> p) {
	float* colors = new float[3]{ 0,0,0 };
	for (int i = 0; i < p.size(); i++) {
		ColorValueHolder a = p[i]->getColor();
		colors[0] += p[i]->getColor().getR();
		colors[1] += p[i]->getColor().getG();
		colors[2] += p[i]->getColor().getB();
	}

	ColorValueHolder mixed = ColorValueHolder(colors[0]/p.size(), colors[1]/p.size(), colors[2]/p.size());
	delete[] colors;

	//std::cout << mixed.getR() << " " << mixed.getG() << " " << mixed.getB() << std::endl;

	return mixed;
}

ColorValueHolder ColorMixer::mix(std::vector<BulletPower*> p) {
	float* colors = new float[3]{ 0,0,0 };
	for (int i = 0; i < p.size(); i++) {
		ColorValueHolder a = p[i]->getColor();
		colors[0] += p[i]->getColor().getR();
		colors[1] += p[i]->getColor().getG();
		colors[2] += p[i]->getColor().getB();
	}

	ColorValueHolder mixed = ColorValueHolder(colors[0]/p.size(), colors[1]/p.size(), colors[2]/p.size());
	delete[] colors;

	return mixed;
}