#pragma once
#include <string>
#include <unordered_map>

class KeypressManager {
private:
	static std::unordered_map<unsigned char, bool> normalKeyStates; //general keyboard
	static std::unordered_map<int, bool> specialKeyStates; //non-ASCII

public:
	static bool getNormalKey(unsigned char key);
	static bool getSpecialKey(int key);

	//glutKeyboardFunc & friends do not like their functions calling other functions, so KeypressManager has its own functions (so the hashmaps can stay private)
	static void setNormalKey(unsigned char key, int x, int y);
	static void unsetNormalKey(unsigned char key, int x, int y);
	static void setSpecialKey(int key, int x, int y);
	static void unsetSpecialKey(int key, int x, int y);

	static bool keyIsSpecialFromString(std::string key);
	static unsigned char normalKeyFromString(std::string key);
	static int specialKeyFromString(std::string key);

	static void Initialize();

private:
	KeypressManager() = delete;
	KeypressManager(const KeypressManager&) = delete;
};
