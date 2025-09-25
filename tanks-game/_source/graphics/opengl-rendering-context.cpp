#include "opengl-rendering-context.h"

#include <GL/glew.h>
#include <iostream>

static std::string getGLErrorString(GLenum err) {
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

static void printGLError() {
	bool error = false;
	while (true) {
		const GLenum err = glGetError();
		if (err == GL_NO_ERROR)
			break;

		std::cout << "GL Error: " << getGLErrorString(err) << std::endl;
		error = true; //set breakpoint to here when debugging!
	}
	if (!error) {
		std::cout << "no error" << std::endl;
	}
}

void OpenGLRenderingContext::Initialize() {
	//TODO: GLFW initialization should go here
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void OpenGLRenderingContext::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderingContext::Flush() const {
	//for single framebuffer, use glFlush; for double framebuffer, swap the buffers
	//swapping buffers is limited to monitor refresh rate, so I use glFlush
	glFlush();
	//glfwSwapBuffers(glfw_window);
}

void OpenGLRenderingContext::PrintRendererInfo() const {
	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;
}

void OpenGLRenderingContext::PrintRenderingError() const {
	printGLError();
}

void OpenGLRenderingContext::DrawWithoutIndices(int vertexCount) const {
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void OpenGLRenderingContext::DrawUsingIndices(int indexCount) const {
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderingContext::DrawInstanced(int indexCount, int drawCount) const {
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, drawCount);
}
