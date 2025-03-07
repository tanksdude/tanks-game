#pragma once
#include "color-value-holder.h"

class BackgroundRect {
private:
	static const ColorValueHolder backColor;

public:
	static const ColorValueHolder& getBackColor() { return backColor; }

	static void draw();

private:
	BackgroundRect() = delete;
	BackgroundRect(const BackgroundRect&) = delete;
};
