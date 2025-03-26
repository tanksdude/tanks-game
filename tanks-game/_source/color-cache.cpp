#include "color-cache.h"

#include "color-mixer.h"

std::unordered_map<std::string, int> ColorCacheBullet::identifierToID;
std::unordered_map<int, ColorValueHolder> ColorCacheBullet::cachedColors;
int ColorCacheBullet::nextFreeID = 0;

void ColorCacheBullet::invalidateCachedColors() {
	cachedColors.clear();
}

int ColorCacheBullet::getColorID(const std::string& identifier) {
	if (identifierToID.find(identifier) == identifierToID.end()) [[unlikely]] {
		identifierToID.insert({ identifier, ++nextFreeID });
		return nextFreeID;
	}
	return identifierToID.at(identifier);
}

const ColorValueHolder& ColorCacheBullet::insertColor(int id, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ id, ColorMixer::mix(colors, num) });
	return cachedColors.at(id);
}

const ColorValueHolder& ColorCacheBullet::getColor(int id) {
	return cachedColors.at(id);
}
