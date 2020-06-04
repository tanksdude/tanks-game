#include "keypressmanager.h"
#include <GL/freeglut.h>

std::unordered_map<unsigned char, bool> KeypressManager::normalKeyStates;
std::unordered_map<int, bool> KeypressManager::specialKeyStates;

bool KeypressManager::getNormalKey(unsigned char key) {
	return normalKeyStates[key];
}

bool KeypressManager::getSpecialKey(int key) {
	return specialKeyStates[key];
}

void KeypressManager::initialize() {
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
