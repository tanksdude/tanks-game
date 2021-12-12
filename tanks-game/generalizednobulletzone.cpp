#include "generalizednobulletzone.h"
#include "backgroundrect.h"
#include "colormixer.h"

ColorValueHolder GeneralizedNoBulletZone::getColor() const {
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.5f, 0.5f, 0.5f), .125);
}

void GeneralizedNoBulletZone::tick() {
	return;
}
