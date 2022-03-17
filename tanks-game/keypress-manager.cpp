#include "keypress-manager.h"
#include <stdexcept>
#include <GL/glew.h> //to avoid potential freeglut before glew issues
#include <GL/freeglut.h>

std::unordered_map<unsigned char, bool> KeypressManager::normalKeyStates;
std::unordered_map<int, bool> KeypressManager::specialKeyStates;

bool KeypressManager::getNormalKey(unsigned char key) {
	return normalKeyStates[key];
}

bool KeypressManager::getSpecialKey(int key) {
	return specialKeyStates[key];
}

void KeypressManager::Initialize() {
	//regular normal keys
	for (int i = 0; i < 26; i++) {
		normalKeyStates.insert({ 'a'+i, false });
	}
	for (int i = 0; i < 10; i++) {
		normalKeyStates.insert({ '0'+i, false });
	}

	//extra normal keys
	normalKeyStates.insert({ ',', false }); // <
	normalKeyStates.insert({ '.', false }); // >
	normalKeyStates.insert({ '-', false });
	normalKeyStates.insert({ '=', false }); // +
	normalKeyStates.insert({ '\'', false });
	normalKeyStates.insert({ '`', false });
	normalKeyStates.insert({ '[', false });
	normalKeyStates.insert({ ']', false });
	normalKeyStates.insert({ '\\', false });
	normalKeyStates.insert({ '/', false });
	normalKeyStates.insert({ ';', false });
	normalKeyStates.insert({ ' ', false });
	//including a numpad
	normalKeyStates.insert({ '*', false });
	normalKeyStates.insert({ '+', false });

	//other normal keys
	normalKeyStates.insert({ 0x08, false }); //backspace
	normalKeyStates.insert({ 0x1B, false }); //escape
	normalKeyStates.insert({ 0x7F, false }); //delete
	//shift + key not included

	//regular special keys
	specialKeyStates.insert({ GLUT_KEY_UP, false });
	specialKeyStates.insert({ GLUT_KEY_LEFT, false });
	specialKeyStates.insert({ GLUT_KEY_RIGHT, false });
	specialKeyStates.insert({ GLUT_KEY_DOWN, false });

	//other special keys just 'cuz
	specialKeyStates.insert({ GLUT_KEY_F1, false });
	specialKeyStates.insert({ GLUT_KEY_F2, false });
	specialKeyStates.insert({ GLUT_KEY_F3, false });
	specialKeyStates.insert({ GLUT_KEY_F4, false });
	specialKeyStates.insert({ GLUT_KEY_F5, false });
	specialKeyStates.insert({ GLUT_KEY_F6, false });
	specialKeyStates.insert({ GLUT_KEY_F7, false });
	specialKeyStates.insert({ GLUT_KEY_F8, false });
	specialKeyStates.insert({ GLUT_KEY_F9, false });
	specialKeyStates.insert({ GLUT_KEY_F10, false });
	specialKeyStates.insert({ GLUT_KEY_F11, false });
	specialKeyStates.insert({ GLUT_KEY_F12, false });
	specialKeyStates.insert({ GLUT_KEY_PAGE_UP, false });
	specialKeyStates.insert({ GLUT_KEY_PAGE_DOWN, false });
	specialKeyStates.insert({ GLUT_KEY_HOME, false });
	specialKeyStates.insert({ GLUT_KEY_END, false });
	specialKeyStates.insert({ GLUT_KEY_INSERT, false });
}

bool KeypressManager::keyIsSpecialFromString(std::string key) {
	try {
		KeypressManager::specialKeyFromString(key);
		return true;
	}
	catch (std::invalid_argument& e) {
		return false;
	}
	return false;
}

unsigned char KeypressManager::normalKeyFromString(std::string key) {
	if (key.size() == 0) {
		throw std::invalid_argument("ERROR: zero-length key!");
	}

	//follows key names from https://www.autohotkey.com/docs/commands/Send.htm#keynames

	if (key == "Backspace" || key == "BS") {
		return 0x08;
	} else if (key == "Escape" || key == "Esc") {
		return 0x1B;
	} else if (key == "Delete" || key == "Del") {
		return 0x7F;
	}

	if (normalKeyStates.find(key[0]) != normalKeyStates.end()) {
		return key[0];
	}

	throw std::invalid_argument("ERROR: unknown normal key!");
}

int KeypressManager::specialKeyFromString(std::string key) {
	if (key.size() == 0) {
		throw std::invalid_argument("ERROR: zero-length key!");
	}

	//follows key names from https://www.autohotkey.com/docs/commands/Send.htm#keynames
	//full list: https://www.autohotkey.com/docs/KeyList.htm#general

	if (key == "Up") {
		return GLUT_KEY_UP;
	} else if (key == "Left") {
		return GLUT_KEY_LEFT;
	} else if (key == "Right") {
		return GLUT_KEY_RIGHT;
	} else if (key == "Down") {
		return GLUT_KEY_DOWN;
	} else if (key == "F1") {
		return GLUT_KEY_F1;
	} else if (key == "F2") {
		return GLUT_KEY_F2;
	} else if (key == "F3") {
		return GLUT_KEY_F3;
	} else if (key == "F4") {
		return GLUT_KEY_F4;
	} else if (key == "F5") {
		return GLUT_KEY_F5;
	} else if (key == "F6") {
		return GLUT_KEY_F6;
	} else if (key == "F7") {
		return GLUT_KEY_F7;
	} else if (key == "F8") {
		return GLUT_KEY_F8;
	} else if (key == "F9") {
		return GLUT_KEY_F9;
	} else if (key == "F10") {
		return GLUT_KEY_F10;
	} else if (key == "F11") {
		return GLUT_KEY_F11;
	} else if (key == "F12") {
		return GLUT_KEY_F12;
	} else if (key == "PgUp") {
		return GLUT_KEY_PAGE_UP;
	} else if (key == "PgDn") {
		return GLUT_KEY_PAGE_DOWN;
	} else if (key == "Home") {
		return GLUT_KEY_HOME;
	} else if (key == "End") {
		return GLUT_KEY_END;
	} else if (key == "Insert" || key == "Ins") {
		return GLUT_KEY_INSERT;
	}

	throw std::invalid_argument("ERROR: unknown special key!");
}

void KeypressManager::setNormalKey(unsigned char key, int x, int y) {
	normalKeyStates[key] = true;
}
void KeypressManager::unsetNormalKey(unsigned char key, int x, int y) {
	normalKeyStates[key] = false;
}
void KeypressManager::setSpecialKey(int key, int x, int y) {
	specialKeyStates[key] = true;
}
void KeypressManager::unsetSpecialKey(int key, int x, int y) {
	specialKeyStates[key] = false;
}
