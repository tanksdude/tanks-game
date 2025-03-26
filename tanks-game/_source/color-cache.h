#pragma once
#include <string>
#include <unordered_map>
#include "color-value-holder.h"

//currently, color cache identifiers are separated by "|", so don't put that in your power's name
//TODO: when powers get added, check through every power's identifier to make sure it doesn't have a "|", exit program if it does (make sure that's only for debug builds)

class ColorCacheBullet {
protected:
	static std::unordered_map<std::string, int> identifierToID;
	static std::unordered_map<int, ColorValueHolder> cachedColors;
	static int nextFreeID;

public:
	static void invalidateCachedColors(); //called at the start of a new frame; does not invalidate IDs

	[[nodiscard]] static int getColorID(const std::string& identifier); //increments nextFreeID if doesn't have an ID
	[[nodiscard]] static const ColorValueHolder& insertColor(int id, const ColorValueHolder* colors, int num);
	static inline bool colorIsCached(int id) { return cachedColors.find(id) != cachedColors.end(); }
	static const ColorValueHolder& getColor(int id);

private:
	ColorCacheBullet() = delete;
	ColorCacheBullet(const ColorCacheBullet&) = delete;
};
