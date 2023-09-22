#include "keypress-manager.h"

#include <stdexcept>

std::unordered_map<int, bool> KeypressManager::keyStates;
std::unordered_map<std::string, int> KeypressManager::keyNames;

void KeypressManager::Initialize() {
	//follows key names from https://www.autohotkey.com/docs/commands/Send.htm#keynames
	//full list: https://www.autohotkey.com/docs/KeyList.htm#general

	//regular keys
	for (int i = 0; i < 26; i++) {
		keyNames.insert({ std::string(1, 'a' + i), GLFW_KEY_A + i });
		//keyNames.insert({ std::string(1, 'A' + i), GLFW_KEY_A + i });
	}
	for (int i = 0; i < 10; i++) {
		keyNames.insert({ std::to_string(i), GLFW_KEY_0 + i });
	}
	keyNames.insert({ "Right", GLFW_KEY_RIGHT });
	keyNames.insert({ "Left", GLFW_KEY_LEFT });
	keyNames.insert({ "Down", GLFW_KEY_DOWN });
	keyNames.insert({ "Up", GLFW_KEY_UP });

	//extra keys
	keyNames.insert({ " ", GLFW_KEY_SPACE });
	keyNames.insert({ "'", GLFW_KEY_APOSTROPHE });
	keyNames.insert({ ",", GLFW_KEY_COMMA });
	keyNames.insert({ "-", GLFW_KEY_MINUS }); //hyphen
	keyNames.insert({ ".", GLFW_KEY_PERIOD });
	keyNames.insert({ "/", GLFW_KEY_SLASH });
	keyNames.insert({ ";", GLFW_KEY_SEMICOLON });
	keyNames.insert({ "=", GLFW_KEY_EQUAL });
	keyNames.insert({ "[", GLFW_KEY_LEFT_BRACKET });
	keyNames.insert({ "\\", GLFW_KEY_BACKSLASH });
	keyNames.insert({ "]", GLFW_KEY_RIGHT_BRACKET });

	//other keys
	keyNames.insert({ "Escape", GLFW_KEY_ESCAPE });
	keyNames.insert({ "Esc",    GLFW_KEY_ESCAPE });
	keyNames.insert({ "Enter", GLFW_KEY_ENTER });
	keyNames.insert({ "Tab", GLFW_KEY_TAB });
	keyNames.insert({ "Backspace", GLFW_KEY_BACKSPACE });
	keyNames.insert({ "BS",        GLFW_KEY_BACKSPACE });
	keyNames.insert({ "Insert", GLFW_KEY_INSERT });
	keyNames.insert({ "Ins",    GLFW_KEY_INSERT });
	keyNames.insert({ "Delete", GLFW_KEY_DELETE });
	keyNames.insert({ "Del",    GLFW_KEY_DELETE });

	keyNames.insert({ "PgUp",   GLFW_KEY_PAGE_UP });
	keyNames.insert({ "PageUp", GLFW_KEY_PAGE_UP }); //technically not AHK syntax
	keyNames.insert({ "PgDn",     GLFW_KEY_PAGE_DOWN });
	keyNames.insert({ "PageDown", GLFW_KEY_PAGE_DOWN }); //again
	keyNames.insert({ "Home", GLFW_KEY_HOME });
	keyNames.insert({ "End", GLFW_KEY_END });
	keyNames.insert({ "Pause", GLFW_KEY_PAUSE });

	for (int i = 0; i < 25; i++) {
		keyNames.insert({ "F"+std::to_string(i+1), GLFW_KEY_F1 + i });
		//GLFW goes all the way up to F25 (and AHK up to F24), though most keyboards cap out at F12
	}

	//numpad
	for (int i = 0; i < 10; i++) {
		keyNames.insert({ "Numpad"+std::to_string(i), GLFW_KEY_KP_0 + i });
	}
	keyNames.insert({ "NumpadDot", GLFW_KEY_KP_DECIMAL });
	keyNames.insert({ "NumpadDiv", GLFW_KEY_KP_DIVIDE });
	keyNames.insert({ "NumpadMult", GLFW_KEY_KP_MULTIPLY });
	keyNames.insert({ "NumpadSub", GLFW_KEY_KP_SUBTRACT });
	keyNames.insert({ "NumpadAdd", GLFW_KEY_KP_ADD });
	keyNames.insert({ "NumpadEnter", GLFW_KEY_KP_ENTER });
	//keyNames.insert({ "", GLFW_KEY_KP_EQUAL }); //very rare for numpads to have this, also AHK doesn't have an identifier for it

	//modifiers
	keyNames.insert({ "Shift",  GLFW_KEY_LEFT_SHIFT });
	keyNames.insert({ "LShift", GLFW_KEY_LEFT_SHIFT });
	keyNames.insert({ "Control",  GLFW_KEY_LEFT_CONTROL });
	keyNames.insert({ "Ctrl",     GLFW_KEY_LEFT_CONTROL });
	keyNames.insert({ "LControl", GLFW_KEY_LEFT_CONTROL });
	keyNames.insert({ "LCtrl",    GLFW_KEY_LEFT_CONTROL });
	keyNames.insert({ "Alt",  GLFW_KEY_LEFT_ALT });
	keyNames.insert({ "LAlt", GLFW_KEY_LEFT_ALT });
	keyNames.insert({ "LWin",   GLFW_KEY_LEFT_SUPER });
	keyNames.insert({ "Win",    GLFW_KEY_LEFT_SUPER }); //not AHK
	keyNames.insert({ "LSuper", GLFW_KEY_LEFT_SUPER }); //same
	keyNames.insert({ "Super",  GLFW_KEY_LEFT_SUPER }); //same
	keyNames.insert({ "RShift", GLFW_KEY_RIGHT_SHIFT });
	keyNames.insert({ "RControl", GLFW_KEY_RIGHT_CONTROL });
	keyNames.insert({ "RCtrl",    GLFW_KEY_RIGHT_CONTROL });
	keyNames.insert({ "RAlt", GLFW_KEY_RIGHT_ALT });
	keyNames.insert({ "RWin",   GLFW_KEY_RIGHT_SUPER });
	keyNames.insert({ "RSuper", GLFW_KEY_RIGHT_SUPER }); //not AHK
	keyNames.insert({ "AppsKey", GLFW_KEY_MENU }); //what a strange name for the key
	keyNames.insert({ "Menu",    GLFW_KEY_MENU }); //the key that brings up the context menu (right click)

	//add everything to the actual key state lookup
	for (const std::pair<std::string, int>& key : keyNames) {
		if (keyStates.find(key.second) == keyStates.end()) { [[likely]]
			//some keys (like Escape) have more than one string definition
			keyStates.insert({ key.second, false });
		}
	}
}

bool KeypressManager::getKeyStateFromCode(int key) {
	return keyStates[key];
}

int KeypressManager::keyFromString(std::string key) {
	if (keyNames.find(key) == keyNames.end()) { [[unlikely]]
		throw std::invalid_argument("ERROR: unknown key!");
	} else {
		return keyNames[key];
	}
}

void KeypressManager::unsetKeyState(std::string key) {
	keyStates[keyFromString(key)] = false;
}

void KeypressManager::keyCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action) {
		case GLFW_PRESS:
			keyStates[key] = true;
			break;
		case GLFW_RELEASE:
			keyStates[key] = false;
			break;
		case GLFW_REPEAT:
			//nothing
			break;
	}
}
