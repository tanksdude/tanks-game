#include "background-rect.h"

#include "constants.h"

#include "renderer.h"

ColorValueHolder BackgroundRect::backColor = ColorValueHolder(0xDD/255.0, 0xDD/255.0, 0xDD/255.0);
//ColorValueHolder BackgroundRect::backColor = ColorValueHolder(0.875f, 0.875f, 0.875f);

ColorValueHolder BackgroundRect::getBackColor() {
	return backColor;
}

void BackgroundRect::draw() {
	ColorValueHolder color = BackgroundRect::getBackColor();

	float background_positions[] = {
		0,          0,             color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		GAME_WIDTH, 0,             color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		GAME_WIDTH, GAME_HEIGHT,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		0,          GAME_HEIGHT,   color.getRf(), color.getGf(), color.getBf(), color.getAf()
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(background_positions, 4 * (2+4), background_indices, 2 * 3);
}
