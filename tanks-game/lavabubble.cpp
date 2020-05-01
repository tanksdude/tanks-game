#pragma once
#include "lavabubble.h"
#include <limits> //for std::numeric_limits<double>::max();
#include <algorithm> //for std::copy

LavaBubble::LavaBubble(float radius, float x0, float y0, float x1, float y1, double tickStart) {
	r = radius;
	xStart = x0;
	xEnd = x1;
	yStart = y0;
	yEnd = y1;

	tickCount = 0;
	tickMax = tickStart;
	state = 0;

	double temp[] = { 1, 1, 1, std::numeric_limits<double>::max() };
	std::copy(temp, temp+4, stateMultiplier); //screw you, C++
}

LavaBubble::LavaBubble(float radius, float x0, float y0, float x1, float y1, double tickStart, double* tickMultiplier)
: LavaBubble(radius, x0, y0, x1, y1, tickStart) {
	std::copy(tickMultiplier, tickMultiplier+4, stateMultiplier);
}

void LavaBubble::tick() {
	tickCount++;
	if (tickCount >= tickMax * stateMultiplier[state]) {
		tickCount -= tickMax * stateMultiplier[state];
		state++;
	}
}

bool LavaBubble::isDead() {
	return (state >= 3);
}

double LavaBubble::getAlpha() {
	switch (state) {
		case 0: return tickCount / (tickMax * stateMultiplier[0]) * .5;
		case 1: return .5;
		case 2: return (1 - tickCount / (tickMax * stateMultiplier[2])) * .5;
		default: return 0;
	}
}

float LavaBubble::getX() {
	switch (state) {
		case 0: return xStart;
		case 1: return xStart + (xEnd - xStart) * tickCount/(tickMax * stateMultiplier[1]);
		case 2: return xEnd;
		default: return xEnd;
	}
}

float LavaBubble::getY() {
	switch (state) {
		case 0: return yStart;
		case 1: return yStart + (yEnd - yStart) * tickCount/(tickMax * stateMultiplier[1]);
		case 2: return yEnd;
		default: return yEnd;
	}
}