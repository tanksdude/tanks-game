#pragma once
#include "constants.h"
#include <string>
#include <unordered_map>
#include "color-value-holder.h"

//currently, color cache identifiers are separated by "|", so don't put that in your power's name
//TODO: when powers get added, check through every power's identifier to make sure it doesn't have a "|", exit program if it does (make sure that's only for debug builds)

class ColorCacheBullet {
protected:
	static std::unordered_map<std::string, Color_ID> identifierToID;
	static std::unordered_map<Color_ID, ColorValueHolder> cachedColors;
	static Color_ID nextFreeID;

public:
	static void invalidateCachedColors(); //called at the start of a new frame; does not invalidate IDs

	[[nodiscard]] static Color_ID getColorID(const std::string& identifier); //increments nextFreeID if doesn't have an ID
	[[nodiscard]] static const ColorValueHolder& insertColor(Color_ID id, const ColorValueHolder* colors, int num);
	static inline bool colorIsCached(Color_ID id) { return cachedColors.find(id) != cachedColors.end(); }
	static const ColorValueHolder& getColor(Color_ID id);

private:
	ColorCacheBullet() = delete;
	ColorCacheBullet(const ColorCacheBullet&) = delete;
};
