#pragma once
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class KeypressManager {
private:
	static std::unordered_map<int, bool> keyStates;
	static std::unordered_map<std::string, int> keyNames;

public:
	static bool getKeyStateFromCode(int key);
	static int keyFromString(std::string key);
	static inline bool getKeyState(std::string key) {
		return getKeyStateFromCode(keyFromString(key));
	}
	static void unsetKeyState(std::string key); //used for toggling fullscreen

	static void keyCallbackFunc(GLFWwindow*, int key, int scancode, int action, int mods);

	static void Initialize();

private:
	KeypressManager() = delete;
	KeypressManager(const KeypressManager&) = delete;
};
