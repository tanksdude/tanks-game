#include "generalizednobulletzone.h"
#include "backgroundrect.h"
#include "colormixer.h"

ColorValueHolder GeneralizedNoBulletZone::getColor() const {
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.5f, .5f, .5f), .125);
}

void GeneralizedNoBulletZone::tick() {
	return;
}
