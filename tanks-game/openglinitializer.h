#pragma once
#include <string>

class OpenGLInitializer {
private:
	
public:
	//initializes FreeGLUT (and GLEW)
	static void Initialize(int* argc, char** argv, std::string windowName);
	static void Initialize(int* argc, char** argv, std::string windowName, int startX, int startY);
};
