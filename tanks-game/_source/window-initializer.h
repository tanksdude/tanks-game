#pragma once
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowInitializer {
public:
	static GLFWwindow* glfw_window;
	static void windowResizeFunc(GLFWwindow* window, int w, int h);
	static void windowCoordsToGameCoords(double inputX, double inputY, int& actualX, int& actualY);
	static GLFWmonitor* get_current_monitor(GLFWwindow* window);

private:
	static int window_width;
	static int window_height;
	static int gamewindow_width; //width of game inside window
	static int gamewindow_height;

	static bool currently_fullscreen;
	static int old_window_width; //for GLFW toggling fullscreen
	static int old_window_height;
	static int old_window_xpos;
	static int old_window_ypos;

public:
	static void WindowInitialize(int* argc, char** argv, std::string windowName); //initialize GLFW and GLEW
	static void WindowInitialize(int* argc, char** argv, std::string windowName, int startX, int startY);
	static void WindowInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, double sizeMultiplier);
	static void SetWindowIcon(const char* image_path); //called during WindowInitialize; 32x32 seems to be the size OSes like the most

	static void BeginningStuff(); //just for fullscreen handling
	static void UninitializeWindow();

private:
	WindowInitializer() = delete;
	WindowInitializer(const WindowInitializer&) = delete;
};
