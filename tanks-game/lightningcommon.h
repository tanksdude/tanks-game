#pragma once
#include <vector>

struct LightningBolt {
	std::vector<float> positions; //positions is array of (x,y) points (for easy vertex streaming)
	int length; //positions.size()/2, unless it's uninitialized
	LightningBolt() { length = 0; } //don't use
	LightningBolt(int l) { //try not to use
		length = l;
		positions.reserve(l*2);
	}
	LightningBolt(float startX, float startY, float endX, float endY, int l) {
		length = l;
		positions.reserve(l*2);
		positions.push_back(startX); positions.push_back(startY);
		for (int i = 1; i < l-1; i++) {
			positions.push_back(startX + (endX-startX) * float(i)/(l-1));
			positions.push_back(startY + (endY-startY) * float(i)/(l-1));
		}
		positions.push_back(endX); positions.push_back(endY);
	}
};
