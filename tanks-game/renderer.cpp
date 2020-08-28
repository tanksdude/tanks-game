#include "renderer.h"
//#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include "constants.h"
#include "keypressmanager.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

glm::mat4 Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
glm::mat4 Renderer::getProj() { return proj; }
int Renderer::window_width = GAME_WIDTH*2 * 1.25;
int Renderer::window_height = GAME_HEIGHT*2 * 1.25;
int Renderer::gamewindow_width = Renderer::window_width;
int Renderer::gamewindow_height = Renderer::window_height;

// Handles window resizing (FreeGLUT event function)
void Renderer::windowResizeFunc(int w, int h) {
	Renderer::window_width = w;
	Renderer::window_height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range (for CPU)
	const double appXmin = 0.0;
	const double appXmax = GAME_WIDTH;
	const double appYmin = 0.0;
	const double appYmax = GAME_HEIGHT;

	// Define that OpenGL should use the whole window for rendering (on CPU)
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disired ones (CPU only)
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) { //too wide
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = 0;
		winXmin = center - (center - appXmin) * scale;
		winXmax = center + (appXmax - center) * scale;
		winYmin = appYmin;
		winYmax = appYmax;

		Renderer::proj = glm::ortho(0.0f, float(GAME_WIDTH*scale), 0.0f, (float)GAME_HEIGHT); //GPU
		Renderer::gamewindow_width = Renderer::window_height * (GAME_WIDTH/GAME_HEIGHT);
		Renderer::gamewindow_height = Renderer::window_height;
	}
	else { //too tall
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = 0;
		winYmin = center - (center - appYmin) * scale;
		winYmax = center + (appYmax - center) * scale;
		winXmin = appXmin;
		winXmax = appXmax;

		Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, float(GAME_HEIGHT*scale)); //GPU
		Renderer::gamewindow_width = Renderer::window_width;
		Renderer::gamewindow_height = Renderer::window_width * (GAME_HEIGHT/GAME_WIDTH);
	}

	// Now we use glOrtho to set up our viewing frustum (CPU only)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

//actual renderer code:

std::unordered_map<std::string, Shader*> Renderer::shaderCache;
unsigned int Renderer::currentShader = -1;
unsigned int Renderer::currentVertexArray = -1;
unsigned int Renderer::currentIndexBuffer = -1;

void Renderer::BeginningStuff() {
	if (KeypressManager::getSpecialKey(GLUT_KEY_F1)) {
		glutFullScreenToggle();
		KeypressManager::unsetSpecialKey(GLUT_KEY_F1, 0, 0);
	}
}

void Renderer::PreInitialize(int* argc, char** argv, std::string windowName) {
	Renderer::PreInitialize(argc, argv, windowName, 60, 60);
}

void Renderer::PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY) {
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

void Renderer::Initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Shader* shader = new Shader("res/shaders/main.vert", "res/shaders/main.frag");
	shaderCache.insert({ "main", shader });
	bindShader(shader); //the main shader will be used most often so it gets binded at start

	shader = new Shader("res/shaders/default.vert", "res/shaders/default.frag");
	shaderCache.insert({ "default", shader });
}

glm::mat4 Renderer::GenerateMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY) {
	glm::mat4 trans = glm::translate(proj, glm::vec3(transX, transY, 0.0f));
	if (rotateAngle == 0) {
		return glm::scale(trans, glm::vec3(scaleX, scaleY, 0));
	}
	glm::mat4 rot = glm::rotate(trans, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::scale(rot, glm::vec3(scaleX, scaleY, 0));
}

inline void Renderer::bindShader(Shader* shader) {
	if (currentShader != shader->getRendererID()) {
		shader->Bind();
		currentShader = shader->getRendererID();
	}
}

inline void Renderer::bindShader(const Shader& shader) {
	if (currentShader != shader.getRendererID()) {
		shader.Bind();
		currentShader = shader.getRendererID();
	}
}

inline void Renderer::bindVertexArray(const VertexArray& va) {
	if (currentVertexArray != va.getRendererID()) {
		va.Bind();
		currentVertexArray = va.getRendererID();
	}
}

inline void Renderer::bindIndexBuffer(const IndexBuffer& ib) {
	if (currentIndexBuffer != ib.getRendererID()) {
		ib.Bind();
		currentIndexBuffer = ib.getRendererID();
	}
}

Shader* Renderer::getShader(std::string s) {
	//return shaderCache[s];
	
	auto get = shaderCache.find(s);
	if (get != shaderCache.end()) {
		Shader* shader = shaderCache[s];
		bindShader(shader);
		return shader;
	}
	//else shader wasn't found
	std::cout << "Could not find '" << s << "' shader!" << std::endl;

	//return the magenta shader, just so there's something
	get = shaderCache.find("default");
	if (get != shaderCache.end()) {
		Shader* shader = shaderCache[s];
		bindShader(shader);
		return shader;
	}
	//else big uh-oh
	std::cout << "Could not find the default shader!" << std::endl;

	return nullptr; //default magenta shader is missing
}

void Renderer::Unbind(const VertexArray& va) {
	if (currentVertexArray == va.getRendererID()) {
		va.Unbind();
		currentVertexArray = -1;
	}
}

void Renderer::Unbind(const IndexBuffer& ib) {
	if (currentVertexArray == ib.getRendererID()) {
		ib.Unbind();
		currentIndexBuffer = -1;
	}
}

void Renderer::Unbind(const Shader& s) {
	if (currentVertexArray == s.getRendererID()) {
		s.Unbind();
		currentShader = -1;
	}
}

void Renderer::UnbindAll() {
	glBindVertexArray(0); //vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //index buffer
	glUseProgram(0); //shader

	currentVertexArray = -1;
	currentIndexBuffer = -1;
	currentShader = -1;
}

std::string Renderer::getErrorString(GLenum err) {
	//gotten from https://codeyarns.com/2015/09/14/how-to-check-error-in-opengl/
	switch (err) {
		case GL_NO_ERROR:          return "No error";
		case GL_INVALID_ENUM:      return "Invalid enum";
		case GL_INVALID_VALUE:     return "Invalid value";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_STACK_OVERFLOW:    return "Stack overflow";
		case GL_STACK_UNDERFLOW:   return "Stack underflow";
		case GL_OUT_OF_MEMORY:     return "Out of memory";
		default:                   return "Unknown error";
	}
}

void Renderer::printGLError() {
	bool error = false;
	while (true) {
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << getErrorString(err) << std::endl;
		error = true; //set breakpoint to here when debugging!
	}
	if (!error) {
		std::cout << "no error" << std::endl;
	}
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Clear(int flags) {
	glClear(flags);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
	bindShader(shader);
	bindVertexArray(va);
	bindIndexBuffer(ib);

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) {
	bindShader(shader);
	bindVertexArray(va);
	bindIndexBuffer(ib);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, GLenum type, GLint first, GLsizei count) {
	bindShader(shader);
	bindVertexArray(va);
	currentIndexBuffer = -1;

	glDrawArrays(type, first, count);
}

void Renderer::Draw(GLenum type, GLint first, GLsizei count) {
	glDrawArrays(type, first, count);
}

void Renderer::Cleanup() {
	glDisableVertexAttribArray(0); //disable vertex attribute to avoid issues
}
