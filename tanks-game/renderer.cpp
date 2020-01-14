#pragma once

#include "renderer.h"
#include <GL/glew.h>
#include <iostream>

std::unordered_map<std::string, Shader*> Renderer::shaderCache;
unsigned int Renderer::currentShader = -1;
unsigned int Renderer::currentVertexArray = -1;
unsigned int Renderer::currentIndexBuffer = -1;

void Renderer::Initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Shader* shader = new Shader("res/shaders/uniform-vertex.shader", "res/shaders/uniform-fragment.shader");
	shader->Bind(); //uniform shader will be used most often (probably entirely) so it gets binded at start
	currentShader = shader->getRendererID();
	shaderCache.insert({ "uniform", shader });

	shader = new Shader("res/shaders/translation-vertex.shader", "res/shaders/translation-fragment.shader");
	shaderCache.insert({ "translation", shader });

	shader = new Shader("res/shaders/scale-vertex.shader", "res/shaders/scale-fragment.shader");
	shaderCache.insert({ "scaling", shader });

	shader = new Shader("res/shaders/rotate-vertex.shader", "res/shaders/rotate-fragment.shader");
	shaderCache.insert({ "rotation", shader });

	shader = new Shader("res/shaders/default-vertex.shader", "res/shaders/default-fragment.shader");
	shaderCache.insert({ "default", shader });
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
	glUseProgram(0); //shader
	glBindVertexArray(0); //array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //index buffer
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