#pragma once
#include "color-value-holder.h"

class BackgroundRect {
private:
	static ColorValueHolder backColor;

public:
	static ColorValueHolder getBackColor();

	static void draw();

private:
	BackgroundRect() {}
	BackgroundRect(const BackgroundRect&) {}
};
