#include "color-cache.h"

#include "color-mixer.h"

std::unordered_map<std::string, Color_ID> ColorCacheBullet::identifierToID;
std::unordered_map<Color_ID, ColorValueHolder> ColorCacheBullet::cachedColors;
Color_ID ColorCacheBullet::nextFreeID = 0;

void ColorCacheBullet::invalidateCachedColors() {
	cachedColors.clear();
}

Color_ID ColorCacheBullet::getColorID(const std::string& identifier) {
	if (identifierToID.find(identifier) == identifierToID.end()) [[unlikely]] {
		identifierToID.insert({ identifier, ++nextFreeID });
		return nextFreeID;
	}
	return identifierToID[identifier];
}

const ColorValueHolder& ColorCacheBullet::insertColor(Color_ID id, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ id, ColorMixer::mix(colors, num) });
	return cachedColors[id];
}

const ColorValueHolder& ColorCacheBullet::getColor(Color_ID id) {
	return cachedColors[id];
}
