#include "generalized-no-bullet-zone.h"
#include "background-rect.h"
#include "color-mixer.h"

ColorValueHolder GeneralizedNoBulletZone::getColor() const {
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.5f, 0.5f, 0.5f), .125);
}

void GeneralizedNoBulletZone::tick() {
	return;
}
