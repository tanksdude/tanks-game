#include "color-cache.h"

#include "color-mixer.h"

std::unordered_map<std::string, ColorValueHolder> ColorCacheBullet::cachedColors;

void ColorCacheBullet::invalidateColors() {
	cachedColors.clear();
}

bool ColorCacheBullet::colorExists(const std::string& identifier) {
	return (cachedColors.find(identifier) != cachedColors.end());
}

const ColorValueHolder& ColorCacheBullet::getColor(const std::string& identifier) {
	return cachedColors.at(identifier);
}

const ColorValueHolder& ColorCacheBullet::insertColor(const std::string& identifier, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ identifier, ColorMixer::mix(colors, num) });
	return cachedColors.at(identifier);
}
