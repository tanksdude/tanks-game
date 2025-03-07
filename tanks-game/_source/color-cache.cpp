#include "color-cache.h"

#include "color-mixer.h"

//std::unordered_map<std::string, ColorValueHolder> ColorCachePowerSquare::cachedColors;
//std::unordered_map<std::string, ColorValueHolder> ColorCacheTank::cachedColors;
std::unordered_map<std::string, ColorValueHolder> ColorCacheBullet::cachedColors;

/*
bool ColorCachePowerSquare::colorExists(const std::string& identifier) {
	return (cachedColors.find(identifier) != cachedColors.end());
}
bool ColorCacheTank::colorExists(const std::string& identifier) {
	return (cachedColors.find(identifier) != cachedColors.end());
}
*/
bool ColorCacheBullet::colorExists(const std::string& identifier) {
	return (cachedColors.find(identifier) != cachedColors.end());
}

/*
const ColorValueHolder& ColorCachePowerSquare::getColor(const std::string& identifier) {
	return cachedColors.at(identifier);
}
const ColorValueHolder& ColorCacheTank::getColor(const std::string& identifier) {
	return cachedColors.at(identifier);
}
*/
const ColorValueHolder& ColorCacheBullet::getColor(const std::string& identifier) {
	return cachedColors.at(identifier);
}

/*
const ColorValueHolder& ColorCachePowerSquare::insertColor(const std::string& identifier, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ identifier, ColorMixer::mix(colors, num) });
	return cachedColors.at(identifier);
}
const ColorValueHolder& ColorCacheTank::insertColor(const std::string& identifier, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ identifier, ColorMixer::mix(colors, num) });
	return cachedColors.at(identifier);
}
*/
const ColorValueHolder& ColorCacheBullet::insertColor(const std::string& identifier, const ColorValueHolder* colors, int num) {
	cachedColors.insert({ identifier, ColorMixer::mix(colors, num) });
	return cachedColors.at(identifier);
}
