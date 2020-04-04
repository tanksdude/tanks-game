#pragma once

#include "renderer.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include "constants.h"
#include <iostream>

glm::mat4 Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
glm::mat4 Renderer::getProj() { return proj; }

std::unordered_map<std::string, Shader*> Renderer::shaderCache;
unsigned int Renderer::currentShader = -1;
unsigned int Renderer::currentVertexArray = -1;
unsigned int Renderer::currentIndexBuffer = -1;

void Renderer::Initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Shader* shader = new Shader("res/shaders/main-vertex.shader", "res/shaders/main-fragment.shader");
	shaderCache.insert({ "main", shader });
	bindShader(shader); //the main shader will be used most often so it gets binded at start

	shader = new Shader("res/shaders/default-vertex.shader", "res/shaders/default-fragment.shader");
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