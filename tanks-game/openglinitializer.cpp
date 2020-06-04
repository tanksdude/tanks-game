#include "openglinitializer.h"
#include "renderer.h" //just for window_width and window_height
#include <GL/glew.h>
#include <GL/freeglut.h>

void OpenGLInitializer::Initialize(int* argc, char** argv, std::string windowName) {
	OpenGLInitializer::Initialize(argc, argv, windowName, 60, 60);
}

void OpenGLInitializer::Initialize(int* argc, char** argv, std::string windowName, int startX, int startY) {
	// Initialize GLUT
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	//thanks to https://community.khronos.org/t/wglmakecurrent-issues/62656/3 for solving why a draw call would take ~15ms for no reason (it's just the V-sync time)

	// Setup window position, size, and title
	glutInitWindowPosition(startX, startY);
	glutInitWindowSize(Renderer::window_width, Renderer::window_height);
	glutCreateWindow(windowName.c_str());

	// Setup some OpenGL options
	glPointSize(2);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialize glew
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		throw "glew failed";
	}

}
