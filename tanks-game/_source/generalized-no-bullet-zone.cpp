#include "generalized-no-bullet-zone.h"

#include "color-mixer.h"
#include "background-rect.h"

const float GeneralizedNoBulletZone::X_WIDTH = 0.1;
const ColorValueHolder GeneralizedNoBulletZone::X_COLOR = ColorValueHolder(1.0f, 0.0f, 0.0f);

ColorValueHolder GeneralizedNoBulletZone::getColor() const {
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.5f, 0.5f, 0.5f), .125);
}

void GeneralizedNoBulletZone::tick() {
	return;
}
